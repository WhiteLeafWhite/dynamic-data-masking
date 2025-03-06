// copyright 2025 Yanghaoyang
#pragma once

#include <string>

class BaseMasking;

class DbField{
private:
    std::string fieldName;
    std::string tableName;
    BaseMasking* maskSolution;
public:
    DbField(const std::string &fn, const std::string &tn, BaseMasking* ms);
    ~DbField();
    std::string getFieldName();
    std::string getTableName();
    BaseMasking* getMaskSolution();
};
