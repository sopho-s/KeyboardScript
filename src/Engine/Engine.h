#pragma once
#include "Lexer/Lexer.h"
#include "../ArgsParse.h"
#include "TreeGen/TreeGen.h"
#include <string>
#include <vector>

namespace Engine {
    /**
     * @brief Analyses the file given
     * 
     * @param _args The arguments
     */
    void AnalyseFile(args _args);
}