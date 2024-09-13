#pragma once
#include "../Lexer/Lexer.h"

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
         * @brief Sorts each section code into postfix
         * 
         * @param section The section to be sorted
         */
        void SortSection(Section &section);
    }
}