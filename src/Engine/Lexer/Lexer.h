#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctype.h>
#include "../Logging/Errors.h"

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


        enum TokenType {null, illegal, eof, _float, _int, _string, greater, lesser, notequal, equal, ident, semi, openbracket, closebracket, cclosebracket, copenbracket, add, sub, mul, div, mod, assign, point, comma, _return, func, _if, _and, _or, _not, _for, _while, _class, addassign, subassign, mulassign, divassign, modassign, greaterequal, lesserequal};

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


        /**
         * @brief Performs lexical analysis on the file
         * 
         * @param filename The name of the file to perform the analysis on
         * @param out A vector to store the tokens from analysis
         */
        void Analysis(std::string filename, std::vector<Token> &out);
        /**
         * @brief Get the Tokens in a file buffer
         * 
         * @param filebuff The file buffer
         * @param out A vector to store the tokens from the file
         */
        void GetTokens(FileBuff &filebuff, std::vector<Token> &out);
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
        /**
         * @brief Combines tokens that mean different things when next to each other
         * 
         * @param out The vector of combined (or not) tokens
         */
        void CombineTokens(std::vector<Token> tokens, std::vector<Token> &out);
    }
}