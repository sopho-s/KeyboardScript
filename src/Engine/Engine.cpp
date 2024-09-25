#include "Engine.h"

namespace Engine {
    void AnalyseFile(args _args) {
        Logging::SetArgs(_args.erroff, _args.warnoff, _args.infooff);
        std::vector<Objects::Token> tokenlist = std::vector<Objects::Token>();
        Lexer::Analysis(_args.filename, tokenlist);
        if (Logging::GetErrorCount() > 0) {
            Logging::Log(Logging::Error(-1, -1, -1, "an error occured during lexical analysis"));
            return;
        }
        Logging::Log("Lexical analysis was performed successfully");
        std::vector<Objects::Section> sections;
        int temp = 0;
        TreeGen::FindSectionsUntil(tokenlist, sections, Objects::eof, temp);
        for (Objects::Section section : sections) {
            TreeGen::LogSection(section);
        }
        if (Logging::GetErrorCount() > 0) {
            Logging::Log(Logging::Error(-1, -1, -1, "an error occured during abstract tree generation"));
            return;
        }
        Logging::Log("Abstract syntax tree generation was performed successfully");
        std::map<std::string, Objects::Function> functions = TreeGen::GetAllFunction(sections);
        functions["print"] = Objects::Function("print", "", 1, {"printv"});
        Logging::Log("Function finding was performed successfully");
        std::map<std::string, Objects::Value> values;
        Executor::EXECUTE(functions["main"].function, values, functions);
        Logging::Log("Execution was performed successfully");
    }
}