// copyright 2025 Yanghaoyang
#include "AddressMasking.h"

#include <sstream>
#include <string>
#include <vector>

AddressMasking::AddressMasking() { maskLevel = 3; }

AddressMasking::AddressMasking(int l) { maskLevel = l; }

std::string AddressMasking::mask(int startPosition, int length,
                                 const std::string &information) {
  std::string address, result = "";
  if (startPosition + length > information.length()) {
    address = information;
  } else {
    address = information.substr(startPosition, length);
  }

  std::vector<std::string> parts;
  std::string part;
  size_t pos;

  pos = address.find("省");
  if (pos != std::string::npos) {
    part = address.substr(0, pos + 3);
    address = address.substr(pos + 3);
    parts.push_back(part);
  }

  pos = address.find("市");
  if (pos != std::string::npos) {
    part = address.substr(0, pos + 3);
    address = address.substr(pos + 3);
    parts.push_back(part);
  }

  pos = address.find("区");
  if (pos != std::string::npos) {
    part = address.substr(0, pos + 3);
    address = address.substr(pos + 3);
    parts.push_back(part);
  }
  while (parts.size() < 4) {
    parts.push_back("");
  }
  parts.push_back(address);

  // 使用字符串流分割省、市、区
  // 很有启发意义，还是不要删了
  //  while (std::getline(ss, part, std::string("省")[0])) {
  //      if (!part.empty() && part.size()!=) {
  //          parts.push_back(part + "省");
  //      }
  //      break;
  //  }
  //  while (std::getline(ss, part, std::string("市")[0])) {
  //      if (!part.empty()) {
  //          parts.push_back(part + "市");
  //      }
  //      break;
  //  }
  //  while (std::getline(ss, part, std::string("区")[0])) {
  //      if (!part.empty()) {
  //          parts.push_back(part + "区");
  //      }
  //      break;
  //  }

  // 根据 level 进行屏蔽
  for (size_t i = 0; i < parts.size(); ++i) {
    if (i < maskLevel) {
      result += parts[i];
    } else {
      result += "****";
    }
  }

  return result;
}
