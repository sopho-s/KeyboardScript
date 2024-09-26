#pragma once
#include "../Logging/Errors.h"
#include "../Lexer/Lexer.h"
#include "../TreeGen/TreeGen.h"
#include "../Objects/Objects.h"
#include "../Builtins/Builtins.h"
#include <stack>

namespace Engine {
    namespace Executor {


        Objects::Value EXECUTE(std::vector<Objects::Section> &sections, std::map<std::string, Objects::Value> &variables, std::map<std::string, Objects::Function> &functions);

        
        Objects::Value EVALUATE(std::vector<Objects::Section> &sections, int pointer, std::map<std::string, Objects::Value> &variables, std::map<std::string, Objects::Function> &functions);


        Objects::Value IF(std::vector<Objects::Section> &sections, int &pointer, std::map<std::string, Objects::Value> &variables, std::map<std::string, Objects::Function> &functions);


        Objects::Value WHILE(std::vector<Objects::Section> &sections, int &pointer, std::map<std::string, Objects::Value> &variables, std::map<std::string, Objects::Function> &functions);


        Objects::Value TRY(std::vector<Objects::Section> &sections, int &pointer, std::map<std::string, Objects::Value> &variables, std::map<std::string, Objects::Function> &functions);


        Objects::Value CallBasicOperation(std::stack<Objects::Token> &values, std::string operation, std::map<std::string, Objects::Value> &variables, std::map<std::string, Objects::Function> &functions);


        Objects::Value ConvertTokenToValue(Objects::Token token, std::map<std::string, Objects::Value> &variables);


        Objects::Token ConvertValueToToken(Objects::Value token);


        Objects::Value EvaluateOperator(Objects::Token &_operator, std::stack<Objects::Token> &values, std::map<std::string, Objects::Value> &variables, std::map<std::string, Objects::Function> &functions, int &braclevel);
    }
}