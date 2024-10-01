#include "Engine.h"

namespace Engine {
    std::pair<std::map<std::string, Objects::Function>, std::map<std::string, std::map<std::string, Objects::Function>>>  AnalyseFile(args _args) {
        Logging::SetArgs(_args.erroff, _args.warnoff, _args.infooff);
        std::vector<Objects::Token> tokenlist = std::vector<Objects::Token>();
        Lexer::Analysis(_args.filename + ".kys", tokenlist);
        if (Logging::GetErrorCount() > 0) {
            Logging::Log(Logging::Error(-1, -1, -1, "an error occured during lexical analysis"));
            return std::pair<std::map<std::string, Objects::Function>, std::map<std::string, std::map<std::string, Objects::Function>>>();
        }
        Logging::Log("Lexical analysis was performed successfully");
        std::vector<std::string> imports = TreeGen::FindImport(tokenlist);
        std::vector<Objects::Section> sections;
        int temp = 0;
        TreeGen::FindSectionsUntil(tokenlist, sections, Objects::eof, temp);
        for (Objects::Section section : sections) {
            TreeGen::LogSection(section);
        }
        if (Logging::GetErrorCount() > 0) {
            Logging::Log(Logging::Error(-1, -1, -1, "an error occured during abstract tree generation"));
            return std::pair<std::map<std::string, Objects::Function>, std::map<std::string, std::map<std::string, Objects::Function>>>();
        }
        Logging::Log("Abstract syntax tree generation was performed successfully");
        std::map<std::string, Objects::Function> functions = TreeGen::GetAllFunction(sections);
        std::map<std::string, std::map<std::string, Objects::Function>> classtemps = TreeGen::GetAllClasses(sections);
        for (std::string import : imports) {
            auto filecontent = AnalyseFile(args(import, _args.erroff, _args.warnoff, _args.infooff));
            for (auto element : filecontent.first) {
                functions[element.first] = element.second;
            }
            for (auto element : filecontent.second) {
                classtemps[element.first] = element.second;
            }
        }
        Builtins::AddBuiltins(functions, classtemps);
        Logging::Log("Function finding was performed successfully");
        std::pair<std::map<std::string, Objects::Function>, std::map<std::string, std::map<std::string, Objects::Function>>> returnval(functions, classtemps);
        return returnval;
    }

    void RunCode(std::map<std::string, Objects::Function> functions, std::map<std::string, std::map<std::string, Objects::Function>> classtemps) {
        std::map<std::string, std::shared_ptr<Objects::Value>> values;
        Objects::Value out = Executor::EXECUTE(functions["main"].function, values, functions, classtemps);
        if (!out.isexception) {
            Logging::Log("Execution was performed successfully");
        } else {
            Logging::Log(Logging::Error(0, 0, out._attributes["code"]->_int, out._attributes["what"]->_string));
        }
    }
}