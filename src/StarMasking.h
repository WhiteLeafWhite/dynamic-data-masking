#pragma once

#include "BaseMasking.h"

class StarMasking:public BaseMasking{
private:
    int prefixLength;
    int suffixLength;
public:
    StarMasking();
    StarMasking(int pre, int suf);
    ~StarMasking();

    std::string mask(int startPosition, int length, std::string &information);

};