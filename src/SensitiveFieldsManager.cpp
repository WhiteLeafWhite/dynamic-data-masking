// copyright 2025 Yanghaoyang
#include "SensitiveFieldsManager.h"

#include "AddressMasking.h"
#include "BaseMasking.h"
#include "StarMasking.h"
#include "EmailMasking.h"
#include "GeneralizationMasking.h"
#include "DbTable.h"
#include "StructureMasking.h"
#include "ReplaceMasking.h"
#include "NoiseMasking.h"

#include <iostream>
#include <string>
#include <utility>
#include <vector>

const char* NORMAL_CONFIGURATION = "/normal-configuration.json";
const char* MASTER_CONFIGURATION = "/master-configuration.json";
const char* EMAIL_MASKING = "EmailMasking";
const char* ADDRESS_MASKING = "AddressMasking";
const char* GENERALIZATION_MASKING = "GeneralizationMasking";
const char* STAR_MASKING = "StarMasking";
const char* NO_MASKING = "NoMasking";
const char* STRUCTURE_MASKING = "StructureMasking";
const char* REPLACE_MASKING = "ReplaceMasking";
const char* NOISE_MASKING = "NoiseMasking";
const char* TABLE_NAME = "tableName";
const char* MASK = "mask";
const char* TABLES = "tables";
const char* FIELDS = "fields";
const char* PARAMETER = "parameter";
const char* FIELDNAME = "fieldName";
const int NORMAL_USER_LEVEL = 0;
const int MASTER_LEVEL = 1;

SensitiveFieldsManager::SensitiveFieldsManager() {
  maskSolution.insert(std::make_pair("pphone", new StarMasking(3, 4)));
}

SensitiveFieldsManager::SensitiveFieldsManager(std::string configurationPath,
                                               int level) {
  std::ifstream input_file;

  if (level == NORMAL_USER_LEVEL) {
    input_file.open(configurationPath + NORMAL_CONFIGURATION);
  } else if (level == MASTER_LEVEL) {
    input_file.open(configurationPath + MASTER_CONFIGURATION);
  } else {
    input_file.open(configurationPath + NORMAL_CONFIGURATION);
  }

  // 检查文件是否成功打开
  if (!input_file.is_open()) {
    std::cerr << "SensitiveManager: Failed to open file!" << std::endl;
    return;
  }
  else{
    std::cout << "SensitiveManager: open file successfully!" << std::endl;
  }

  // 解析 JSON 文件
  try {
    input_file >> configureJson;  // 将文件内容读取到 json 对象
  } catch (const std::exception& e) {
    std::cerr << "Error parsing JSON: " << e.what() << std::endl;
    return;
  }

  if (configureJson[0].contains(TABLES)) {
    json tables = configureJson[0][TABLES];
    for (auto table : tables) {
      json fields = table[FIELDS];
      std::string tableName = table[TABLE_NAME];
      //DbTable temp(tableName);
      for (auto field : fields) {
        fieldNames.push_back(field[FIELDNAME]);
        std::string fieldName = field[FIELDNAME];
        if (field[MASK] == NO_MASKING) {
          maskSolution.insert(std::make_pair(field[FIELDNAME], nullptr));
          //temp.addField(fieldName, new BaseMasking());
        } else if (field[MASK] == STAR_MASKING) {
          if(field[PARAMETER].size()==2){
            int parameter1 = field[PARAMETER][0];
            int parameter2 = field[PARAMETER][1];
            maskSolution.insert(std::make_pair(
              field[FIELDNAME], new StarMasking(parameter1, parameter2)));
          }else if(field[PARAMETER].size() == 1){
            int parameter1 = field[PARAMETER][0];
            maskSolution.insert(std::make_pair(
              field[FIELDNAME], new StarMasking(parameter1)));
          }else{
            maskSolution.insert(std::make_pair(
              field[FIELDNAME], new StarMasking()));
          }
          //temp.addField(fieldName, new StarMasking(parameter1, parameter2));
        } else if (field[MASK] == ADDRESS_MASKING) {// 地址屏蔽
          int parameter1 = field[PARAMETER][0];
          maskSolution.insert(
              std::make_pair(field[FIELDNAME], new AddressMasking(parameter1)));
          //temp.addField(fieldName, new AddressMasking(parameter1));
        } else if (field[MASK] == EMAIL_MASKING){// 邮箱屏蔽
          std::string parameter = field[PARAMETER][0];
          char parameter1 = parameter[0];
          maskSolution.insert(
            std::make_pair(field[FIELDNAME], new EmailMasking(parameter1)));
          //temp.addField(fieldName, new EmailMasking(parameter1));
        } else if (field[MASK] == GENERALIZATION_MASKING){//范围模糊
          int parameter1 = field[PARAMETER][0];
          maskSolution.insert(
            std::make_pair(field[FIELDNAME], new GeneralizationMasking(parameter1))
          );
        } else if (field[MASK] == STRUCTURE_MASKING){
          std::string parameter1 = field[PARAMETER][0];
          std::vector<std::string> indexs;
          for(int i = 1;i<field[PARAMETER].size();++i){
            indexs.push_back(field[PARAMETER][i]);
          }
          maskSolution.insert(
            std::make_pair(field[FIELDNAME], new StructureMasking(parameter1, indexs))
          );
        } else if (field[MASK] == REPLACE_MASKING){
          std::string parameter1 = field[PARAMETER][0];
          maskSolution.insert(
            std::make_pair(field[FIELDNAME], new ReplaceMasking(parameter1))
          );
        }else if (field[MASK] == NOISE_MASKING){
          double parameter1 = field[PARAMETER][0];
          maskSolution.insert(
            std::make_pair(field[FIELDNAME], new NoiseMasking(parameter1))
          );
        }
        
      }
      //std::cout<<"发现星星！"<<std::endl;
      //DbTables.push_back(temp);
    }
  }
}

