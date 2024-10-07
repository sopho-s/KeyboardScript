#pragma once
#include "Lexer/Lexer.h"
#include "../ArgsParse.h"
#include "TreeGen/TreeGen.h"
#include "Objects/Objects.h"
#include "Executor/Executor.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace Engine {
    /**
     * @brief Analyses the file given
     * 
     * @param _args The arguments
     */
    std::pair<std::unordered_map<std::string, std::shared_ptr<Objects::Function>>, std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<Objects::Function>>>> AnalyseFile(args _args);

    
    void RunCode(std::unordered_map<std::string, std::shared_ptr<Objects::Function>> functions, std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<Objects::Function>>> classtemps);
}