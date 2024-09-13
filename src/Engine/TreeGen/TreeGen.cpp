#include "TreeGen.h"

namespace Engine {
    namespace TreeGen {
        void FindSectionsUntil(std::vector<Lexer::Token> tokens, std::vector<Section> &out, Lexer::TokenType until, int &index) {
            Logging::Log(index);
            Lexer::Token pausetoken = tokens[index]; 
            while (true) {
                bool _break = false;
                Section section;
                while (index < tokens.size()) {
                    if (tokens[index].ident == Lexer::semi) {
                        out.push_back(section);
                        section = Section();
                    } else if (tokens[index].ident == until) {
                        _break = true;
                        out.push_back(section);
                        break;
                    } else if (tokens[index].ident == Lexer::openbracket) {
                        FindSectionsUntil(tokens, section.sections, Lexer::closebracket, index);
                        out.push_back(section);
                        section = Section();
                    } else if (tokens[index].ident == Lexer::copenbracket) {
                        FindSectionsUntil(tokens, section.sections, Lexer::cclosebracket, index);
                        out.push_back(section);
                        section = Section();
                    } else {
                        section.tokens.push_back(tokens[index]);
                    }
                    index++;
                }
                if (_break) {
                    break;
                } else {
                    Logging::Log(Logging::Error(pausetoken.column, pausetoken.row, 3, pausetoken.value + " is not closed"));
                    return;
                }
            }
        }
    }
}