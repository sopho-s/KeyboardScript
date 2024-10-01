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
        functions["raise"] = Objects::Function("raise", "", 1, {"what"});
        functions["typeof"] = Objects::Function("typeof", "", 1, {"var"});
        functions["input"] = Objects::Function("input", "", 0);
        functions["toString"] = Objects::Function("toString", "", 1, {"var"});
        Logging::Log("Function finding was performed successfully");
        std::map<std::string, std::shared_ptr<Objects::Value>> values;
        Objects::Value out = Executor::EXECUTE(functions["main"].function, values, functions);
        if (!out.isexception) {
            Logging::Log("Execution was performed successfully");
        } else {
            Logging::Log(Logging::Error(0, 0, out._attributes["code"]->_int, out._attributes["what"]->_string));
        }
    }
}