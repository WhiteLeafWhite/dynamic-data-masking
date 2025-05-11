// copyright 2025 Yanghaoyang
#pragma once

#include "BaseMasking.h"

#include<string>

class GeneralizationMasking:
public BaseMasking
{
private:
    int accuracy;
public:
    GeneralizationMasking(int a);
    virtual std::string mask(int startPosition, int length,
        const std::string &information);
};
