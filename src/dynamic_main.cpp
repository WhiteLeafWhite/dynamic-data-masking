// copyright 2025 Yanghaoyang

#include <arpa/inet.h>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <event2/listener.h>

#include <codecvt>
#include <cstring>
#include <iostream>
#include <locale>
#include <mutex>
#include <string>
#include <vector>

#include "DetectorManager.h"
#include "MysqlOperator.h"
#include "PhoneDetector.h"
#include "SensitiveFieldsManager.h"
#include "AccountManager.h"

#define SEPPD_TEST

// MySQL 连接配置
constexpr const char* DB_HOST = "localhost";
constexpr const char* DB_USER = "root";
constexpr const char* DB_PASS = "";
constexpr const char* DB_NAME = "bisheInformation";
constexpr unsigned int DB_PORT = 3306;

// 配置文件
constexpr const char* CONFIGURATIONPATH = "./mask-configuration";
constexpr const char* ACCOUNT_INFO_PATH = "./mask-configuration";

// 信息常量
constexpr const char* SQL_INVALID = "查询无效！";
constexpr const char* NOT_LOGIN_MESSAGE = "请先登录！";
constexpr const char* LOGIN_FAIL = "登陆失败！";
constexpr const char* LOGIN_SUCCESS = "登陆成功！您是：";

std::string whoYouAre(int indexLevel){
  if(indexLevel == 0) return "登陆成功！您是：普通用户";
  else if(indexLevel == 1) return "登陆成功！您是：大师";
  else return "登陆成功！您是：神奇人";
}

// 保护 MySQL 连接的互斥锁
std::mutex db_mutex;

//用户等级

int level = 0;
int isLogIn = false;
AccountManager am(ACCOUNT_INFO_PATH);

// 处理客户端消息的回调函数
void on_read(struct bufferevent* bev, void* ctx) {
  char buffer[256];
  memset(buffer, 0, sizeof(buffer));

  // 从客户端读取数据
  int n = bufferevent_read(bev, buffer, sizeof(buffer) - 1);
  if (n <= 0) {
    return;
  }

  // 输出客户端发送的消息
  std::string client_message(buffer);
  std::cout << "Received message: " << client_message << std::endl;

  if (client_message == "1") {
    return;
  } else if (client_message == "0") {
    return;
  }

  if(client_message.substr(0,5) == "login"){
    std::cout << "正在登陆：" << client_message << std::endl;
    level = am.Login(client_message.substr(5));
    if(level == -1){
      isLogIn = false;
      bufferevent_write(bev, LOGIN_FAIL, strlen(LOGIN_FAIL));
    }
    else{
      isLogIn = true;
      bufferevent_write(bev, whoYouAre(level).c_str(), whoYouAre(level).length());
    }
    return;
  }

  if(!isLogIn){
    bufferevent_write(bev, NOT_LOGIN_MESSAGE, strlen(NOT_LOGIN_MESSAGE));
    return;
  }

  MysqlOperator* mysqlOperator =
      new MysqlOperator(DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_PORT);
  SensitiveFieldsManager manager(CONFIGURATIONPATH, level);
  bool isValidQuery = manager.generateSensitiveResult(client_message);

  // 无效语句则提前结束，防止程序崩溃
  if (!isValidQuery) {
    bufferevent_write(bev, SQL_INVALID, strlen(SQL_INVALID));
    return;
  }

  std::vector<std::vector<std::string> > res =
      mysqlOperator->execute(client_message);

  delete (mysqlOperator);

  // 向客户端回显消息
  for (auto s : res) {
    #ifndef SEPPD_TEST
    std::cout<<std::endl;
    #endif
    for (int i = 0; i < s.size(); ++i) {
      std::string temp = s[i];
      if (manager.isSensitive(i)) {
        temp = manager.mask(i, temp);
      }
      temp.push_back(' ');
      bufferevent_write(bev, temp.c_str(), temp.length());
      #ifndef SEPPD_TEST
      std::cout<<temp<<"#";
      #endif
    }
    bufferevent_write(bev, "\n", 1);
  }
}

// 处理事件的回调函数（例如：断开连接或出错）
void on_event(struct bufferevent* bev, int16_t events, void* ctx) {
  if (events & BEV_EVENT_EOF) {
    bufferevent_free(bev);
    std::cout << " bye bye~" << std::endl;
    return;
  } else if (events & BEV_EVENT_ERROR) {
    bufferevent_free(bev);
  }
}

// 处理新客户端连接的回调函数
void on_accept(struct evconnlistener* listener, evutil_socket_t fd,
               struct sockaddr* addr, int socklen, void* ctx) {
  struct event_base* base = (struct event_base*)ctx;

  // 为新连接创建 bufferevent
  struct bufferevent* bev =
      bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
  bufferevent_setcb(bev, on_read, nullptr, on_event, nullptr);
  bufferevent_enable(bev, EV_READ | EV_WRITE);

  std::cout<< "new client!"<<std::endl;

}

// 主函数：初始化服务端并监听端口
int main() {
  struct event_base* base;
  struct evconnlistener* listener;
  struct sockaddr_in sin;

  // 初始化 libevent
  base = event_base_new();
  if (!base) {
    std::cerr << "Could not initialize libevent" << std::endl;
    return 1;
  }

  // 配置监听地址
  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = htonl(INADDR_ANY);
  sin.sin_port = htons(9876);

  // 创建监听器
  listener =
      evconnlistener_new_bind(base, on_accept, reinterpret_cast<void*>(base),
                              LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, -1,
                              (struct sockaddr*)&sin, sizeof(sin));
  if (!listener) {
    std::cerr << "Could not create a listener!" << std::endl;
    return 1;
  }

  std::cout << "Server is running on port 9876, waiting for connections..."
            << std::endl;

  // 启动事件循环
  event_base_dispatch(base);

  // 清理资源
  evconnlistener_free(listener);
  event_base_free(base);

  return 0;
}
