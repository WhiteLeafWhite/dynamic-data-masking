// copyright 2025 Yanghaoyang
#include "BaseMasking.h"

#include <string>

BaseMasking::BaseMasking() { maskName = "BaseMasking"; }

BaseMasking::~BaseMasking() {}

std::string BaseMasking::mask(int startPosition, int length,
                              const std::string &information) {
  return information;
}
