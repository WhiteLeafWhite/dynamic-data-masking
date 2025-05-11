// copyright 2025 Yanghaoyang
#pragma once

#include <string>
#include <fstream>
#include <random>
#include <ctime>

#include "BaseMasking.h"

class NoiseMasking:
public BaseMasking{
private:
    double range;
public:
    NoiseMasking();
    explicit NoiseMasking(double r);
    std::string mask(int startPosition, int length,
        const std::string &information);

};
