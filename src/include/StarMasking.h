// copyright 2025 Yanghaoyang
#pragma once

#include <string>

#include "BaseMasking.h"

class StarMasking : public BaseMasking {
 private:
  int prefixLength;
  int suffixLength;
  int starNum;
  int mode;

 public:
  StarMasking();
  StarMasking(int num);
  StarMasking(int pre, int suf);
  ~StarMasking();

  std::string mask(int startPosition, int length,
                   const std::string &information);
};
