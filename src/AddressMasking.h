#pragma once
#include "BaseMasking.h"
#include <string>
#include <vector>

class AddressMasking:
 public BaseMasking{
private:
    int maskLevel;
    
public:
    AddressMasking();
    AddressMasking(int l);
    std::string mask(int startPosition, int length, std::string &information);
};