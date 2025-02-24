#pragma once
#include <string>
#include "BaseDetector.h"

class PhoneDetector:
    public BaseDetector{
private:
    int minLength;
public:
    PhoneDetector();
    bool detect(std::string information) override;
    void setParameter(std::string name,std::string value) override;
    std::string getParameter(std::string name) override;
};