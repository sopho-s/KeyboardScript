#include "Executor.h"

namespace Engine {
    namespace Executor {
        Objects::Value EXECUTE(std::vector<Objects::Section> &sections, std::map<std::string, Objects::Value> &variables, std::map<std::string, Objects::Function> &functions) {
            bool skip = false;
            for (int i = 0; i < sections.size(); i++) {
                if (sections[i].tokens.size() > 0) {
                    if (!skip) {
                        switch (sections[i].tokens[0].ident) {
                            case Objects::TokenType::_if:
                                IF(sections, i, variables, functions);
                                break;
                            case Objects::TokenType::_while:
                                WHILE(sections, i, variables, functions);
                                break;
                            case Objects::TokenType::_for:
                                WHILE(sections, i, variables, functions);
                                break;
                            default:
                                EVALUATE(sections, i, variables, functions);
                                break;
                        }
                    } else {
                        skip = false;
                    }
                }
            }
            return Objects::Value();
        }

        
        Objects::Value EVALUATE(std::vector<Objects::Section> &sections, int pointer, std::map<std::string, Objects::Value> &variables, std::map<std::string, Objects::Function> &functions) {
            std::stack<Objects::Token> values = std::stack<Objects::Token>();
            std::stack<Objects::Token> parameterstack = std::stack<Objects::Token>();
            Objects::Value lastresult;
            bool functioncall = false;
            int paramleft = 0;
            for (Objects::Token value : sections[pointer].tokens) {
                Lexer::LogToken(value);
                if (paramleft == 1 && functioncall) {
                    paramleft--;
                    parameterstack.push(value);
                } else if (!value.isoperator) {
                    if (functions.find(value.value) != functions.end()) {
                        functioncall = true;
                        paramleft = functions[value.value].parametercount;
                    }
                    values.push(value);
                } else {
                    switch (value.ident) {
                        case Objects::TokenType::assign: {
                            Objects::Value result = CallBasicOperation(values, "ASSIGN", variables, functions);
                            if (result.type != "exception") {
                                values.push(ConvertValueToToken(result));
                            } else {
                                return result;
                            }
                            break;
                        }
                        case Objects::TokenType::add: {
                            Objects::Value result = CallBasicOperation(values, "ADD", variables, functions);
                            if (result.type != "exception") {
                                values.push(ConvertValueToToken(result));
                            } else {
                                return result;
                            }
                            break;
                        }
                        case Objects::TokenType::sub: {
                            Objects::Value result = CallBasicOperation(values, "SUB", variables, functions);
                            if (result.type != "exception") {
                                values.push(ConvertValueToToken(result));
                            } else {
                                return result;
                            }
                            break;
                        }
                        case Objects::TokenType::mul: {
                            Objects::Value result = CallBasicOperation(values, "MUL", variables, functions);
                            if (result.type != "exception") {
                                values.push(ConvertValueToToken(result));
                            } else {
                                return result;
                            }
                            break;
                        }
                        case Objects::TokenType::div: {
                            Objects::Value result = CallBasicOperation(values, "DIV", variables, functions);
                            if (result.type != "exception") {
                                values.push(ConvertValueToToken(result));
                            } else {
                                return result;
                            }
                            break;
                        }
                        case Objects::TokenType::equal: {
                            Objects::Value result = CallBasicOperation(values, "EQUAL", variables, functions);
                            if (result.type != "exception") {
                                values.push(ConvertValueToToken(result));
                            } else {
                                return result;
                            }
                            break;
                        }
                        case Objects::TokenType::comma: {
                            if (functioncall) {
                                paramleft--;
                                parameterstack.push(values.top());
                                values.pop();
                            }
                        }
                    }
                }
                if (paramleft == 0 && functioncall) {
                    std::map<std::string, Objects::Value> parameters;
                    for (int i = 0; i < functions[values.top().value].parametercount; i++) {
                        Objects::Value param = Builtins::Copy(ConvertTokenToValue(parameterstack.top(), variables));
                        param.varname = functions[values.top().value].parameternames[i];
                        parameters[param.varname] = param;
                        parameterstack.pop();
                    }
                    
                    if (functions[values.top().value].builtin) {
                        values.push(ConvertValueToToken(Builtins::BuiltinCall(values.top().value, parameters)));
                    } else {
                        values.push(ConvertValueToToken(EXECUTE(functions[values.top().value].function, parameters, functions)));
                    }
                }
                lastresult = ConvertTokenToValue(values.top(), variables);
            }
            return lastresult;
        }


