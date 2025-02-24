#include "SensitiveFieldsManager.h"
#include "BaseMasking.h"
#include "StarMasking.h"
#include "AddressMasking.h"

//记得删掉
#include <iostream>


const std::string NORMAL_CONFIGURATION = "/normal-configuration.json";
const std::string MASTER_CONFIGURATION = "/master-configuration.json";
const std::string ADDRESS_MASKING = "AddressMasking";
const std::string STAR_MASKING = "StarMasking";
const std::string NO_MASKING = "NoMasking";
const std::string MASK = "mask";
const std::string TABLES = "tables";
const std::string FIELDS = "fields";
const std::string PARAMETER = "parameter";
const std::string FIELDNAME = "fieldName";
const int NORMAL_USER_LEVEL = 0;
const int MASTER_LEVEL = 1;

SensitiveFieldsManager::SensitiveFieldsManager(){
    maskSolution.insert(std::make_pair("pphone", new StarMasking(3,4)));
}


SensitiveFieldsManager::SensitiveFieldsManager(std::string configurationPath, int level){

    std::ifstream input_file;

    if(level == NORMAL_USER_LEVEL){
        input_file.open(configurationPath + NORMAL_CONFIGURATION);
    }
    else if(level == MASTER_LEVEL){
        input_file.open(configurationPath + MASTER_CONFIGURATION);
    }
    else{
        input_file.open(configurationPath + NORMAL_CONFIGURATION);
    }
    
    // 检查文件是否成功打开
    if (!input_file.is_open()) {
        std::cerr << "Failed to open file!" << std::endl;
        return ;
    }
    
    // 解析 JSON 文件
    try {
        input_file >> configureJson;  // 将文件内容读取到 json 对象
    } catch (const std::exception& e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
        return ;
    }

    if(configureJson[0].contains(TABLES)){
        json tables = configureJson[0][TABLES];
        for(auto table : tables){
            json fields = table[FIELDS];
            for(auto field : fields){
                fieldNames.push_back(field[FIELDNAME]);
                if(field[MASK] == NO_MASKING){
                    maskSolution.insert(
                        std::make_pair(field[FIELDNAME], nullptr)
                    );
                }
                else if(field[MASK] == STAR_MASKING){
                    int parameter1 = field[PARAMETER][0];
                    int parameter2 = field[PARAMETER][1];
                    maskSolution.insert(
                        std::make_pair(field[FIELDNAME], 
                            new StarMasking(parameter1, parameter2))
                        );
                }
                else if(field[MASK] == ADDRESS_MASKING){
                    int parameter1 = field[PARAMETER][0];
                    maskSolution.insert(
                        std::make_pair(field[FIELDNAME], 
                            new AddressMasking(parameter1))
                        );
                }
            }
        }
    }
}


SensitiveFieldsManager::~SensitiveFieldsManager(){}


bool SensitiveFieldsManager::generateSensitiveResult(std::string query){
    queryFields.clear();
    hsql::SQLParserResult result;
    hsql::SQLParser::parse(query, &result);
    bool ret = result.isValid();
    if (result.isValid()) {
        for (auto i = 0u; i < result.size(); ++i) {
            const hsql::SQLStatement *stmt = result.getStatement(i);
            if (stmt->isType(hsql::kStmtSelect)) {
                const hsql::SelectStatement *select_stmt = (const hsql::SelectStatement *)stmt;
                if (select_stmt->selectList) {
                    for (auto *expr : *select_stmt->selectList) {
                        if (expr->type == hsql::kExprColumnRef) {
                            queryFields.push_back(expr->name);
                        }//if expr->type == hsql::kExprColumnRef
                        else if(expr->type == hsql::kExprStar) {

                        }
                    }//for auto *expr : *select_stmt->selectList
                }//if select_stmt->selectList
            }//if stmt->isType(hsql::kStmtSelect)
        }//  for (auto i = 0u; i < result.size(); ++i)
    }//if (result.isValid())
    return ret;
} 


bool SensitiveFieldsManager::isSensitive(int index){
    if(index > queryFields.size()) return false;
    if(index < 0) return false;
    std::string fieldname = queryFields[index];
    if(maskSolution[fieldname]!=nullptr){
        return true;
    }
    else{
        return false;
    }
}

std::string SensitiveFieldsManager::mask(int index, std::string info){
    if(index<0 || index>fieldNames.size()){
        return info;
    }
    std::string fieldname = queryFields[index];
    if(maskSolution[fieldname]!=nullptr){
        BaseMasking* mask= maskSolution[fieldname];
        return mask->mask(0, info.length(), info);
    }
    else{
        return info;
    }
}  