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

        /**
         * @brief Executes a series of sections
         * 
         * @param sections The sections to be executed
         * @param variables The variables to be used with the sections
         * @param functions The functions to be used with the sections
         * @param classtemp The class templates to be used with the sections
         * @return Objects::Value This will be 3 things, the return value of a function, an exception, or a variable storing whether the execution was truthful
         */
        Objects::Value EXECUTE(std::vector<Objects::Section> &sections, std::map<std::string, std::shared_ptr<Objects::Value>> &variables, std::map<std::string, Objects::Function> &functions, std::map<std::string, std::map<std::string, Objects::Function>> &classtemp);


        /**
         * @brief Evaluates a series of tokens
         * 
         * @param sections The section list
         * @param pointer A pointer to the section being evaluated
         * @param variables The variables to be used with the tokens
         * @param functions The functions to be used with the tokens
         * @param classtemp The class templates to be used with the tokens
         * @return Objects::Value This will be 3 things, the return value of a function, an exception, or a variable storing whether the execution was truthful
         */
        Objects::Value EVALUATE(std::vector<Objects::Section> &sections, int pointer, std::map<std::string, std::shared_ptr<Objects::Value>> &variables, std::map<std::string, Objects::Function> &functions, std::map<std::string, std::map<std::string, Objects::Function>> &classtemp);


        /**
         * @brief Evaluates a series of sections as long as the condition is true
         * 
         * @param sections The section list
         * @param pointer A pointer to the section being evaluated
         * @param variables The variables to be used with the tokens
         * @param functions The functions to be used with the tokens
         * @param classtemp The class templates to be used with the tokens
         * @return Objects::Value This will be 3 things, the return value of a function, an exception, or a variable storing whether the execution was truthful
         */
        Objects::Value IF(std::vector<Objects::Section> &sections, int &pointer, std::map<std::string, std::shared_ptr<Objects::Value>> &variables, std::map<std::string, Objects::Function> &functions, std::map<std::string, std::map<std::string, Objects::Function>> &classtemp);


        /**
         * @brief Repeatedly evaluates a series of sections until the condition is false
         * 
         * @param sections The section list
         * @param pointer A pointer to the section being evaluated
         * @param variables The variables to be used with the tokens
         * @param functions The functions to be used with the tokens
         * @param classtemp The class templates to be used with the tokens
         * @return Objects::Value This will be 3 things, the return value of a function, an exception, or a variable storing whether the execution was truthful
         */
        Objects::Value WHILE(std::vector<Objects::Section> &sections, int &pointer, std::map<std::string, std::shared_ptr<Objects::Value>> &variables, std::map<std::string, Objects::Function> &functions, std::map<std::string, std::map<std::string, Objects::Function>> &classtemp);


        /**
         * @brief Trys evaluates a series of sections unless there is an exception
         * 
         * @param sections The section list
         * @param pointer A pointer to the section being evaluated
         * @param variables The variables to be used with the tokens
         * @param functions The functions to be used with the tokens
         * @param classtemp The class templates to be used with the tokens
         * @return Objects::Value This will be 3 things, the return value of a function, an exception, or a variable storing whether the execution was truthful
         */
        Objects::Value TRY(std::vector<Objects::Section> &sections, int &pointer, std::map<std::string, std::shared_ptr<Objects::Value>> &variables, std::map<std::string, Objects::Function> &functions, std::map<std::string, std::map<std::string, Objects::Function>> &classtemp);


        /**
         * @brief Attemps to perform the basic operations
         * 
         * @param values 
         * @param operation 
         * @param variables 
         * @param functions 
         * @param classtemp 
         * @return Objects::Value 
         */
        Objects::Value CallBasicOperation(std::stack<Objects::Token> &values, std::string operation, std::map<std::string, std::shared_ptr<Objects::Value>> &variables, std::map<std::string, Objects::Function> &functions, std::map<std::string, std::map<std::string, Objects::Function>> &classtemp);


        Objects::Value ConvertTokenToValue(Objects::Token token, std::map<std::string, std::shared_ptr<Objects::Value>> &variables);


        Objects::Token ConvertValueToToken(Objects::Value *token);


        Objects::Value EvaluateOperator(Objects::Token &_operator, std::stack<Objects::Token> &values, std::map<std::string, std::shared_ptr<Objects::Value>> &variables, std::map<std::string, Objects::Function> &functions, int &braclevel, std::map<std::string, std::map<std::string, Objects::Function>> &classtemp);


        std::shared_ptr<Objects::Value> FindValue(Objects::Token value, std::map<std::string, std::shared_ptr<Objects::Value>> &variables);


        std::pair<Objects::Function, std::shared_ptr<Objects::Value>> FindFunction(Objects::Token value, std::map<std::string, std::shared_ptr<Objects::Value>> &variables, std::map<std::string, Objects::Function> &functions);


        std::pair<Objects::Value, Objects::Function> CreateClass(Objects::Token value, std::map<std::string, std::map<std::string, Objects::Function>> &classtemp);
    }
}