SensitiveFieldsManager::~SensitiveFieldsManager() {}

bool SensitiveFieldsManager::generateSensitiveResult(std::string query) {
  queryFields.clear();
  hsql::SQLParserResult result;
  hsql::SQLParser::parse(query, &result);
  bool ret = result.isValid();
  if (result.isValid()) {
    bool isStarSelect = false;
    for (auto i = 0u; i < result.size(); ++i) {
      const hsql::SQLStatement* stmt = result.getStatement(i);
      if (stmt->isType(hsql::kStmtSelect)) {
        const hsql::SelectStatement* select_stmt =
            (const hsql::SelectStatement*)stmt;
        if (select_stmt->selectList != nullptr) {
          for (auto* expr : *select_stmt->selectList) {
            if (expr->type == hsql::kExprColumnRef) {
              queryFields.push_back(expr->name);
            } else if (expr->type == hsql::kExprStar) {
              isStarSelect = true;
              std::cout<<"发现星星！"<<std::endl;
              queryFields.push_back("sid");
              queryFields.push_back("sname");
              queryFields.push_back("sphone");
              queryFields.push_back("saddress");
              queryFields.push_back("semail");
              queryFields.push_back("sps");
              queryFields.push_back("sage");
              queryFields.push_back("salary");
            } else{
            }
          }  // for auto *expr : *select_stmt->selectList
        }    // if select_stmt->selectList
        if(select_stmt -> fromTable != nullptr && isStarSelect){
          if (select_stmt->fromTable->type == hsql::kTableName) {
            std::string tableName = select_stmt->fromTable->name;
            std::cout << "查询的表名: " << tableName << std::endl;
            for(int i = 0; i < DbTables.size(); ++i){
              if(DbTables[i].getTableName() == tableName){
                std::cout<<"就是它！"<<std::endl;
              }
            }

          }
        }
      }      // if stmt->isType(hsql::kStmtSelect)
    }        //  for (auto i = 0u; i < result.size(); ++i)
  }          // if (result.isValid())
  return ret;
}

bool SensitiveFieldsManager::isSensitive(int index) {
  if (index > queryFields.size()) return false;
  if (index < 0) return false;
  std::string fieldname = queryFields[index];
  if (maskSolution[fieldname] != nullptr) {
    return true;
  } else {
    return false;
  }
}

std::string SensitiveFieldsManager::mask(int index, std::string info) {
  if (index < 0 || index > fieldNames.size()) {
    return info;
  }
  std::string fieldname = queryFields[index];
  if (maskSolution[fieldname] != nullptr) {
    BaseMasking* mask = maskSolution[fieldname];
    return mask->mask(0, info.length(), info);
  } else {
    return info;
  }
}
