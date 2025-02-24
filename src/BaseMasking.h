#pragma once
#include <string>

class BaseMasking{
protected:
    std::string maskName;
public:
    BaseMasking();
    virtual ~BaseMasking();
    virtual std::string mask(int startPosition, int length, std::string &information);
};