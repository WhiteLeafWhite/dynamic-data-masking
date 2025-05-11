// copyright 2025 Yanghaoyang

#pragma once

#include <string>
#include <fstream>
#include <random>
#include <ctime>

#include "BaseMasking.h"

class ReplaceMasking:
public BaseMasking{
private:
    int maskMode;
    std::ifstream file;
    std::random_device rd;
    std::vector<std::string> names;
public:
    ReplaceMasking();
    explicit ReplaceMasking(std::string m);
    std::string mask(int startPosition, int length,
        const std::string &information);

};
