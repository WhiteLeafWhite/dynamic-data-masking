// copyright 2025 Yanghaoyang

#include "DbField.h"
#include "BaseMasking.h"

#include <iostream>

DbField::DbField():maskSolution(nullptr){}

DbField::DbField(const std::string &fn, const std::string &tn, BaseMasking* ms){
    fieldName = fn;
    tableName = tn;
    maskSolution = ms;
}

DbField::DbField(const DbField& other){
    this->fieldName = other.fieldName;
    this->tableName = other.tableName;
    this->maskSolution = other.maskSolution;
}

DbField::~DbField(){
    delete maskSolution;
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
