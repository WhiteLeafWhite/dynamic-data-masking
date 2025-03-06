// copyright 2025 Yanghaoyang
#pragma once

#include "BaseMasking.h"

#include<string>

class EmailMasking:
public BaseMasking
{
private:
    char delimiter;
public:
    EmailMasking(char d);
    virtual std::string mask(int startPosition, int length,
        const std::string &information);
};
