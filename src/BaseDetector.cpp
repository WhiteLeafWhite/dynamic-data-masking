#include "BaseDetector.h"

BaseDetector::BaseDetector():enable(true),sensitivePosition(-1),sensitiveLength(-1){}

BaseDetector::~BaseDetector(){
    
}

void BaseDetector::setEnable(bool e){
    enable = e;
}
bool BaseDetector::getEnable(){
    return enable;
}

std::string BaseDetector::getName(){
    return detectorName;
}

int BaseDetector::getSensitivePosition(){
    return sensitivePosition;
}

int BaseDetector::getSensitiveLength(){
    return sensitiveLength;
}