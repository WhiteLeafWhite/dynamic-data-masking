// copyright 2025 Yanghaoyang

#include "NoiseMasking.h"

#include <string>
#include <random>
#include <ctime>
#include <iostream>

NoiseMasking::NoiseMasking(){
    range = 0.5;
}

NoiseMasking::NoiseMasking(double r){
    if(r > 1) r = 1;
    if(r < 0) r = 0;
    range = r;
}

std::string NoiseMasking::mask(int startPosition, int length, const std::string &information){
    double data;
    try {
        data = (double)std::stoi(information);
    } catch (const std::invalid_argument& e) {
        std::cerr << "无效的参数: " << information << std::endl;
        return "1024";
    } catch (const std::out_of_range& e) {
        std::cerr << "超出范围: " << information << std::endl;
        return "114514";
    } catch (const std::exception& e) {
        std::cerr << "其他异常: " << information << std::endl;
        return "114514";
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(range * -1, range);
    double noise = dis(gen);
    noise = (1 + noise) * data;
    return std::to_string(noise);
}
