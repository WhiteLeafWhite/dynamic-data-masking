#pragma once
#include <string>

class BaseDetector{
protected:
    bool enable;
    std::string detectorName;
    int sensitivePosition,sensitiveLength;
public:
    BaseDetector();
    virtual ~BaseDetector();
    void setEnable(bool e);
    bool getEnable();
    int getSensitivePosition();
    int getSensitiveLength();
    virtual std::string getName();
    virtual bool detect(std::string information) = 0;
    virtual void setParameter(std::string name,std::string value) = 0;
    virtual std::string getParameter(std::string name) = 0;
};