// copyright 2025 Yanghaoyang
#pragma once

#include <string>
#include <vector>

#include "BaseMasking.h"
#include "DbField.h"

class DbTable{
private:
    std::string tableName;
    std::vector<DbField> fields;
public:
    DbTable() = default;
    DbTable(const std::string& tname);
    void addField(const std::string &fname, BaseMasking* ms);
    DbField getFiled(const std::string &fname);
    std::string getTableName();
};
