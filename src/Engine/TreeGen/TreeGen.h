#pragma once
#include "../Lexer/Lexer.h"
#include "../Logging/Errors.h"
#include "../Objects/Objects.h"
#include <vector>
#include <unordered_map>
#include <stack>

namespace Engine {
    namespace TreeGen {
        std::vector<std::string> FindImport(std::vector<Objects::Token> &tokens);

        
        /**
         * @brief Finds the start and end bounds until a specific token and makes it a section
         * 
         * @param tokens File Tokens
         * @param out All the sections as a pointer vector
         * @param until The end token
         * @param index The start index
         */
        void FindSectionsUntil(std::vector<Objects::Token> tokens, std::vector<std::shared_ptr<Objects::Section>> &out, Objects::TokenType until, int &index);


        /**
         * @brief Sorts the tokens into prefix
         * 
         * @param tokens The tokens to be sorted
         */
        void SortTokens(std::vector<Objects::Token> &tokens);


        /**
         * @brief Sorts each section code into postfix
         * 
         * @param section The section to be sorted
         */
        void SortSection(std::shared_ptr<Objects::Section> &section);


        /**
         * @brief Logs the section for debugging purposes
         * 
         * @param section The section to be debugged
         */
        void LogSection(std::shared_ptr<Objects::Section> &section);


        std::unordered_map<std::string, std::shared_ptr<Objects::Function>> GetAllFunction(std::vector<std::shared_ptr<Objects::Section>> &sections);


        std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<Objects::Function>>> GetAllClasses(std::vector<std::shared_ptr<Objects::Section>> &sections);
    }
}