#include "Lexer.h"

namespace Engine {
    namespace Lexer {
        enum TokenType idents[24] = {semi, openbracket, closebracket, cclosebracket, copenbracket, add, sub, mul, div, assign, point, comma, greater, lesser, mod, _return, func, _if, _and, _or, _not, _for, _while, _class};
        char identschar[15] = {';', '(', ')', '}', '{', '+', '-', '*', '/', '=', '.', ',', '>', '<', '%'};
        std::string identsstr[24] = {";", "(", ")", "}", "{", "+", "-", "*", "/", "=", ".", ",", ">", "<", "%", "return", "func", "if", "and", "or", "not", "for", "while", "class"};
        std::string identsname[24] = {"semi", "openbracket", "closebracket", "cclosebracket", "copenbracket", "add", "sub", "mul", "div", "assign", "point", "comma", "greater than", "lesser than", "mod" "return", "func", "if", "and", "or", "not", "for", "while", "class"};
        enum TokenType combines[8][2] = {{add, assign}, {sub, assign}, {mul, assign}, {div, assign}, {mod, assign}, {assign, assign}, {greater, assign}, {lesser, assign}};
        enum TokenType combinesto[8] = {addassign, subassign, mulassign, divassign, modassign, equal, greaterequal, lesserequal};
        std::string identenum[40] = {"null", "illegal", "eof", "float", "int", "string", "greater", "lesser", "notequal", "equal", "ident", "semi", "openbracket", "closebracket", "cclosebracket", "copenbracket", "add", "sub", "mul", "div", "mod", "assign", "point", "comma", "return", "func", "if", "and", "or", "not", "for", "while", "class", "addassign", "subassign", "mulassign", "divassign", "modassign", "greaterequal", "lesserequal"};
        
        
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


        void Analysis(std::string filename, std::vector<Token> &out) {
            FileBuff file = FileBuff(filename);
            GetTokens(file, out);
            LogTokenList(out);
        }


        void GetTokens(FileBuff &filebuff, std::vector<Token> &out) {
            Token token;
            std::vector<Token> tempout = std::vector<Token>();
            do {
                token = GetNextToken(filebuff);
                tempout.push_back(token);
            } while (token.ident != eof);
            CombineTokens(tempout, out);
        }


        Token GetNextToken(FileBuff &filebuff) {
            char nextchar;
            std::string stringtoken = "";
            nextchar = filebuff.ReadChar();
            if (nextchar == '\n') {
                return GetNextToken(filebuff);
            } else if (nextchar == '\x00') {
                return Token(0, 0, eof, "");
            } else {
                for (int i = 0; i < 15; i++) {
                    if (identschar[i] == nextchar) {
                        return Token(filebuff.prevcolumn, filebuff.prevrow, idents[i], identsstr[i]);
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
            return Token(filebuff.prevcolumn, filebuff.prevrow, illegal, "ILLEGAL");
        }

        Token LexNum(FileBuff &filebuff) {
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
                        return Token(column, row, illegal, "");
                    }
                    num += nextchar;
                    haspoint = true;
                } else {
                    filebuff.Backup();
                    endnum = true;
                }
            }
            if (haspoint) {
                return Token(column, row, _float, num);
            } else {
                return Token(column, row, _int, num);
            }
        }


        Token LexStr(FileBuff &filebuff) {
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
                    return Token(column, row, _string, string);
                } else if (nextchar == '\x00') {
                    Logging::Log(Logging::Error(column, row, 2, "string is not closed"));
                    return Token(0, 0, illegal, "");
                } else {
                    string += nextchar;
                }
            }
        }


        Token LexIdent(FileBuff &filebuff) {
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
            for (int i = 15; i < 24; i++) {
                if (identsstr[i] == name) {
                    return Token(column, row, idents[i], name);
                }
            }
            return Token(column, row, ident, name);
        }


        void CombineTokens(std::vector<Token> tokens, std::vector<Token> &out) {
            bool skip = false;
            for (int i = 1; i < tokens.size(); i++) {
                if (!skip) {
                    for (int t = 0; t < 8; t++) {
                        if (tokens[i-1].ident == combines[t][0] && tokens[i].ident == combines[t][1]){
                            Token temptok = tokens[i-1];
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


        void LogTokenList(std::vector<Token> tokens) {
            for (Token token : tokens) {
                Logging::Log(Logging::Token(token.column, token.row, identenum[token.ident], token.value));
            }
        }
    }
}