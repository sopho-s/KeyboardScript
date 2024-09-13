#include "ArgsParse.h"

args ParseArgs(int argc, char* argv[]) {
    std::vector<std::string> argvec = GetArgVec(argc, argv);
    return InterpretArgs(argvec);
}


std::vector<std::string> GetArgVec(int argc, char* argv[]) {
    std::vector<std::string> argvec = std::vector<std::string>();
    for (int i = 1; i < argc; i++) {
        argvec.push_back(argv[i]);
    }
    return argvec;
}


args InterpretArgs(std::vector<std::string> argvec) {
    args _args;
    for (int i = 0; i < argvec.size(); i++) {
        if (argvec[i] == "--silent") {
            _args.infooff = true;
            _args.warnoff = true;
        } else if (argvec[i] == "--super-silent") {
            _args.erroff = true;
        } else if (argvec[i] == "--no-warn") {
            _args.warnoff = true;
        } else {
            _args.filename = argvec[i];
        }
    }
    return _args;
}