#pragma once
#include <string>
#include <map>
#include <vector>


namespace Engine {
    namespace Objects {
        enum TokenType {null, illegal, eof, _float, _int, _string, greater, lesser, notequal, equal, ident, semi, openbracket, closebracket, cclosebracket, copenbracket, add, sub, mul, div, mod, assign, point, comma, _return, func, _if, _and, _or, _not, _for, _while, _class, addassign, subassign, mulassign, divassign, modassign, greaterequal, lesserequal, sopenbracket, sclosebracket, _else, _bool, ___try};

        /**
         * @brief Stores a token read from the lexer
         * 
         */
        struct Token {
            int column;
            int row;
            enum TokenType ident;
            std::string value;
            bool isoperator = true;
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
         * @brief Stores all the sections of code to be executed
         * 
         */
        struct Section {
            std::vector<Token> tokens;
            std::vector<Section> sections;
            std::vector<Section> conditions;
            Section() {
                tokens = std::vector<Token>();
                sections = std::vector<Section>();
            }
        };


        struct Function {
            std::string name;
            std::string _namespace;
            std::vector<Section> function = std::vector<Section>();
            int parametercount = 0;
            std::vector<std::string> parameternames;
            bool builtin = false;
            bool exist = false;

            Function() {;}
            

            Function(std::string name, std::string _namespace, int parametercount) {
                this->builtin = true;
                this->name = name;
                this->_namespace = _namespace;
                this->parametercount = parametercount;
            }


            Function(std::string name, std::string _namespace, int parametercount, std::vector<std::string> parameternames) {
                this->builtin = true;
                this->name = name;
                this->_namespace = _namespace;
                this->parametercount = parametercount;
                this->parameternames = parameternames;
            }
        };


        struct Value {
            bool isvar = false;
            bool isexception = false;
            std::string varname = "";
            std::string type = "null";
            int _int = 0;
            float _float = 0;
            bool _bool = true;
            std::string _string = "";
            std::vector<Value> _list;
            std::map<std::string, Value*> _attributes;
            std::map<std::string, Function> _functions;
        };
    }
}