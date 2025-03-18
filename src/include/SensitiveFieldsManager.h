// copyright 2025 Yanghaoyang
#pragma once

#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "./json.hpp"
#include "hsql/SQLParser.h"
#include "DbTable.h"

using json = nlohmann::json;

class BaseMasking;

class SensitiveFieldsManager {
 private:
  std::string configurationFile;
  std::map<std::string, BaseMasking*> maskSolution;
  std::vector<std::string> queryFields;
  std::vector<std::string> fieldNames;
  std::vector<DbTable> DbTables;
  json configureJson;

 public:
  SensitiveFieldsManager();
  SensitiveFieldsManager(std::string configurationPath, int level);
  ~SensitiveFieldsManager();
  bool generateSensitiveResult(std::string query);
  bool isSensitive(int index);
  std::string mask(int index, std::string info);
};
