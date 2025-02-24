#pragma once

#include "hsql/SQLParser.h"
#include "json.hpp"
#include <fstream>
#include <string>
#include <map>
#include <vector>

using json = nlohmann::json;

class BaseMasking;

class SensitiveFieldsManager{
private:
    std::string configurationFile;
    std::map<std::string, BaseMasking* > maskSolution;
    std::vector<std::string> queryFields;
    std::vector<std::string> fieldNames;
    json configureJson;
public:
    SensitiveFieldsManager();
    SensitiveFieldsManager(std::string configurationPath, int level);
    ~SensitiveFieldsManager();
    bool generateSensitiveResult(std::string query);
    bool isSensitive(int index);
    std::string mask(int index, std::string info);
};