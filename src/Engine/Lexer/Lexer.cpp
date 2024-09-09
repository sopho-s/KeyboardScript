#include "Lexer.h"

namespace Engine {
    namespace Lexer {
        FileBuff::FileBuff(std::string filename) {
            this->filename = filename;
            std::ifstream File(filename);
            File >> this->buffer;
        }

        char FileBuff::ReadChar() {
            char character;
            if (this->pointer + 1 < this->buffer.length()) {
                this->prevcolumn = this->column;
                this->prevrow = this->row;
                this->pointer++;
                character = this->buffer[this->pointer];
                if (character == (char)'\n') {
                    this->row++;
                } else {
                    this->column++;
                }
            } else {
                return (char)"";
            }
            return character;
        }

        void FileBuff::Backup() {
            if (this->pointer - 1 >= 0) {
                this->pointer--;
                char character = this->buffer[this->pointer];
                if (character == (char)'\n') {
                    row--;
                } else {
                    column--;
                }
            }
        }

        void Lexer::Analysis(std::string filename, std::vector<Token> *out) {
            out = new std::vector<Token>();
            FileBuff file = FileBuff(filename);
            this->GetTokens(file, out);
        }

        void Lexer::GetTokens(FileBuff &filebuff, std::vector<Token> *out) {
            Token token;
            do {
                token = this->GetNextToken(filebuff);
                out->push_back(token);
            } while (token.ident != eof);
        }

        Token Lexer::GetNextToken(FileBuff &filebuff) {
            char nextchar;
            std::string stringtoken = "";
            nextchar = filebuff.ReadChar();
            if (nextchar == (char)"\n") {
                return this->GetNextToken(filebuff);
            } else if (nextchar == (char)"") {
                return Token(0, 0, eof, "");
            }
            return Token();
        }
    }
}