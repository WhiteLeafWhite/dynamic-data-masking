// copyright 2025 Yanghaoyang

#include <string>
#include <vector>
#include <iostream>

#include "DbTable.h"

DbTable::DbTable(const std::string& tname){
    tableName = tname;
}

void DbTable::addField(const std::string &fname, BaseMasking* ms){
    DbField temp(fname, tableName, ms);
    fields.push_back(temp);
}

DbField DbTable::getFiled(const std::string &fname){
    DbField ret("WARNING!", "WARNING!", nullptr);
    for(auto f:fields){
        if(f.getFieldName() == fname){
            return f;
        }
    }
    //TODO:抛错 or 默认
    return ret;
}

std::string DbTable::getTableName(){
    return tableName;
}
