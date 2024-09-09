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
                this->pointer++;
                character = this->buffer[this->pointer];
                if (character == (char)'\n') {
                    row++;
                } else {
                    column++;
                }
            } else {
                return NULL;
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

        
    }
}