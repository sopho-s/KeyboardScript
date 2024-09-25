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
    void AnalyseFile(args _args);
}