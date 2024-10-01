#pragma once
#include "Lexer/Lexer.h"
#include "../ArgsParse.h"
#include "TreeGen/TreeGen.h"
#include "Objects/Objects.h"
#include "Executor/Executor.h"
#include <string>
#include <vector>
#include <map>

namespace Engine {
    /**
     * @brief Analyses the file given
     * 
     * @param _args The arguments
     */
    std::pair<std::map<std::string, Objects::Function>, std::map<std::string, std::map<std::string, Objects::Function>>> AnalyseFile(args _args);

    
    void RunCode(std::map<std::string, Objects::Function> functions, std::map<std::string, std::map<std::string, Objects::Function>> classtemps);
}