#pragma once
#include "../Logging/Errors.h"
#include "../Lexer/Lexer.h"
#include "../TreeGen/TreeGen.h"
#include "../Objects/Objects.h"
#include "../Builtins/Builtins.h"
#include "../Misc/String.h"
#include <stack>
#include <utility>

namespace Engine {
    namespace Executor {


        Objects::Value EXECUTE(std::vector<Objects::Section> &sections, std::map<std::string, std::shared_ptr<Objects::Value>> &variables, std::map<std::string, Objects::Function> &functions, std::map<std::string, std::map<std::string, Objects::Function>> &classtemp);

        
        Objects::Value EVALUATE(std::vector<Objects::Section> &sections, int pointer, std::map<std::string, std::shared_ptr<Objects::Value>> &variables, std::map<std::string, Objects::Function> &functions, std::map<std::string, std::map<std::string, Objects::Function>> &classtemp);


        Objects::Value IF(std::vector<Objects::Section> &sections, int &pointer, std::map<std::string, std::shared_ptr<Objects::Value>> &variables, std::map<std::string, Objects::Function> &functions, std::map<std::string, std::map<std::string, Objects::Function>> &classtemp);


        Objects::Value WHILE(std::vector<Objects::Section> &sections, int &pointer, std::map<std::string, std::shared_ptr<Objects::Value>> &variables, std::map<std::string, Objects::Function> &functions, std::map<std::string, std::map<std::string, Objects::Function>> &classtemp);


        Objects::Value TRY(std::vector<Objects::Section> &sections, int &pointer, std::map<std::string, std::shared_ptr<Objects::Value>> &variables, std::map<std::string, Objects::Function> &functions, std::map<std::string, std::map<std::string, Objects::Function>> &classtemp);


        Objects::Value CallBasicOperation(std::stack<Objects::Token> &values, std::string operation, std::map<std::string, std::shared_ptr<Objects::Value>> &variables, std::map<std::string, Objects::Function> &functions, std::map<std::string, std::map<std::string, Objects::Function>> &classtemp);


        Objects::Value ConvertTokenToValue(Objects::Token token, std::map<std::string, std::shared_ptr<Objects::Value>> &variables);


        Objects::Token ConvertValueToToken(Objects::Value *token);


        Objects::Value EvaluateOperator(Objects::Token &_operator, std::stack<Objects::Token> &values, std::map<std::string, std::shared_ptr<Objects::Value>> &variables, std::map<std::string, Objects::Function> &functions, int &braclevel, std::map<std::string, std::map<std::string, Objects::Function>> &classtemp);


        std::shared_ptr<Objects::Value> FindValue(Objects::Token value, std::map<std::string, std::shared_ptr<Objects::Value>> &variables);


        std::pair<Objects::Function, std::shared_ptr<Objects::Value>> FindFunction(Objects::Token value, std::map<std::string, std::shared_ptr<Objects::Value>> &variables, std::map<std::string, Objects::Function> &functions);


        std::pair<Objects::Value, Objects::Function> CreateClass(Objects::Token value, std::map<std::string, std::map<std::string, Objects::Function>> &classtemp);
    }
}