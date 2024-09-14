#pragma once
#include "../Lexer/Lexer.h"
#include "../Logging/Errors.h"
#include <vector>
#include <map>

namespace Engine {
    namespace TreeGen {
        /**
         * @brief Stores all the sections of code to be executed
         * 
         */
        struct Section {
            std::vector<Lexer::Token> tokens;
            std::vector<Section> sections;
            Section() {
                tokens = std::vector<Lexer::Token>();
                sections = std::vector<Section>();
            }
        };


        /**
         * @brief Finds the start and end bounds until a specific token and makes it a section
         * 
         * @param tokens File Tokens
         * @param out All the sections as a pointer vector
         * @param until The end token
         * @param index The start index
         */
        void FindSectionsUntil(std::vector<Lexer::Token> tokens, std::vector<Section> &out, Lexer::TokenType until, int &index);


        /**
         * @brief Sorts the tokens into prefix
         * 
         * @param tokens The tokens to be sorted
         */
        void SortTokens(std::vector<Lexer::Token> &tokens);


        /**
         * @brief Sorts each section code into postfix
         * 
         * @param section The section to be sorted
         */
        void SortSection(Section &section);


        /**
         * @brief Logs the section for debugging purposes
         * 
         * @param section The section to be debugged
         */
        void LogSection(Section &section);
    }
}