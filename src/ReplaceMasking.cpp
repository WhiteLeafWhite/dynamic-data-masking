// copyright 2025 Yanghaoyang

#include "ReplaceMasking.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <ctime>
#include <cstdlib>

const int NORMAL_MODE = 1;
const int NAME_MODE = 2;
const int NUMBER_MODE = 3;
const int Aa1_MODE = 4;

const std::string NORMAL_MODE_STR = "normal";
const std::string NAME_MODE_STR = "name";
const std::string NUMBER_MODE_STR = "number";
const std::string Aa1_STR = "Aa1";

const char* nameFile = "replace-data/Chinese_Names_Corpus（120W）.txt";

ReplaceMasking::ReplaceMasking(){
    maskMode = NORMAL_MODE;
    srand(time(0));
}

ReplaceMasking::ReplaceMasking(std::string m){
    srand(time(0));
    if(m == NORMAL_MODE_STR){
        maskMode = NORMAL_MODE;
    }
    else if(m == NAME_MODE_STR){
        maskMode = NAME_MODE;
        file.open(nameFile);
        std::string current_line;
        std::mt19937 gen(rd());
        
        while (std::getline(file, current_line)) {
            std::uniform_int_distribution<> distrib(1, 50);
            if(distrib(gen) == 1){
                names.push_back(current_line);
            }
            if(names.size()==10000) break;
        }
        file.close();
    }
    else if(m == NUMBER_MODE_STR){
        maskMode = NUMBER_MODE;
    }
    else if(m == Aa1_STR){
        maskMode = Aa1_MODE;
    }
}

std::string gen_numbers(int l){
    if(l<=0) return "";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1,10);
    std::string res = "";
    for(int i = 1; i <= l; ++ i){
        int r = dis(gen);
        res.push_back(r-1+'0');
    }
    return res;
}

std::string gen_aA1_(int l){
    if(l<=0) return "";
    std::random_device rd;
    std::mt19937 gen(rd());
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

std::string ReplaceMasking::mask(int startPosition, int length, const std::string &information){
    if(maskMode == NORMAL_MODE) return information;
    if(maskMode == NAME_MODE){
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(1, names.size());
        return names[distrib(gen)-1];
    }
    if(maskMode == NUMBER_MODE){
        return gen_numbers(information.size());
    }
    if(maskMode == Aa1_MODE){
        return gen_aA1_(information.size());
    }
    return "***";
}