        Objects::Value IF(std::vector<Objects::Section> &sections, int &pointer, std::map<std::string, Objects::Value> &variables, std::map<std::string, Objects::Function> &functions) {
            bool passed = true;
            for (int i = 0; i < sections[pointer].conditions.size(); i++) {
                Objects::Value pass = EVALUATE(sections[pointer].conditions, i, variables, functions);
                if (!pass._bool) {
                    passed = false;
                    break;
                }
            }
            if (passed) {
                EXECUTE(sections[pointer].sections, variables, functions);
            } else {
                if (sections.size() > pointer + 1) { 
                    if (sections[pointer + 1].tokens.size() > 0) {
                        if (sections[pointer + 1].tokens[0].ident == Objects::TokenType::_else) {
                            IF(sections, ++pointer, variables, functions);
                        }
                    }
                }
            }
            return Objects::Value();
        }


        Objects::Value WHILE(std::vector<Objects::Section> &sections, int &pointer, std::map<std::string, Objects::Value> &variables, std::map<std::string, Objects::Function> &functions) {
            bool passed = true;
            while (passed) {
                for (int i = 0; i < sections[pointer].conditions.size(); i++) {
                    Objects::Value pass = EVALUATE(sections[pointer].conditions, i, variables, functions);
                    if (!pass._bool) {
                        passed = false;
                        break;
                    }
                }
                if (passed) {
                    EXECUTE(sections[pointer].sections, variables, functions);
                } else {
                    if (sections.size() > pointer + 1) { 
                        if (sections[pointer + 1].tokens.size() > 0) {
                            if (sections[pointer + 1].tokens[0].ident == Objects::TokenType::_else) {
                                WHILE(sections, ++pointer, variables, functions);
                            }
                        }
                    }
                }
            }
            return Objects::Value();
        }


        Objects::Value CallBasicOperation(std::stack<Objects::Token> &values, std::string operation, std::map<std::string, Objects::Value> &variables, std::map<std::string, Objects::Function> &functions) {
            std::vector<Objects::Value> params;
            Objects::Value temp = ConvertTokenToValue(values.top(), variables);
            if (temp._functions.find(operation) != temp._functions.end()) {
                for (int i = 0; i < temp._functions[operation].parametercount; i++) {
                    params.insert(params.begin(), ConvertTokenToValue(values.top(), variables));
                    values.pop();
                }
                if (temp._functions[operation].builtin) {
                    if (operation == "ASSIGN") {
                        return Builtins::ASSIGN(params[0], params[1], variables);
                    } else if (operation == "EQUAL") {
                        return Builtins::EQUAL(params[0], params[1]);
                    } else if (operation == "SUB") {
                        return Builtins::SUB(params[0], params[1]);
                    } else if (operation == "ADD") {
                        return Builtins::ADD(params[0], params[1]);
                    } else if (operation == "DIV") {
                        return Builtins::DIV(params[0], params[1]);
                    } else if (operation == "MUL") {
                        return Builtins::MUL(params[0], params[1]);
                    }
                } else {
                    std::map<std::string, Objects::Value> parameters;
                    for (int i = 0; i < temp._functions[operation].parametercount; i++) {
                        Objects::Value param = Builtins::Copy(params[i]);
                        param.varname = temp._functions[operation].parameternames[i];
                        parameters[param.varname] = param;
                    }
                    return EXECUTE(temp._functions[operation].function, parameters, functions);
                }
            } else {
                return RaiseException("type \"" + params[0].type + "\" and \"" + params[1].type + "\" have no \"" + operation + "\" function", 0);
            }
        }


