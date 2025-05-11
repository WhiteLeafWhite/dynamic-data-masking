// copyright 2025 Yanghaoyang
#include "StructureMasking.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <random>

const std::string ID_NUMBER = "ID Number";
const std::string PHONE_NUMBER = "Phone Number";
const std::string EMAIL = "Email";
const std::string KEEP_OPERATOR = "keep operator";
const std::string KEEP_AREA = "keep area";
const std::string KEEP_PERSON = "keep person";
const std::string KEEP_USERNAME = "keep username";
const std::string KEEP_SITE = "keep site";
const int PHONE_KEEP_OPERATOR = 1;
const int PHONE_KEEP_AREA = 2;
const int PHONE_KEEP_PERSON = 3;
const int EMAIL_KEEP_USERNAME = 4;
const int EMAIL_KEEP_SITE = 5;

const int FILEMAX = 437143;
const int FILEMIN = 1;

StructureMasking::StructureMasking(std::string itype, const std::vector<std::string>& mode):
infoType(itype),
maskMode(50,0),
gen(rd()){
    for(auto s : mode){
        if (infoType == PHONE_NUMBER && s == KEEP_OPERATOR){
            maskMode[PHONE_KEEP_OPERATOR] = true;
        }
        if (infoType == PHONE_NUMBER && s == KEEP_AREA){
            maskMode[PHONE_KEEP_AREA] = true;
        }
        if (infoType == PHONE_NUMBER && s == KEEP_PERSON){
            maskMode[PHONE_KEEP_PERSON] = true;
        }
        if (infoType == EMAIL && s == KEEP_USERNAME){
            maskMode[EMAIL_KEEP_USERNAME] = true;
        }
        if (infoType == EMAIL && s == KEEP_SITE){
            maskMode[EMAIL_KEEP_SITE] = true;
        }
    }
}

std::string StructureMasking::gen_aA1_(int l){
    if(l<=0) return "";
    std::uniform_int_distribution<> dis(1,63);//一共52个字母+10个数字+下划线共63种情况
    std::string res = "";
    for(int i = 1; i <= l; ++ i){
        int r = dis(gen);
        if(r==63){//下划线
            res.push_back('_');
        }
        else if(r>52){//数字
            res.push_back(r - 53 + '0');
        }
        else if(r>=1&&r<=26){
            res.push_back(r+'A');
        }
        else{
            res.push_back(r - 26 + 'a');
        }
    }
    return res;
}

std::string StructureMasking::mask(int startPosition, int length,
                   const std::string &information){
    if(infoType == ID_NUMBER){
        return information;
    }
    else if(infoType == PHONE_NUMBER){
        std::string ret = information.substr(0,3);
        std::uniform_int_distribution<> dis(0, 99);
        for(int i = 1 ;i<=7;++i){
            ret.push_back(dis(gen) + '0');
        }
        return ret;
    }
    else if(infoType == EMAIL){
        std::string ret = information;
        size_t atpos = ret.find("@");
        std::string usernamePart = "";
        std::string sitePart = "";
        if(atpos != std::string::npos){
            usernamePart = ret.substr(0,atpos);
            sitePart = ret.substr(atpos);
        }
        else{
            return information;
        }
        if(maskMode[EMAIL_KEEP_SITE] && maskMode[EMAIL_KEEP_USERNAME]){
            return information;
        }
        else if(maskMode[EMAIL_KEEP_SITE]){
            return gen_aA1_(10)+sitePart;
        }
        else if(maskMode[EMAIL_KEEP_USERNAME]){
            return usernamePart+"@"+gen_aA1_(4)+".com";
        }
        else{
            return gen_aA1_(10)+"@"+gen_aA1_(4)+".com";
        }
    }
    return information;
}
