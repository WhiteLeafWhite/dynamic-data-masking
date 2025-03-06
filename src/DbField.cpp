// copyright 2025 Yanghaoyang

#include "DbField.h"

DbField::DbField(const std::string &fn, const std::string &tn, BaseMasking* ms){
    fieldName = fn;
    tableName = tn;
    maskSolution = ms;
}

DbField::~DbField(){
    delete ms;
}

std::string DbField::getFieldName(){
    return fieldName;
}

std::string DbField::getTableName(){
    return tableName;
}

BaseMasking* DbField::getMaskSolution(){
    return maskSolution;
}
