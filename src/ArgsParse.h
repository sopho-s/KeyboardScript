#pragma once
#include <vector>
#include <string>
#include "Engine/Logging/Errors.h"


/**
 * @brief Stores args
 * 
 */
struct args {
    std::string filename;
    bool erroff;
    bool warnoff;
    bool infooff;
    /**
     * @brief Construct a new args object
     * 
     * @param filename The name of the file to be analysed 
     * @param erroff The flag to turn off errors
     * @param warnoff The flag to turn off warnings
     * @param infooff The flag to turn off info
     */
    args(std::string filename, bool erroff, bool warnoff, bool infooff) : filename(filename), erroff(erroff), warnoff(warnoff), infooff(infooff) {}

    args() {}
};


/**
 * @brief Parses the args from the command line
 * 
 * @param argc The amount of args
 * @param argv The args
 * @return args The flags and filename set in the command line
 */
args ParseArgs(int argc, char* argv[]);

/**
 * @brief Converts the char pointer array into a vector of strings
 * 
 * @param argc The amount of args
 * @param argv The args
 * @return std::vector<std::string> The args as a vector of strings
 */
std::vector<std::string> GetArgVec(int argc, char* argv[]);


/**
 * @brief Interprets the arguments
 * 
 * @param argvec The vector with the args
 * @return args The args interpreted
 */
args InterpretArgs(std::vector<std::string> argvec);