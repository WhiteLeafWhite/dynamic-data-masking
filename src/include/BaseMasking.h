// copyright 2025 Yanghaoyang
#pragma once
#include <string>

class BaseMasking {
 protected:
  std::string maskName;

 public:
  BaseMasking();
  virtual ~BaseMasking();
  virtual std::string mask(int startPosition, int length,
                           const std::string &information);
};
