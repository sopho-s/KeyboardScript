#include "Lexer.h"

namespace Engine {
    namespace Lexer {
        enum Objects::TokenType idents[30] = {Objects::semi, Objects::openbracket, Objects::closebracket, Objects::cclosebracket, Objects::copenbracket, Objects::add, Objects::sub, Objects::mul, Objects::div, Objects::assign, Objects::point, Objects::comma, Objects::greater, Objects::lesser, Objects::mod, Objects::sopenbracket, Objects::sclosebracket, Objects::_return, Objects::func, Objects::_if, Objects::_and, Objects::_or, Objects::_not, Objects::_for, Objects::_while, Objects::_class, Objects::_else, Objects::_try, Objects::_bool, Objects::_bool};
        char identschar[17] = {';', '(', ')', '}', '{', '+', '-', '*', '/', '=', '.', ',', '>', '<', '%', '[', ']'};
        std::string identsstr[30] = {";", "(", ")", "}", "{", "+", "-", "*", "/", "=", ".", ",", ">", "<", "%", "[", "]", "return", "func", "if", "and", "or", "not", "for", "while", "class", "else", "try", "true", "false"};
        std::string identsname[30] = {"semi", "openbracket", "closebracket", "cclosebracket", "copenbracket", "add", "sub", "mul", "div", "assign", "point", "comma", "greater than", "lesser than", "mod", "square open bracket", "square close bracket", "return", "func", "if", "and", "or", "not", "for", "while", "class", "else", "try", "bool", "bool"};
        enum Objects::TokenType combines[8][2] = {{Objects::add, Objects::assign}, {Objects::sub, Objects::assign}, {Objects::mul, Objects::assign}, {Objects::div, Objects::assign}, {Objects::mod, Objects::assign}, {Objects::assign, Objects::assign}, {Objects::greater, Objects::assign}, {Objects::lesser, Objects::assign}};
        enum Objects::TokenType combinesto[8] = {Objects::addassign, Objects::subassign, Objects::mulassign, Objects::divassign, Objects::modassign, Objects::equal, Objects::greaterequal, Objects::lesserequal};
        std::string identenum[45] = {"null", "illegal", "eof", "float", "int", "string", "greater", "lesser", "notequal", "equal", "ident", "semi", "openbracket", "closebracket", "cclosebracket", "copenbracket", "add", "sub", "mul", "div", "mod", "assign", "point", "comma", "return", "func", "if", "and", "or", "not", "for", "while", "class", "addassign", "subassign", "mulassign", "divassign", "modassign", "greaterequal", "lesserequal", "squareopenbracket", "squareclosebracket", "else", "bool", "try"};
        
        
        FileBuff::FileBuff(std::string filename) {
            this->filename = filename;
            std::ifstream File(filename);
            std::stringstream tempbuffer;
            tempbuffer << File.rdbuf();
            this->buffer = tempbuffer.str();
        }


        char FileBuff::ReadChar() {
            char character;
            if (this->pointer < this->buffer.length()) {
                this->prevcolumn = this->column;
                this->prevrow = this->row;
                character = this->buffer[this->pointer];
                if (character == '\n') {
                    this->row++;
                    this->column = 0;
                } else {
                    this->column++;
                }
                this->pointer++;
            } else {
                return '\x00';
            }
            return character;
        }


        void FileBuff::Backup() {
            if (this->pointer - 1 >= 0) {
                this->pointer--;
                char character = this->buffer[this->pointer];
                if (character == '\n') {
                    row--;
                } else {
                    column--;
                }
                this->prevcolumn = column;
                this->prevrow = row;
            }
        }


        void Analysis(std::string filename, std::vector<Objects::Token> &out) {
            FileBuff file = FileBuff(filename);
            GetTokens(file, out);
            LogTokenList(out);
        }


        void GetTokens(FileBuff &filebuff, std::vector<Objects::Token> &out) {
            Objects::Token token;
            std::vector<Objects::Token> tempout = std::vector<Objects::Token>();
            do {
                token = GetNextToken(filebuff);
                tempout.push_back(token);
            } while (token.ident != Objects::eof);
            CombineTokens(tempout, out);
        }


