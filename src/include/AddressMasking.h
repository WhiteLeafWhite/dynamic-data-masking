// copyright 2025 Yanghaoyang

#pragma once
#include <string>
#include <vector>

#include "BaseMasking.h"

class AddressMasking : public BaseMasking {
 private:
  int maskLevel;

 public:
  AddressMasking();
  explicit AddressMasking(int l);
  std::string mask(int startPosition, int length,
                   const std::string &information);
};
