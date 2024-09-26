#include "TreeGen.h"

namespace Engine {
    namespace TreeGen {
        std::vector<std::pair<Objects::TokenType, int>> precvec = {{Objects::greater, 0},
                                                                {Objects::lesser, 0},
                                                                {Objects::notequal, 0},
                                                                {Objects::equal, 0},
                                                                {Objects::greaterequal, 0},
                                                                {Objects::lesserequal, 0},
                                                                {Objects::assign, 0},
                                                                {Objects::addassign, 0},
                                                                {Objects::subassign, 0},
                                                                {Objects::mulassign, 0},
                                                                {Objects::divassign, 0},
                                                                {Objects::modassign, 0},
                                                                {Objects::_and, 0},
                                                                {Objects::_or, 0},
                                                                {Objects::_not, 0},
                                                                {Objects::add, 1},
                                                                {Objects::sub, 1},
                                                                {Objects::mul, 2},
                                                                {Objects::div, 2},
                                                                {Objects::mod, 2},
                                                                {Objects::comma, 3}};
        std::map<Objects::TokenType, int> precedence(precvec.begin(), precvec.end());
        std::vector<Objects::TokenType> operators = {Objects::greater, Objects::lesser, Objects::notequal, Objects::equal, Objects::greaterequal, Objects::lesserequal, Objects::assign, Objects::addassign, Objects::subassign, Objects::divassign, Objects::mulassign, Objects::modassign, Objects::_and, Objects::_or, Objects::_not, Objects::div, Objects::mul, Objects::mod, Objects::sub, Objects::add, Objects::openbracket, Objects::closebracket};
        std::vector<Objects::TokenType> values = {Objects::_int, Objects::_float, Objects::_string, Objects::ident, Objects::_class, Objects::_else, Objects::_if, Objects::func, Objects::_while};
        void FindSectionsUntil(std::vector<Objects::Token> tokens, std::vector<Objects::Section> &out, Objects::TokenType until, int &index) {
            Objects::Token pausetoken = tokens[index]; 
            while (true) {
                bool _break = false;
                Objects::Section section;
                while (index < tokens.size()) {
                    if (tokens[index].ident == Objects::semi) {
                        SortTokens(section.tokens);
                        out.push_back(section);
                        section = Objects::Section();
                    } else if (tokens[index].ident == until) {
                        _break = true;
                        SortTokens(section.tokens);
                        out.push_back(section);
                        break;
                    } else if (tokens[index].ident == Objects::TokenType::_if || tokens[index].ident == Objects::TokenType::_while || tokens[index].ident == Objects::TokenType::_for) {
                        section.tokens.push_back(tokens[index]);
                        index += 2;
                        FindSectionsUntil(tokens, section.conditions, Objects::closebracket, index);
                        for (int i = 0; i < section.conditions.size(); i++) {
                            SortTokens(section.conditions[i].tokens);
                        }
                        index += 2;
                        FindSectionsUntil(tokens, section.sections, Objects::cclosebracket, index);
                        out.push_back(section);
                        LogSection(section);
                        section = Objects::Section();
                    } else if (tokens[index].ident == Objects::TokenType::___try) {
                        section.tokens.push_back(tokens[index]);
                        index += 2;
                        FindSectionsUntil(tokens, section.sections, Objects::cclosebracket, index);
                        out.push_back(section);
                        LogSection(section);
                        section = Objects::Section();
                    } else if (tokens[index].ident == Objects::copenbracket) {
                        index++;
                        FindSectionsUntil(tokens, section.sections, Objects::cclosebracket, index);
                        SortTokens(section.tokens);
                        out.push_back(section);
                        section = Objects::Section();
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


        void SortTokens(std::vector<Objects::Token> &tokens) {
            std::vector<Objects::Token> temptokens = std::vector<Objects::Token>();
            std::stack<Objects::Token> storedoperators = std::stack<Objects::Token>();
            for (Objects::Token token : tokens) {
                bool isspec = true;
                for (Objects::TokenType ident : values) {
                    if (token.ident == ident) {
                        token.isoperator = false;
                        isspec = false;
                        temptokens.push_back(token);
                    }
                }
                if (isspec) {
                    for (Objects::TokenType _operator : operators) {
                        if (token.ident == _operator) {
                            isspec = false;
                            if (storedoperators.size() > 0) {
                                if (token.ident == Objects::TokenType::openbracket) {
                                    temptokens.push_back(token);
                                    storedoperators.push(token);
                                } else if (token.ident == Objects::TokenType::closebracket) {
                                    while (storedoperators.size() > 0 && storedoperators.top().ident != Objects::TokenType::openbracket) {
                                        temptokens.push_back(storedoperators.top());
                                        storedoperators.pop();
                                    }
                                    temptokens.push_back(token);
                                    storedoperators.pop();
                                } else if (precvec[storedoperators.top().ident] < precvec[token.ident]) {
                                    while (storedoperators.size() > 0 && storedoperators.top().ident != Objects::TokenType::openbracket) {
                                        temptokens.push_back(storedoperators.top());
                                        storedoperators.pop();
                                    }
                                    storedoperators.push(token);
                                } else {
                                    storedoperators.push(token);
                                }
                            } else {
                                if (token.ident == Objects::TokenType::openbracket) {
                                    temptokens.push_back(token);
                                }
                                storedoperators.push(token);
                            }
                        }
                    }
                }
            }
            while (storedoperators.size() > 0) {
                temptokens.push_back(storedoperators.top());
                storedoperators.pop();
            }
            Lexer::LogTokenList(temptokens);
            tokens = temptokens;
        }


        void SortSection(Objects::Section &section) {
            SortTokens(section.tokens);
            for (Objects::Section sectionsection : section.sections) {
                SortSection(sectionsection);
            }
        }


        void LogSection(Objects::Section &section) {
            Logging::Log("section:");
            Lexer::LogTokenList(section.tokens);
            for (Objects::Section sectionsection : section.sections) {
                LogSection(sectionsection);
            }
        }


        std::map<std::string, Objects::Function> GetAllFunction(std::vector<Objects::Section> &sections) {
            std::map<std::string, Objects::Function> returnvals;
            for (Objects::Section section : sections) {
                if (section.tokens.size() > 0) {
                    if (section.tokens[0].ident == Objects::func) {
                        Objects::Function newfunc;
                        newfunc.name = section.tokens[1].value;
                        newfunc.builtin = false;
                        newfunc._namespace = "main";
                        if (section.tokens.size() > 2) {
                            for (int i = 3; i < section.tokens.size() - 1; i++) {
                                if (section.tokens[i].ident != Objects::comma) {
                                    newfunc.parametercount++;
                                    newfunc.parameternames.push_back(section.tokens[i].value);
                                } else {
                                    break;
                                }
                            }
                        }
                        newfunc.function = section.sections;
                        returnvals[section.tokens[1].value] = newfunc;
                    }
                }
            }
            return returnvals;
        }
    }
}