// copyright 2025 Yanghaoyang

#include "AccountManager.h"

#include <iostream>
#include <string>
#include <utility>
#include <map>
#include <fstream>
#include <sodium.h>
#include <cstring>

#define CRYPTO_OPS_LIMIT crypto_pwhash_OPSLIMIT_MODERATE
#define CRYPTO_MEM_LIMIT crypto_pwhash_MEMLIMIT_MODERATE
#define HASH_LEN crypto_pwhash_BYTES


const char* ACCOUNT_CONFIGFILE_NAME = "/account-configuration.json";

std::string hashPassword(const std::string &password) {
  std::string SALT = "viva odden";
  char hashedPassword[crypto_pwhash_STRBYTES];

  std::string passAndSalt = password + SALT;

  if (crypto_pwhash_str(hashedPassword, passAndSalt.c_str(), passAndSalt.size(),
  CRYPTO_OPS_LIMIT, CRYPTO_MEM_LIMIT) != 0) {
      throw std::runtime_error("密码哈希失败!");
  }

  return std::string(hashedPassword);
}

AccountManager::AccountManager(const std::string &path){

  if (sodium_init() < 0) {
    std::cerr << "libsodium 初始化失败!" << std::endl;
    return ;
  }

    accountConfigurationFile = path;
    std::ifstream input_file;
    input_file.open(accountConfigurationFile + ACCOUNT_CONFIGFILE_NAME);

    if (!input_file.is_open()) {
        std::cerr << "AccountManager: Failed to open file!" << std::endl;
        return;
      }
      else{
        std::cout << "AccountManager: open file successfully!" << std::endl;
      }

    try {
    input_file >> configureJson;  // 将文件内容读取到 json 对象
    } catch (const std::exception& e) {
    std::cerr << "AccountManager: Error parsing JSON: " << e.what() << std::endl;
    return;
    }

    for(auto info : configureJson){
        accountInfos.insert(std::make_pair(info["username"], info["hashed-password"]));
        // std::cout<<"读取："<<info["username"]<<" "<<info["hashed-password"]<<std::endl;
    } 
}

void AccountManager::re_readFile(){
  std::ifstream input_file;
    input_file.open(accountConfigurationFile + ACCOUNT_CONFIGFILE_NAME);

    if (!input_file.is_open()) {
        std::cerr << "AccountManager: Failed to open file!" << std::endl;
        return;
      }
      else{
        std::cout << "AccountManager: open file successfully!" << std::endl;
      }

    try {
    input_file >> configureJson;  // 将文件内容读取到 json 对象
    } catch (const std::exception& e) {
    std::cerr << "AccountManager: Error parsing JSON: " << e.what() << std::endl;
    return;
    }

    accountInfos.clear();

    for(auto info : configureJson){
      accountInfos.insert(std::make_pair(info["username"], info["hashed-password"]));
      // std::cout<<"读取："<<info["username"]<<" "<<info["hashed-password"]<<std::endl;
    } 
}

std::string AccountManager::Login(const std::string &name, const std::string &password){
  this->re_readFile();
  std::cout<<"AM.cpp:   "<<name<<"##"<<password<<std::endl;
  if(!accountInfos.count(name)) return "fail"; // 如果没有这个用户，那么就直接返回false
  std::string SALT = password + "viva odden";
  bool success = crypto_pwhash_str_verify(accountInfos[name].c_str(), SALT.c_str(), SALT.size()) == 0;
  if(success){
    for(auto info : configureJson){
      if(info["username"] == name){
        return info["identity"];
      }
    }
  }
  return "fail";
}

std::string AccountManager::Login(const std::string &clientMessage){
  size_t userPos = clientMessage.find("username:");
  size_t passPos = clientMessage.find("password:");


  size_t userStart = userPos + 9; // "username:"
  size_t userEnd = clientMessage.find(';', userStart);
  std::string username = clientMessage.substr(userStart, userEnd - userStart);


  size_t passStart = passPos + 9; // "password:"
  std::string password = clientMessage.substr(passStart);

  return this->Login(username, password);
}
