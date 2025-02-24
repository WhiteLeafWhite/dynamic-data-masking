#pragma once
#include <mysql/mysql.h>
#include <mutex>
#include <vector>
#include <string>

class MysqlOperator{
private:
    std::mutex db_mutex;
    const char* DB_HOST;
    const char* DB_USER;
    const char* DB_PASS;
    const char* DB_NAME;
    unsigned int DB_PORT;
    MYSQL* conn;
public:
    MysqlOperator(const char* dbHost, const char* dbUser, const char* dbPass, 
    const char* dbName, unsigned int dbPort);
    ~MysqlOperator();
    std::vector<std::vector<std::string> > execute(const std::string& query);
};