// copyright 2025 Yanghaoyang

#include "GeneralizationMasking.h"

#include <string>
#include <iostream>

const std::string CONVERT_FAIL = "转换失败，数据类型错误";

GeneralizationMasking::GeneralizationMasking(int a):accuracy(a){

}
std::string GeneralizationMasking::mask(int startPosition, int length,
    const std::string &information){
    long long num = 0;
    int dotPosition = information.find(".");
    // 要去除小数点以及之后的部分
    std::string noDotInformation = "";
    if(dotPosition != std::string::npos){
        noDotInformation = information.substr(0,dotPosition);
    }
    else{
        noDotInformation = information;
    }
    try {
        num = std::stoi(noDotInformation);
    } catch (const std::exception& e) {
        std::cout << CONVERT_FAIL << e.what() << std::endl;
        return CONVERT_FAIL;
    }    
    num = (num - 1) / accuracy * accuracy;
    long long num2 = num + accuracy;
    return std::to_string(num) + "-" + std::to_string(num2);
}
