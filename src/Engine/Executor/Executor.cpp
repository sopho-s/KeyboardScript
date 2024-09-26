#include "Executor.h"

namespace Engine {
    namespace Executor {
        Objects::Value EXECUTE(std::vector<Objects::Section> &sections, std::map<std::string, Objects::Value> &variables, std::map<std::string, Objects::Function> &functions) {
            bool skip = false;
            for (int i = 0; i < sections.size(); i++) {
                if (sections[i].tokens.size() > 0) {
                    if (!skip) {
                        Objects::Value out;
                        switch (sections[i].tokens[0].ident) {
                            case Objects::TokenType::_if:
                                out = IF(sections, i, variables, functions);
                                break;
                            case Objects::TokenType::_while:
                                out = WHILE(sections, i, variables, functions);
                                break;
                            case Objects::TokenType::_for:
                                out = WHILE(sections, i, variables, functions);
                                break;
                            default:
                                out = EVALUATE(sections, i, variables, functions);
                                break;
                        }
                        if (out.type == "Exception") {
                            return out;
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
                            if (result.isexception) {
                                values.push(ConvertValueToToken(result));
                            } else {
                                return result;
                            }
                            break;
                        }
                        case Objects::TokenType::add: {
                            Objects::Value result = CallBasicOperation(values, "ADD", variables, functions);
                            if (result.isexception) {
                                values.push(ConvertValueToToken(result));
                            } else {
                                return result;
                            }
                            break;
                        }
                        case Objects::TokenType::sub: {
                            Objects::Value result = CallBasicOperation(values, "SUB", variables, functions);
                            if (result.isexception) {
                                values.push(ConvertValueToToken(result));
                            } else {
                                return result;
                            }
                            break;
                        }
                        case Objects::TokenType::mul: {
                            Objects::Value result = CallBasicOperation(values, "MUL", variables, functions);
                            if (result.isexception) {
                                values.push(ConvertValueToToken(result));
                            } else {
                                return result;
                            }
                            break;
                        }
                        case Objects::TokenType::div: {
                            Objects::Value result = CallBasicOperation(values, "DIV", variables, functions);
                            if (result.isexception) {
                                values.push(ConvertValueToToken(result));
                            } else {
                                return result;
                            }
                            break;
                        }
                        case Objects::TokenType::equal: {
                            Objects::Value result = CallBasicOperation(values, "EQUAL", variables, functions);
                            if (result.isexception) {
                                values.push(ConvertValueToToken(result));
                            } else {
                                return result;
                            }
                            break;
                        }
                        case Objects::TokenType::greaterequal: {
                            Objects::Value result = CallBasicOperation(values, "GREATEREQUAL", variables, functions);
                            if (result.isexception) {
                                values.push(ConvertValueToToken(result));
                            } else {
                                return result;
                            }
                            break;
                        }
                        case Objects::TokenType::lesserequal: {
                            Objects::Value result = CallBasicOperation(values, "LESSEREQUAL", variables, functions);
                            if (result.isexception) {
                                values.push(ConvertValueToToken(result));
                            } else {
                                return result;
                            }
                            break;
                        }
                        case Objects::TokenType::lesser: {
                            Objects::Value result = CallBasicOperation(values, "LESSER", variables, functions);
                            if (result.isexception) {
                                values.push(ConvertValueToToken(result));
                            } else {
                                return result;
                            }
                            break;
                        }
                        case Objects::TokenType::greater: {
                            Objects::Value result = CallBasicOperation(values, "GREATER", variables, functions);
                            if (result.isexception) {
                                values.push(ConvertValueToToken(result));
                            } else {
                                return result;
                            }
                            break;
                        }
                        case Objects::TokenType::notequal: {
                            Objects::Value result = CallBasicOperation(values, "NOTEQUAL", variables, functions);
                            if (result.isexception) {
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
                    functioncall = false;
                    std::map<std::string, Objects::Value> parameters;
                    for (int i = 0; i < functions[values.top().value].parametercount; i++) {
                        Objects::Value param = Builtins::Copy(ConvertTokenToValue(parameterstack.top(), variables));
                        param.varname = functions[values.top().value].parameternames[i];
                        parameters[param.varname] = param;
                        parameterstack.pop();
                    }
                    if (functions[values.top().value].builtin) {
                        Objects::Value temp = Builtins::BuiltinCall(values.top().value, parameters);
                        values.pop();
                        values.push(ConvertValueToToken(temp));
                    } else {
                        Objects::Value temp = EXECUTE(functions[values.top().value].function, parameters, functions);
                        if (temp.type != "exception") {
                            return temp;
                        }
                        values.pop();
                        values.push(ConvertValueToToken(temp));
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
                if (pass.isexception) {
                    return pass;
                }
                if (!pass._bool) {
                    passed = false;
                    break;
                }
            }
            if (passed) {
                return EXECUTE(sections[pointer].sections, variables, functions);
            } else {
                if (sections.size() > pointer + 1) { 
                    if (sections[pointer + 1].tokens.size() > 0) {
                        if (sections[pointer + 1].tokens[0].ident == Objects::TokenType::_else) {
                            return IF(sections, ++pointer, variables, functions);
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
                    if (pass.isexception) {
                        return pass;
                    }
                    if (!pass._bool) {
                        passed = false;
                        break;
                    }
                }
                if (passed) {
                    Objects::Value result = EXECUTE(sections[pointer].sections, variables, functions);
                    if (result.isexception) {
                        return result;
                    }
                } else {
                    if (sections.size() > pointer + 1) { 
                        if (sections[pointer + 1].tokens.size() > 0) {
                            if (sections[pointer + 1].tokens[0].ident == Objects::TokenType::_else) {
                                Objects::Value result = WHILE(sections, ++pointer, variables, functions);
                                if (result.type == "exception") {
                                    return result;
                                }
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
                    } else if (operation == "NOTEQUAL") {
                        return Builtins::NOTEQUAL(params[0], params[1]);
                    } else if (operation == "SUB") {
                        return Builtins::SUB(params[0], params[1]);
                    } else if (operation == "ADD") {
                        return Builtins::ADD(params[0], params[1]);
                    } else if (operation == "DIV") {
                        return Builtins::DIV(params[0], params[1]);
                    } else if (operation == "MUL") {
                        return Builtins::MUL(params[0], params[1]);
                    } else if (operation == "GREATEREQUAL") {
                        return Builtins::GREATEREQUAL(params[0], params[1]);
                    } else if (operation == "LESSEREQUAL") {
                        return Builtins::LESSEREQUAL(params[0], params[1]);
                    } else if (operation == "GREATER") {
                        return Builtins::GREATER(params[0], params[1]);
                    } else if (operation == "LESSER") {
                        return Builtins::LESSER(params[0], params[1]);
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
                return Builtins::RaiseException("type \"" + params[0].type + "\" has no \"" + operation + "\" function", 0);
            }
        }


        Objects::Value ConvertTokenToValue(Objects::Token token, std::map<std::string, Objects::Value> &variables) {
            Objects::Value returnvalue;
            switch (token.ident) {
                case Objects::TokenType::_int:
                    returnvalue = Builtins::_int();
                    returnvalue._int = std::stoi(token.value);
                    break;
                case Objects::TokenType::_float:
                    returnvalue = Builtins::_float();
                    returnvalue._float = std::stof(token.value);
                    break;
                case Objects::TokenType::_bool:
                    returnvalue = Builtins::_bool();
                    if (token.value == "true") {
                        returnvalue._bool = true;
                    } else {
                        returnvalue._bool = false;
                    }
                    break;
                case Objects::TokenType::_string:
                    returnvalue = Builtins::_string();
                    returnvalue._string = token.value;
                    break;
                default:
                    if (variables.find(token.value) != variables.end()) {
                        returnvalue = variables[token.value];
                    } else {
                        returnvalue.varname = token.value;
                    }
            }
            Logging::Log(returnvalue.varname + " is:");
            Logging::Log(Builtins::ToString(returnvalue)._string);
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