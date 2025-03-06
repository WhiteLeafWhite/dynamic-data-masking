// copyright 2025 Yanghaoyang
#include "PhoneDetector.h"

#include <iostream>
#include <stdexcept>
#include <string>

#include "BaseDetector.h"

PhoneDetector::PhoneDetector() : minLength(11) {
  detectorName = "PhoneDetector";
}

bool PhoneDetector::detect(std::string information) {
  if (information.length() < minLength) return false;
  bool res = false;
  int startPosition = 0, endPosition = 0;
  bool isnum = false;
  for (int i = 0; i < information.length(); ++i) {
    if (information[i] >= '0' && information[i] <= '9') {
      if (isnum == false) {
        startPosition = i;
        isnum = true;
      } else {
        endPosition = i;
        if (endPosition - startPosition + 1 >= minLength) {
          sensitivePosition = startPosition;
          sensitiveLength = endPosition - startPosition + 1;
          res = true;
        }
      }
    } else {
      isnum = false;
      startPosition = i;
      endPosition = i;
    }
  }
  return res;
}

void PhoneDetector::setParameter(std::string name, std::string value) {
  if (name == "minLength") {
    try {
      int minLength = std::stoi(value);
    } catch (const std::invalid_argument& e) {
      std::cerr << "转换失败：字符串中包含非数字字符。" << std::endl;
    } catch (const std::out_of_range& e) {
      std::cerr << "转换失败：数字超出了 int 类型的范围。" << std::endl;
    } catch (const std::exception& e) {
      std::cerr << "转换失败：发生了未知错误。" << std::endl;
    }
  }
}

std::string PhoneDetector::getParameter(std::string name) {
  if (name == "minLength") {
    return std::to_string(minLength);
  } else {
    return "";
  }
}
