// copyright 2025 Yanghaoyang

#pragma once
#include <string>
#include <random>
#include <vector>

#include "BaseMasking.h"

class StructureMasking : public BaseMasking {
 private:
  std::string infoType;
  std::vector<std::string> indexs;
  std::vector<int> maskMode;
  std::random_device rd;      // 用于生成随机种子
  std::mt19937 gen;    // 使用 Mersenne Twister 生成器
  std::string gen_aA1_(int l);
 public:
  StructureMasking();
  explicit StructureMasking(std::string itype, const std::vector<std::string>& mode);
  std::string mask(int startPosition, int length,
                   const std::string &information);
};
