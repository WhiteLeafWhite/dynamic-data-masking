#include "BaseMasking.h"

BaseMasking::BaseMasking(){
    maskName="BaseMasking";
}

BaseMasking::~BaseMasking(){}

std::string BaseMasking::mask(int startPosition, int length, std::string &information){
    return information;
}