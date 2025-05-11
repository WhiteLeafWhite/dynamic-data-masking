// copyright 2025 Yanghaoyang
#include "StarMasking.h"

#include <iostream>
#include <string>

StarMasking::StarMasking() {
  mode = 1;
  maskName = "StarMasking";
  prefixLength = -1;
  suffixLength = -1;
}

StarMasking::StarMasking(int pre, int suf) {
  mode = 2;
  maskName = "StarMasking";
  prefixLength = pre;
  suffixLength = suf;
}

StarMasking::StarMasking(int num) {
  mode = 3;
  maskName = "StarMasking";
  prefixLength = -1;
  suffixLength = -1;
  starNum = num;
}

StarMasking::~StarMasking() {}

std::string StarMasking::mask(int startPosition, int length,
                              const std::string &information) {
  std::string ret = information;
  if(mode == 1){
    // 这里没有做参数校验
    for (int i = startPosition; i <= startPosition + length - 1; ++i) {
      ret[i] = '*';
    }
    return ret;
  }
  else if(mode == 2){
    int l = ret.length();
    if (prefixLength + suffixLength >= l) {
      return ret;
    } else {
      for (int i = prefixLength; i <= l - suffixLength - 1; ++i) {
        ret[i] = '*';
      }
    }
    return ret;
  }
  else{
    ret = "";
    for(int i = 1;i <= starNum;i++){
      ret.push_back('*');
    }
    return ret;
  }
}
