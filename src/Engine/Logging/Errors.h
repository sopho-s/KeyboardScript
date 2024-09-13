#pragma once
#include <string>
#include <iostream>

namespace Engine {
    namespace Logging {
        /**
         * @brief Stores an error
         * 
         */
        struct Error {
            int column;
            int row;
            int code;
            std::string what;
            /**
             * @brief Construct a new Error object
             * 
             * @param column Starting column of the error, -1 if not based on a token
             * @param row Starting row of the error, -1 if not based on a token
             * @param code Error code
             * @param what String of what happened
             */
            Error(int column, int row, int code, std::string what) : column(column), row(row), code(code), what(what) {}
        };


        /**
         * @brief Stores a warning
         * 
         */
        struct Warning {
            int column;
            int row;
            int code;
            std::string what;
            /**
             * @brief Construct a new Warning object
             * 
             * @param column Starting column of the warning, -1 if not based on a token
             * @param row Starting row of the warning, -1 if not based on a token
             * @param code Warning code
             * @param what String of what happened
             */
            Warning(int column, int row, int code, std::string what) : column(column), row(row), code(code), what(what) {}
        };


        /**
         * @brief Stores a token
         * 
         */
        struct Token {
            int column;
            int row;
            std::string type;
            std::string value;
            /**
             * @brief Construct a new Token object
             * 
             * @param column Starting column of the token
             * @param row Starting row of the token
             * @param value Value of the token
             */
            Token(int column, int row, std::string type, std::string value) : column(column), row(row), type(type), value(value) {}
        };


        /**
         * @brief Logs an error
         * 
         * @param err The error
         */
        void Log(Error err);


        /**
         * @brief Logs a warning
         * 
         * @param warn The warning
         */
        void Log(Warning warn);


        /**
         * @brief Logs info
         * 
         * @param info The info
         */
        void Log(std::string info);


        /**
         * @brief Logs char
         * 
         * @param info The char
         */
        void Log(char _char);


        /**
         * @brief Logs a token
         * 
         * @param token The token
         */
        void Log(Token token);


        /**
         * @brief Logs an int
         * 
         * @param value The int
         */
        void Log(int value);


        /**
         * @brief Logs an bool
         * 
         * @param value The bool
         */
        void Log(bool value);


        /**
         * @brief Get the error count
         * 
         * @return int Error count
         */
        int GetErrorCount();


        /**
         * @brief Get the warn count
         * 
         * @return int Warning count
         */
        int GetWarnCount();


        /**
         * @brief Get the info count
         * 
         * @return int Info count
         */
        int GetInfoCount();


        /**
         * @brief Sets args
         * 
         * @param erroff Error off flag
         * @param warnoff Warning off flag
         * @param infooff Info off flag
         */
        void SetArgs(bool erroff, bool warnoff, bool infooff);
    }
}