        Objects::Token GetNextToken(FileBuff &filebuff) {
            char nextchar;
            std::string stringtoken = "";
            nextchar = filebuff.ReadChar();
            if (nextchar == '\n') {
                return GetNextToken(filebuff);
            } else if (nextchar == '\x00') {
                return Objects::Token(0, 0, Objects::eof, "");
            } else {
                for (int i = 0; i < 15; i++) {
                    if (identschar[i] == nextchar) {
                        return Objects::Token(filebuff.prevcolumn, filebuff.prevrow, idents[i], identsstr[i]);
                    }
                }
                if (isdigit(nextchar)) {
                    filebuff.Backup();
                    return LexNum(filebuff);
                } else if (nextchar == '\"' || nextchar == '\'') {
                    filebuff.Backup();
                    return LexStr(filebuff);
                } else if (nextchar != ' ' && isalpha(nextchar)) {
                    filebuff.Backup();
                    return LexIdent(filebuff);
                } else if (nextchar <= 37) {
                    return GetNextToken(filebuff);
                }
            }
            Logging::Log(Logging::Error(filebuff.prevcolumn, filebuff.prevrow, 1, "illegal character"));
            return Objects::Token(filebuff.prevcolumn, filebuff.prevrow, Objects::illegal, "ILLEGAL");
        }

        Objects::Token LexNum(FileBuff &filebuff) {
            char nextchar;
            bool endnum = false;
            std::string num = "";
            bool haspoint = false;
            int column = filebuff.column;
            int row = filebuff.row;
            while (!endnum) {
                nextchar = filebuff.ReadChar();
                if (isdigit(nextchar)) {
                    num += nextchar;
                } else if (nextchar == '.') {
                    if (haspoint) {
                        Logging::Log(Logging::Error(column, row, 0, "number contains multiple points"));
                        return Objects::Token(column, row, Objects::illegal, "");
                    }
                    num += nextchar;
                    haspoint = true;
                } else {
                    filebuff.Backup();
                    endnum = true;
                }
            }
            if (haspoint) {
                return Objects::Token(column, row, Objects::_float, num);
            } else {
                return Objects::Token(column, row, Objects::_int, num);
            }
        }


        Objects::Token LexStr(FileBuff &filebuff) {
            char nextchar = '\x00';
            std::string string = "";
            bool isescaped = false;
            char startstring = filebuff.ReadChar();
            int column = filebuff.column;
            int row = filebuff.row;
            while (true) {
                nextchar = filebuff.ReadChar();
                if (!isescaped && nextchar == '\\') {
                    isescaped = true;
                } else if (isescaped) {
                    isescaped = false;
                    if (nextchar == 'n') {
                        string += "\n";
                    } else {
                        string += nextchar;
                    }
                } else if (startstring == nextchar) {
                    return Objects::Token(column, row, Objects::_string, string);
                } else if (nextchar == '\x00') {
                    Logging::Log(Logging::Error(column, row, 2, "string is not closed"));
                    return Objects::Token(0, 0, Objects::illegal, "");
                } else {
                    string += nextchar;
                }
            }
        }


        Objects::Token LexIdent(FileBuff &filebuff) {
            char nextchar;
            bool endident = false;
            std::string name = "";
            int column = filebuff.column;
            int row = filebuff.row;
            while (!endident) {
                nextchar = filebuff.ReadChar();
                if (isalnum(nextchar) || nextchar == '_') {
                    name += nextchar;
                } else {
                    filebuff.Backup();
                    endident = true;
                }
            }
            for (int i = 15; i < 30; i++) {
                if (identsstr[i] == name) {
                    return Objects::Token(column, row, idents[i], name);
                }
            }
            return Objects::Token(column, row, Objects::ident, name);
        }


        void CombineTokens(std::vector<Objects::Token> tokens, std::vector<Objects::Token> &out) {
            bool skip = false;
            for (int i = 1; i < tokens.size(); i++) {
                if (!skip) {
                    for (int t = 0; t < 8; t++) {
                        if (tokens[i-1].ident == combines[t][0] && tokens[i].ident == combines[t][1]){
                            Objects::Token temptok = tokens[i-1];
                            temptok.ident = combinesto[t];
                            temptok.value = tokens[i-1].value + tokens[i].value;
                            out.push_back(temptok);
                            skip = true;
                            break;
                        }
                    }
                    if (!skip) {
                        out.push_back(tokens[i-1]);
                    }
                } else {
                    skip = false;
                }
            }
            out.push_back(tokens[tokens.size()-1]);
        }


        void LogTokenList(std::vector<Objects::Token> tokens) {
            for (Objects::Token token : tokens) {
                LogToken(token);
            }
        }


        void LogToken(Objects::Token token) {
            Logging::Log(Logging::Token(token.column, token.row, identenum[token.ident], token.value));
        }
    }
}