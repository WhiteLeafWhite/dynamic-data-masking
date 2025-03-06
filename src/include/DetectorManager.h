// copyright 2025 Yanghaoyang
#pragma once
#include <map>
#include <string>
#include <vector>

class BaseDetector;
class BaseMasking;

class DetectorManager {
 private:
  std::vector<BaseDetector*> detectors;
  std::map<std::string, BaseMasking*> maskSolution;

 public:
  DetectorManager();
  ~DetectorManager();
  std::string detect(int level, std::string information);
  void setEnable(std::string dname, bool enable);
  void insertSolution(std::string detectorName, BaseMasking* pm);
};