        Objects::Value RaiseException(std::string pwhat, int pcode) {
            Objects::Value returnvalue;
            returnvalue.type = "exception";
            Objects::Value what;
            what.type = "string";
            what.varname = "what";
            what._string = pwhat;
            returnvalue._attributes["what"] = what;
            Objects::Value code;
            code.type = "int";
            code.varname = "code";
            code._int = pcode;
            returnvalue._attributes["code"] = code;
            return returnvalue;
        }


        Objects::Value ConvertTokenToValue(Objects::Token token, std::map<std::string, Objects::Value> &variables) {
            Objects::Value returnvalue;
            switch (token.ident) {
                case Objects::TokenType::_int:
                    returnvalue._int = std::stoi(token.value);
                    returnvalue.type = "int";
                    returnvalue._functions["ADD"] = Objects::Function("ADD", "", 2);
                    returnvalue._functions["SUB"] = Objects::Function("SUB", "", 2);
                    returnvalue._functions["MUL"] = Objects::Function("MUL", "", 2);
                    returnvalue._functions["DIV"] = Objects::Function("DIV", "", 2);
                    returnvalue._functions["ASSIGN"] = Objects::Function("ASSIGN", "", 2);
                    returnvalue._functions["EQUAL"] = Objects::Function("EQUAL", "", 2);
                    break;
                case Objects::TokenType::_float:
                    returnvalue._float = std::stof(token.value);
                    returnvalue.type = "float";
                    returnvalue._functions["ADD"] = Objects::Function("ADD", "", 2);
                    returnvalue._functions["SUB"] = Objects::Function("SUB", "", 2);
                    returnvalue._functions["MUL"] = Objects::Function("MUL", "", 2);
                    returnvalue._functions["DIV"] = Objects::Function("DIV", "", 2);
                    returnvalue._functions["ASSIGN"] = Objects::Function("ASSIGN", "", 2);
                    returnvalue._functions["EQUAL"] = Objects::Function("EQUAL", "", 2);
                    break;
                case Objects::TokenType::_bool:
                    if (token.value == "true") {
                        returnvalue._bool = true;
                    } else {
                        returnvalue._bool = false;
                    }
                    returnvalue._functions["ASSIGN"] = Objects::Function("ASSIGN", "", 2);
                    returnvalue._functions["EQUAL"] = Objects::Function("EQUAL", "", 2);
                    returnvalue.type = "bool";
                    break;
                case Objects::TokenType::_string:
                    returnvalue._string = token.value;
                    returnvalue.type = "string";
                    returnvalue._functions["ASSIGN"] = Objects::Function("ASSIGN", "", 2);
                    returnvalue._functions["ADD"] = Objects::Function("ADD", "", 2);
                    returnvalue._functions["EQUAL"] = Objects::Function("EQUAL", "", 2);
                    break;
                default:
                    if (variables.find(token.value) != variables.end()) {
                        returnvalue = variables[token.value];
                    } else {
                        returnvalue.varname = token.value;
                    }
            }
            return returnvalue;
        }


        Objects::Token ConvertValueToToken(Objects::Value value) {
            Objects::Token returntoken;
            if (value.type == "string") {
                returntoken.ident = Objects::TokenType::_string;
                returntoken.value = value._string;
                returntoken.isoperator = false;
            } else if (value.type == "int") {
                returntoken.ident = Objects::TokenType::_int;
                returntoken.value = std::to_string(value._int);
                returntoken.isoperator = false;
            } else if (value.type == "bool") {
                returntoken.ident = Objects::TokenType::_bool;
                if (value._bool) {
                    returntoken.value = "true";
                } else {
                    returntoken.value = "false";
                }
                returntoken.isoperator = false;
            } else if (value.type == "float") {
                returntoken.ident = Objects::TokenType::_float;
                returntoken.value = std::to_string(value._float);
                returntoken.isoperator = false;
            }
            return returntoken;
        }
    }
}