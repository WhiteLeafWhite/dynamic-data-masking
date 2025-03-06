// copyright 2025 Yanghaoyang

#include <string>
#include <vector>

#include "DbTable.h"

DbTable(const std::string& tname){
    tableName = tname;
}

void addField(const std::string &fname, BaseMasking* ms){
    fields.emplace_back(fname, tableName, ms);
}

DbField getFiled(const std::string &fname){
    for(auto f:fields){
        if(f.fieldName == fname){
            return f;
        }
    }
    //TODO:抛错 or 默认
}

std::string getTableName(){
    return tableName;
}
