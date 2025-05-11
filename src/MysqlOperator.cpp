// copyright 2025 Yanghaoyang

#include "MysqlOperator.h"

#include <iostream>
#include <string>
#include <vector>

MysqlOperator::MysqlOperator(const char* dbHost, const char* dbUser,
                             const char* dbPass, const char* dbName,
                             unsigned int dbPort)
    : DB_HOST(dbHost),
      DB_USER(dbUser),
      DB_PASS(dbPass),
      DB_NAME(dbName),
      DB_PORT(dbPort) {
  std::lock_guard<std::mutex> lock(db_mutex);

  conn = mysql_init(nullptr);
  if (!conn) {
    std::cerr << "MySQL initialization failed" << std::endl;
    return;
  }

  if (!mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_PORT,
                          nullptr, 0)) {
    std::cerr << "Failed to connect to MySQL: " << mysql_error(conn)
              << std::endl;
    mysql_close(conn);
    return;
  }

  std::cout << "Connected to MySQL database successfully!" << std::endl;
}

MysqlOperator::~MysqlOperator() { mysql_close(conn); }

std::vector<std::vector<std::string> > MysqlOperator::execute(
    const std::string& query) {
  // 用到的变量
  std::vector<std::vector<std::string> > ret;
  MYSQL_RES* res;
  MYSQL_ROW row;
  // 查询
  if (mysql_query(conn, query.c_str())) {
    std::cerr << "Query failed: " << mysql_error(conn) << std::endl;
    mysql_close(conn);
    return ret;
  }

  res = mysql_store_result(conn);
  if (!res) {
    std::cerr << "Fetching result failed: " << mysql_error(conn) << std::endl;
    mysql_close(conn);
    return ret;
  }

  int num_fields = mysql_num_fields(res);

  // 假设有数据
  while ((row = mysql_fetch_row(res))) {
    std::vector<std::string> temp;
    for (unsigned int i = 0; i < num_fields; ++i) {
      if (row[i] != nullptr) {
        temp.push_back(std::string(row[i]));
      }
    }
    ret.push_back(temp);
  }

  return ret;
}
