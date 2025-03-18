// copyright 2025 Yanghaoyang

#pragma once

#include<string>
#include<map>

#include "./json.hpp"

using json = nlohmann::json;

class AccountManager{
private:
    std::string accountConfigurationFile;
    json configureJson;
    std::map<std::string,std::string> accountInfos;
public:
    AccountManager() = default;
    AccountManager(const std::string &path);
    int Login(const std::string &name, const std::string &password);
    int Login(const std::string &clientMessage);
};
