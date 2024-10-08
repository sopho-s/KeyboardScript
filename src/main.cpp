#include <cstdio>
#include "Engine/Engine.h"
#include "ArgsParse.h"

int main(int argc, char* argv[]) {
    args _args = ParseArgs(argc, argv);
    auto filecontents = Engine::AnalyseFile(_args);
    Engine::RunCode(filecontents.first, filecontents.second);
}