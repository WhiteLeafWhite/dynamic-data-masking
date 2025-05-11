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

const std::string NORMAL_MODE_STR = "normal";
const std::string NAME_MODE_STR = "name";

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
}

std::string ReplaceMasking::mask(int startPosition, int length, const std::string &information){
    if(maskMode == NORMAL_MODE) return information;
    if(maskMode == NAME_MODE){
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(1, names.size());
        return names[distrib(gen)-1];
    }
    return "***";
}