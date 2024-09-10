#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <ctype.h>

namespace Engine {
    namespace Lexer {
        /**
         * @brief Stores the buffer for the file being read
         * 
         */
        class FileBuff {
            private:
                std::string buffer;
                int pointer = 0;
            public:
                int column = 0;
                int row = 0;
                int prevcolumn;
                int prevrow;
                std::string filename;
                /**
                 * @brief Construct a new File Buff object
                 * 
                 * @param filename 
                 */
                FileBuff(std::string filename);
                /**
                 * @brief Reads the next char in the buffer
                 * 
                 * @return char 
                 */
                char ReadChar();
                /**
                 * @brief Backs up a char in the buffer
                 * 
                 */
                void Backup();
        };

        enum TokenType { null, eof, illegal, ident, _int, _float, func, semi, openbracket, closebracket, cclosebracket, copenbracket, _return, add, sub, mul, div, assign, point, comma, include};
        enum TokenType idents[12] = {semi, openbracket, closebracket, cclosebracket, copenbracket, add, sub, mul, div, assign, point, comma};
        char identschar[12] = {(char)";", (char)"(", (char)")", (char)"}", (char)"{", (char)"+", (char)"-", (char)"*", (char)"/", (char)"=", (char)".", (char)","};
        std::string identsstr[12] = {";", "(", ")", "}", "{", "+", "-", "*", "/", "=", ".", ","};

        /**
         * @brief Stores a token read from the lexer
         * 
         */
        struct Token {
            int column;
            int row;
            enum TokenType ident;
            std::string value;
            /**
             * @brief Construct a new Token object
             * 
             */
            Token() {}
            /**
             * @brief Construct a new Token object
             * 
             * @param column Column of the start of the token
             * @param row Row of the start of the token
             * @param ident What type of token it is
             * @param value The original value of the token
             */
            Token(int column, int row, enum TokenType ident, std::string value) : column(column), row(row), ident(ident), value(value) {}
        };

        class Lexer {
            public:
                /**
                 * @brief Construct a new Lexer object
                 * 
                 */
                Lexer() {};
                /**
                 * @brief Performs lexical analysis on the file
                 * 
                 * @param filename The name of the file to perform the analysis on
                 * @param out A pointer to the tokens from analysis
                 */
                void Analysis(std::string filename, std::vector<Token> *out);
                /**
                 * @brief Get the Tokens in a file buffer
                 * 
                 * @param filebuff The file buffer
                 * @param out A pointer to the tokens from the file
                 */
                void GetTokens(FileBuff &filebuff, std::vector<Token> *out);
                /**
                 * @brief Get the Next Token object
                 * 
                 * @param filebuff The file buffer
                 * @return Token The next token from the file buffer
                 */
                Token GetNextToken(FileBuff &filebuff);
                /**
                 * @brief Extracts a number from the file buffer
                 * 
                 * @param filebuff The file buffer
                 * @return Token The Token
                 */
                Token LexNum(FileBuff &filebuff);
                /**
                 * @brief Extracts an identifier from the file buffer
                 * 
                 * @param filebuff The file buffer
                 * @return Token The Token
                 */
                Token LexIdent(FileBuff &filebuff);
                /**
                 * @brief Extracts a string from the file buffer
                 * 
                 * @param filebuff The file buffer
                 * @return Token The Token
                 */
                Token LexStr(FileBuff &filebuff);
        };
    }
}