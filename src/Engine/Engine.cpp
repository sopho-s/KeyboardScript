#include "Engine.h"

namespace Engine {
    void AnalyseFile(args _args) {
        Logging::SetArgs(_args.erroff, _args.warnoff, _args.infooff);
        std::vector<Lexer::Token> tokenlist = std::vector<Lexer::Token>();
        Lexer::Analysis(_args.filename, tokenlist);
        if (Logging::GetErrorCount() > 0) {
            Logging::Log(Logging::Error(-1, -1, -1, "an error occured during lexical analysis"));
            return;
        }
        Logging::Log("Lexical analysis was performed successfully");
        std::vector<TreeGen::Section> sections;
        int temp = 0;
        TreeGen::FindSectionsUntil(tokenlist, sections, Lexer::eof, temp);
        for (TreeGen::Section section : sections) {
            TreeGen::LogSection(section);
        }
        if (Logging::GetErrorCount() > 0) {
            Logging::Log(Logging::Error(-1, -1, -1, "an error occured during abstract tree generation"));
            return;
        }
        Logging::Log("Abstract syntax tree generation was performed successfully");
    }
}