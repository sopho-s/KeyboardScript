#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctype.h>
#include "../Objects/Objects.h"
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


        /**
         * @brief Performs lexical analysis on the file
         * 
         * @param filename The name of the file to perform the analysis on
         * @param out A vector to store the tokens from analysis
         */
        void Analysis(std::string filename, std::vector<Objects::Token> &out);
        /**
         * @brief Get the Tokens in a file buffer
         * 
         * @param filebuff The file buffer
         * @param out A vector to store the tokens from the file
         */
        void GetTokens(FileBuff &filebuff, std::vector<Objects::Token> &out);
        /**
         * @brief Get the Next Token object
         * 
         * @param filebuff The file buffer
         * @return Token The next token from the file buffer
         */
        Objects::Token GetNextToken(FileBuff &filebuff);
        /**
         * @brief Extracts a number from the file buffer
         * 
         * @param filebuff The file buffer
         * @return Token The Token
         */
        Objects::Token LexNum(FileBuff &filebuff);
        /**
         * @brief Extracts an identifier from the file buffer
         * 
         * @param filebuff The file buffer
         * @return Token The Token
         */
        Objects::Token LexIdent(FileBuff &filebuff);
        /**
         * @brief Extracts a string from the file buffer
         * 
         * @param filebuff The file buffer
         * @return Token The Token
         */
        Objects::Token LexStr(FileBuff &filebuff);
        /**
         * @brief Combines tokens that mean different things when next to each other
         * 
         * @param out The vector of combined (or not) tokens
         */
        void CombineTokens(std::vector<Objects::Token> tokens, std::vector<Objects::Token> &out);

        
        /**
         * @brief Logs all tokens
         * 
         * @param tokens Token list
         */
        void LogTokenList(std::vector<Objects::Token> tokens);


        /**
         * @brief Logs a tokens
         * 
         * @param token A token
         */
        void LogToken(Objects::Token token);
    }
}