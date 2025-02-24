#include "DetectorManager.h"
#include "BaseDetector.h"
#include "PhoneDetector.h"
#include "BaseMasking.h"
#include "StarMasking.h"

DetectorManager::DetectorManager(){
    detectors.push_back(new PhoneDetector());
    maskSolution.insert(std::make_pair("PhoneDetector", new StarMasking(3,4)));
}

DetectorManager::~DetectorManager(){
    for(auto i:detectors){
        delete i;
    }
    for(auto i:maskSolution){
        delete i.second;
    }
}

std::string DetectorManager::detect(int level, std::string information){
    if(level == 1) return information;
    else if(level == 0){
        bool ret = false;
        for(BaseDetector* pd : detectors){
            ret = pd -> detect(information);
            if(ret){
                return maskSolution[pd -> getName()]->mask(pd->getSensitivePosition(), 
                pd->getSensitiveLength(), information);
            }
        }
    }
    return information;
}

void DetectorManager::setEnable(std::string dname, bool enable){
    for(BaseDetector* pd : detectors){
        if(pd -> getName() == dname){
            pd -> setEnable(enable);
            return;
        }
    }
}