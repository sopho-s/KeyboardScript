#include "TreeGen.h"

namespace Engine {
    namespace TreeGen {
        std::vector<std::pair<Lexer::TokenType, int>> precvec = {{Lexer::greater, 0},
                                                                {Lexer::lesser, 0},
                                                                {Lexer::notequal, 0},
                                                                {Lexer::equal, 0},
                                                                {Lexer::greaterequal, 0},
                                                                {Lexer::lesserequal, 0},
                                                                {Lexer::assign, 0},
                                                                {Lexer::addassign, 0},
                                                                {Lexer::subassign, 0},
                                                                {Lexer::mulassign, 0},
                                                                {Lexer::divassign, 0},
                                                                {Lexer::modassign, 0},
                                                                {Lexer::_and, 0},
                                                                {Lexer::_or, 0},
                                                                {Lexer::_not, 0},
                                                                {Lexer::add, 1},
                                                                {Lexer::sub, 1},
                                                                {Lexer::mul, 2},
                                                                {Lexer::div, 2},
                                                                {Lexer::mod, 2}};
        std::map<Lexer::TokenType, int> precedence(precvec.begin(), precvec.end());
        std::vector<Lexer::TokenType> operators = {Lexer::greater, Lexer::lesser, Lexer::notequal, Lexer::equal, Lexer::greaterequal, Lexer::lesserequal, Lexer::assign, Lexer::addassign, Lexer::subassign, Lexer::divassign, Lexer::mulassign, Lexer::modassign, Lexer::_and, Lexer::_or, Lexer::_not, Lexer::div, Lexer::mul, Lexer::mod, Lexer::sub, Lexer::add};
        std::vector<Lexer::TokenType> values = {Lexer::_int, Lexer::_float, Lexer::_string, Lexer::ident};
        void FindSectionsUntil(std::vector<Lexer::Token> tokens, std::vector<Section> &out, Lexer::TokenType until, int &index) {
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
                    } else if (tokens[index].ident == Lexer::copenbracket) {
                        index++;
                        FindSectionsUntil(tokens, section.sections, Lexer::cclosebracket, index);
                        out.push_back(section);
                        section = Section();
                    } else {
                        section.tokens.push_back(tokens[index]);
                    }
                    index++;
                }
                if (_break) {
                    return;
                } else {
                    Logging::Log(Logging::Error(pausetoken.column, pausetoken.row, 3, pausetoken.value + " is not closed"));
                    return;
                }
            }
        }


        void SortTokens(std::vector<Lexer::Token> &tokens) {
            std::vector<Lexer::Token> temptokens = std::vector<Lexer::Token>();
            std::vector<Lexer::Token> storedvalues = std::vector<Lexer::Token>();
            std::vector<Lexer::Token> storedoperators = std::vector<Lexer::Token>();
            for (Lexer::Token token : tokens) {
                bool isspec = true;
                for (Lexer::TokenType ident : values) {
                    if (token.ident == ident) {
                        isspec = false;
                        storedvalues.push_back(token);
                    }
                }
                if (!isspec) {
                    for (Lexer::TokenType _operator : operators) {
                        if (token.ident == _operator) {
                            isspec = false;
                            storedoperators.push_back(token);
                        }
                    }
                    if (isspec) {
                        temptokens.push_back(token);
                    }
                }
            }
        }


        void SortSection(Section &section) {
            SortTokens(section.tokens);
            for (Section sectionsection : section.sections) {
                SortSection(sectionsection);
            }
        }


        void LogSection(Section &section) {
            Logging::Log("section:");
            Lexer::LogTokenList(section.tokens);
            for (Section sectionsection : section.sections) {
                LogSection(sectionsection);
            }
        }
    }
}