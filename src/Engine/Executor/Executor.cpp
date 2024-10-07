#include "Executor.h"

namespace Engine {
    namespace Executor {
        Objects::Value EXECUTE(std::vector<std::shared_ptr<Objects::Section>> &sections, std::unordered_map<std::string, std::shared_ptr<Objects::Value>> &variables, std::unordered_map<std::string, std::shared_ptr<Objects::Function>> &functions, std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<Objects::Function>>> &classtemp) {
            bool skip = false;
            for (int i = 0; i < sections.size(); i++) {
                if (sections[i]->tokens.size() > 0) {
                    if (!skip) {
                        Objects::Value out;
                        switch (sections[i]->tokens[0].ident) {
                            case Objects::TokenType::_if:
                                out = IF(sections, i, variables, functions, classtemp);
                                break;
                            case Objects::TokenType::_while:
                                out = WHILE(sections, i, variables, functions, classtemp);
                                break;
                            case Objects::TokenType::_for:
                                out = WHILE(sections, i, variables, functions, classtemp);
                                break;
                            case Objects::TokenType::___try:
                                out = TRY(sections, i, variables, functions, classtemp);
                                break;
                            default:
                                out = EVALUATE(sections, i, variables, functions, classtemp);
                                break;
                        }
                        if (out.isexception || out.isreturn) {
                            return out;
                        }
                    } else {
                        skip = false;
                    }
                }
            }
            return Objects::Value();
        }

        
        Objects::Value EVALUATE(std::vector<std::shared_ptr<Objects::Section>> &sections, int pointer, std::unordered_map<std::string, std::shared_ptr<Objects::Value>> &variables, std::unordered_map<std::string, std::shared_ptr<Objects::Function>> &functions, std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<Objects::Function>>> &classtemp) {
            std::stack<Objects::Token> values = std::stack<Objects::Token>();
            std::stack<Objects::Token> parameterstack = std::stack<Objects::Token>();
            std::stack<int> braclevels = std::stack<int>();
            std::stack<int> paramcounts = std::stack<int>();
            std::stack<std::shared_ptr<Objects::Function>> funcs = std::stack<std::shared_ptr<Objects::Function>>();
            std::unordered_map<std::string, std::shared_ptr<Objects::Value>> parameters = std::unordered_map<std::string, std::shared_ptr<Objects::Value>>();
            Objects::Value lastresult;
            int braclevel = 0;
            bool isreturn = false;
            for (Objects::Token value : sections[pointer]->tokens) {
                Lexer::LogToken(value);
                if (value.ident == Objects::TokenType::_return) {
                    isreturn = true;
                }
                if (value.isoperator) {
                    Objects::Value result = EvaluateOperator(value, values, variables, functions, braclevel, classtemp);
                    if (result.isexception || result.isreturn) {
                        return result;
                    }
                }
                if (!value.isoperator && value.ident != Objects::TokenType::_return) {
                    std::pair<std::shared_ptr<Objects::Function>, std::shared_ptr<Objects::Value>> temp = FindFunction(value, variables, functions);
                    if (temp.first->exist) {
                        paramcounts.push(temp.first->parametercount);
                        funcs.push(temp.first);
                        if (temp.second->type != "none") {
                            parameters["self"] = temp.second;
                        }
                        braclevels.push(braclevel);
                    } else {
                        std::pair<Objects::Value, std::shared_ptr<Objects::Function>> temp = CreateClass(value, classtemp);
                        if (temp.second->exist) {
                            paramcounts.push(temp.second->parametercount);
                            funcs.push(temp.second);
                            if (temp.first.type != "none") {
                                parameters["self"] = std::make_shared<Objects::Value>(temp.first);
                            }
                            braclevels.push(braclevel);
                        }
                    }
                    values.push(value);
                } else if (!braclevels.empty() && braclevel == braclevels.top()) {
                    while (paramcounts.top() - parameters.size() > parameterstack.size()) {
                        parameterstack.push(values.top());
                        values.pop();
                    }
                    paramcounts.pop();
                    braclevels.pop();
                    for (int i = parameters.size(); i < funcs.top()->parametercount; i++) {
                        Objects::Value param = Builtins::Copy(ConvertTokenToValue(parameterstack.top(), variables));
                        param.varname = funcs.top()->parameternames[i];
                        parameters[param.varname] = std::make_shared<Objects::Value>(param);
                        parameterstack.pop();
                    }
                    Objects::Value temp;
                    if (funcs.top()->builtin) {
                        temp = Builtins::BuiltinCall(values.top().value, parameters);
                        temp.isreturn = false;
                    } else {
                        temp = EXECUTE(funcs.top()->function, parameters, functions, classtemp);
                        std::string tempname = "*" + Misc::Random(20);
                        temp.varname = tempname;
                        variables[tempname] = std::make_shared<Objects::Value>(temp);
                        Logging::Log(tempname);
                        Logging::Log(temp.varname);
                        temp.isreturn = false;
                    }
                    if (temp.isexception || temp.isreturn) {
                        return temp;
                    }
                    funcs.pop();
                    values.pop();
                    values.push(ConvertValueToToken(&temp));
                }
                if (!values.empty()) {
                    lastresult = ConvertTokenToValue(values.top(), variables);
                }
            }
            if (isreturn) {
                while (!values.empty()) {
                    lastresult = ConvertTokenToValue(values.top(), variables);
                    lastresult.isreturn = isreturn;
                    values.pop();
                }
            }
            return lastresult;
        }


        Objects::Value IF(std::vector<std::shared_ptr<Objects::Section>> &sections, int &pointer, std::unordered_map<std::string, std::shared_ptr<Objects::Value>> &variables, std::unordered_map<std::string, std::shared_ptr<Objects::Function>> &functions, std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<Objects::Function>>> &classtemp) {
            bool passed = true;
            for (int i = 0; i < sections[pointer]->conditions.size(); i++) {
                Objects::Value pass = EVALUATE(sections[pointer]->conditions, i, variables, functions, classtemp);
                if (pass.isexception || pass.isreturn) {
                    return pass;
                }
                if (!pass._bool) {
                    passed = false;
                    break;
                }
            }
            if (passed) {
                return EXECUTE(sections[pointer]->sections, variables, functions, classtemp);
            } else {
                if (sections.size() > pointer + 1) { 
                    if (sections[pointer + 1]->tokens.size() > 0) {
                        if (sections[pointer + 1]->tokens[0].ident == Objects::TokenType::_else) {
                            return IF(sections, ++pointer, variables, functions, classtemp);
                        }
                    }
                }
            }
            return Objects::Value();
        }


        Objects::Value WHILE(std::vector<std::shared_ptr<Objects::Section>> &sections, int &pointer, std::unordered_map<std::string, std::shared_ptr<Objects::Value>> &variables, std::unordered_map<std::string, std::shared_ptr<Objects::Function>> &functions, std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<Objects::Function>>> &classtemp) {
            bool passed = true;
            while (passed) {
                for (int i = 0; i < sections[pointer]->conditions.size(); i++) {
                    Objects::Value pass = EVALUATE(sections[pointer]->conditions, i, variables, functions, classtemp);
                    if (pass.isexception || pass.isreturn) {
                        return pass;
                    }
                    if (!pass._bool) {
                        passed = false;
                        break;
                    }
                }
                if (passed) {
                    Objects::Value result = EXECUTE(sections[pointer]->sections, variables, functions, classtemp);
                    if (result.isexception || result.isreturn) {
                        return result;
                    }
                } else {
                    if (sections.size() > pointer + 1) { 
                        if (sections[pointer + 1]->tokens.size() > 0) {
                            if (sections[pointer + 1]->tokens[0].ident == Objects::TokenType::_else) {
                                Objects::Value result = WHILE(sections, ++pointer, variables, functions, classtemp);
                                if (result.isexception || result.isreturn) {
                                    return result;
                                }
                            }
                        }
                    }
                }
            }
            return Objects::Value();
        }


        Objects::Value TRY(std::vector<std::shared_ptr<Objects::Section>> &sections, int &pointer, std::unordered_map<std::string, std::shared_ptr<Objects::Value>> &variables, std::unordered_map<std::string, std::shared_ptr<Objects::Function>> &functions, std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<Objects::Function>>> &classtemp) {
            std::shared_ptr<Objects::Value> result = std::shared_ptr<Objects::Value>();
            result = std::make_shared<Objects::Value>(EXECUTE(sections[pointer]->sections, variables, functions, classtemp));
            if (result->isexception) {
                variables["ex"] = result;
                if (sections.size() > pointer + 1) { 
                    if (sections[pointer + 1]->tokens.size() > 0) {
                        if (sections[pointer + 1]->tokens[0].ident == Objects::TokenType::_else) {
                            Objects::Value result = EXECUTE(sections[pointer + 1]->sections, variables, functions, classtemp);
                            if (result.isexception || result.isreturn) {
                                return result;
                            }
                        }
                    }
                } else {
                    return Objects::Value();
                }
                return Objects::Value();
            } else if (result->isreturn) {
                return *result;
            }
            return Objects::Value();
        }


        Objects::Value CallBasicOperation(std::stack<Objects::Token> &values, std::string operation, std::unordered_map<std::string, std::shared_ptr<Objects::Value>> &variables, std::unordered_map<std::string, std::shared_ptr<Objects::Function>> &functions, std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<Objects::Function>>> &classtemp) {
            Objects::Value params[2];
            if (operation != "NOT" && operation != "INCREMENT" && operation != "DECREMENT") {
                for (int i = 0; i < 2; i++) {
                    params[1-i] = ConvertTokenToValue(values.top(), variables);
                    values.pop();
                }
            } else {
                params[0] = ConvertTokenToValue(values.top(), variables);
                values.pop();
            }
            if (params[0]._functions.find(operation) != params[0]._functions.end()) {
                if (params[0]._functions[operation]->builtin) {
                    if (operation == "ASSIGN") {
                        return Builtins::ASSIGN(params[0], params[1], variables);
                    } else if (operation == "ADDASSIGN") {
                        return Builtins::ASSIGN(params[0], Builtins::ADD(params[0], params[1]), variables);
                    } else if (operation == "SUBASSIGN") {
                        return Builtins::ASSIGN(params[0], Builtins::SUB(params[0], params[1]), variables);
                    } else if (operation == "DIVASSIGN") {
                        return Builtins::ASSIGN(params[0], Builtins::DIV(params[0], params[1]), variables);
                    } else if (operation == "MULASSIGN") {
                        return Builtins::ASSIGN(params[0], Builtins::MUL(params[0], params[1]), variables);
                    } else if (operation == "MODASSIGN") {
                        return Builtins::ASSIGN(params[0], Builtins::MOD(params[0], params[1]), variables);
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
                    } else if (operation == "MOD") {
                        return Builtins::MOD(params[0], params[1]);
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
                    std::unordered_map<std::string, std::shared_ptr<Objects::Value>> parameters;
                    for (int i = 0; i < params[0]._functions[operation]->parametercount; i++) {
                        Objects::Value param = Builtins::Copy(params[i]);
                        param.varname = params[0]._functions[operation]->parameternames[i];
                        parameters[param.varname] = std::make_shared<Objects::Value>(param);
                    }
                    return EXECUTE(params[0]._functions[operation]->function, parameters, functions, classtemp);
                }
            } else {
                Logging::Log((int)params[0]._functions.size());
                return Builtins::RaiseException("\"" + params[0].varname + "\" of type \"" + params[0].type + "\" has no \"" + operation + "\" function", 0);
            }
        }


        Objects::Value ConvertTokenToValue(Objects::Token token, std::unordered_map<std::string, std::shared_ptr<Objects::Value>> &variables) {
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
                    if (token.value.size() == 0) {
                        return returnvalue;
                    }
                    returnvalue = *FindValue(token, variables);
                    returnvalue.isvar = true;
            }
            return returnvalue;
        }


        Objects::Token ConvertValueToToken(Objects::Value *value) {
            Objects::Token returntoken;
            Logging::Log("CONVERTING " + value->varname);
            if (value->type == "string") {
                returntoken.ident = Objects::TokenType::_string;
                returntoken.value = value->_string;
                returntoken.isoperator = false;
            } else if (value->type == "int") {
                returntoken.ident = Objects::TokenType::_int;
                returntoken.value = std::to_string(value->_int);
                returntoken.isoperator = false;
            } else if (value->type == "bool") {
                returntoken.ident = Objects::TokenType::_bool;
                if (value->_bool) {
                    returntoken.value = "true";
                } else {
                    returntoken.value = "false";
                }
                returntoken.isoperator = false;
            } else if (value->type == "float") {
                returntoken.ident = Objects::TokenType::_float;
                returntoken.value = std::to_string(value->_float);
                returntoken.isoperator = false;
            } else {
                returntoken.ident = Objects::TokenType::ident;
                returntoken.value = value->varname;
                returntoken.isoperator = false;
            }
            return returntoken;
        }


        Objects::Value EvaluateOperator(Objects::Token &_operator, std::stack<Objects::Token> &values, std::unordered_map<std::string, std::shared_ptr<Objects::Value>> &variables, std::unordered_map<std::string, std::shared_ptr<Objects::Function>> &functions, int &braclevel, std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<Objects::Function>>> &classtemp) {
            switch (_operator.ident) {
                case Objects::TokenType::assign: {
                    Objects::Value result = CallBasicOperation(values, "ASSIGN", variables, functions, classtemp);
                    if (result.isexception || result.isreturn) {
                        return result;
                    } else {
                        values.push(ConvertValueToToken(&result));
                    }
                    break;
                }
                case Objects::TokenType::addassign: {
                    Objects::Value result = CallBasicOperation(values, "ADDASSIGN", variables, functions, classtemp);
                    if (result.isexception || result.isreturn) {
                        return result;
                    } else {
                        values.push(ConvertValueToToken(&result));
                    }
                    break;
                }
                case Objects::TokenType::subassign: {
                    Objects::Value result = CallBasicOperation(values, "AUBASSIGN", variables, functions, classtemp);
                    if (result.isexception || result.isreturn) {
                        return result;
                    } else {
                        values.push(ConvertValueToToken(&result));
                    }
                    break;
                }
                case Objects::TokenType::divassign: {
                    Objects::Value result = CallBasicOperation(values, "DIVASSIGN", variables, functions, classtemp);
                    if (result.isexception || result.isreturn) {
                        return result;
                    } else {
                        values.push(ConvertValueToToken(&result));
                    }
                    break;
                }
                case Objects::TokenType::mulassign: {
                    Objects::Value result = CallBasicOperation(values, "MULASSIGN", variables, functions, classtemp);
                    if (result.isexception || result.isreturn) {
                        return result;
                    } else {
                        values.push(ConvertValueToToken(&result));
                    }
                    break;
                }
                case Objects::TokenType::modassign: {
                    Objects::Value result = CallBasicOperation(values, "MODASSIGN", variables, functions, classtemp);
                    if (result.isexception || result.isreturn) {
                        return result;
                    } else {
                        values.push(ConvertValueToToken(&result));
                    }
                    break;
                }
                case Objects::TokenType::add: {
                    Objects::Value result = CallBasicOperation(values, "ADD", variables, functions, classtemp);
                    if (result.isexception || result.isreturn) {
                        return result;
                    } else {
                        values.push(ConvertValueToToken(&result));
                    }
                    break;
                }
                case Objects::TokenType::sub: {
                    Objects::Value result = CallBasicOperation(values, "SUB", variables, functions, classtemp);
                    if (result.isexception || result.isreturn) {
                        return result;
                    } else {
                        values.push(ConvertValueToToken(&result));
                    }
                    break;
                }
                case Objects::TokenType::mul: {
                    Objects::Value result = CallBasicOperation(values, "MUL", variables, functions, classtemp);
                    if (result.isexception || result.isreturn) {
                        return result;
                    } else {
                        values.push(ConvertValueToToken(&result));
                    }
                    break;
                }
                case Objects::TokenType::div: {
                    Objects::Value result = CallBasicOperation(values, "DIV", variables, functions, classtemp);
                    if (result.isexception || result.isreturn) {
                        return result;
                    } else {
                        values.push(ConvertValueToToken(&result));
                    }
                    break;
                }
                case Objects::TokenType::mod: {
                    Objects::Value result = CallBasicOperation(values, "MOD", variables, functions, classtemp);
                    if (result.isexception || result.isreturn) {
                        return result;
                    } else {
                        values.push(ConvertValueToToken(&result));
                    }
                    break;
                }
                case Objects::TokenType::equal: {
                    Objects::Value result = CallBasicOperation(values, "EQUAL", variables, functions, classtemp);
                    if (result.isexception || result.isreturn) {
                        return result;
                    } else {
                        values.push(ConvertValueToToken(&result));
                    }
                    break;
                }
                case Objects::TokenType::greaterequal: {
                    Objects::Value result = CallBasicOperation(values, "GREATEREQUAL", variables, functions, classtemp);
                    if (result.isexception || result.isreturn) {
                        return result;
                    } else {
                        values.push(ConvertValueToToken(&result));
                    }
                    break;
                }
                case Objects::TokenType::lesserequal: {
                    Objects::Value result = CallBasicOperation(values, "LESSEREQUAL", variables, functions, classtemp);
                    if (result.isexception || result.isreturn) {
                        return result;
                    } else {
                        values.push(ConvertValueToToken(&result));
                    }
                    break;
                }
                case Objects::TokenType::lesser: {
                    Objects::Value result = CallBasicOperation(values, "LESSER", variables, functions, classtemp);
                    if (result.isexception || result.isreturn) {
                        return result;
                    } else {
                        values.push(ConvertValueToToken(&result));
                    }
                    break;
                }
                case Objects::TokenType::greater: {
                    Objects::Value result = CallBasicOperation(values, "GREATER", variables, functions, classtemp);
                    if (result.isexception || result.isreturn) {
                        return result;
                    } else {
                        values.push(ConvertValueToToken(&result));
                    }
                    break;
                }
                case Objects::TokenType::notequal: {
                    Objects::Value result = CallBasicOperation(values, "NOTEQUAL", variables, functions, classtemp);
                    if (result.isexception || result.isreturn) {
                        return result;
                    } else {
                        values.push(ConvertValueToToken(&result));
                    }
                    break;
                }
                case Objects::TokenType::openbracket: {
                    braclevel++;
                    break;
                }
                case Objects::TokenType::closebracket: {
                    braclevel--;
                    break;
                }
            }
            return Objects::Value();
        }


        std::shared_ptr<Objects::Value> FindValue(Objects::Token value, std::unordered_map<std::string, std::shared_ptr<Objects::Value>> &variables) {
            std::string origvalue = value.value;
            std::shared_ptr<Objects::Value> returnvalue;
            std::unordered_map<std::string, std::shared_ptr<Objects::Value>> currlist = variables;
            int pointloc = Misc::Contains(value.value, ".");
            if (pointloc != -1) {
                int prevpointloc = 0;
                while (pointloc != -1) {
                    std::string substr = value.value.substr(prevpointloc, pointloc-prevpointloc);
                    value.value = value.value.substr(pointloc + 1, value.value.size()-pointloc-1);
                    prevpointloc = pointloc;
                    if (currlist.find(substr) == currlist.end()) {
                        returnvalue = std::make_shared<Objects::Value>(Builtins::_none());
                        returnvalue->varname = origvalue;
                        returnvalue->isvar = true;
                        return returnvalue;
                    }
                    returnvalue = currlist[substr];
                    currlist = returnvalue->_attributes;
                    pointloc = Misc::Contains(value.value, ".");
                }
                if (currlist.find(value.value) == currlist.end()) {
                    returnvalue = std::make_shared<Objects::Value>(Builtins::_none());
                    returnvalue->varname = origvalue;
                    returnvalue->isvar = true;
                    return returnvalue;
                }
                returnvalue = currlist[value.value];
                return returnvalue;
            } else {
                if (currlist.find(origvalue) == currlist.end()) {
                    returnvalue = std::make_shared<Objects::Value>(Builtins::_none());
                    returnvalue->varname = origvalue;
                    returnvalue->isvar = true;
                    return returnvalue;
                } else {
                    returnvalue = currlist[origvalue];
                    returnvalue->isvar = true;
                    return returnvalue;
                }
            }
        }


        std::pair<std::shared_ptr<Objects::Function>, std::shared_ptr<Objects::Value>> FindFunction(Objects::Token value, std::unordered_map<std::string, std::shared_ptr<Objects::Value>> &variables, std::unordered_map<std::string, std::shared_ptr<Objects::Function>> &functions) {
            std::string origvalue = value.value;
            std::pair<std::shared_ptr<Objects::Function>, std::shared_ptr<Objects::Value>> returnpair;
            std::shared_ptr<Objects::Value> returnvalue;
            std::unordered_map<std::string, std::shared_ptr<Objects::Value>> currlist = variables;
            int pointcount = Misc::Count(value.value, ".");
            if (Misc::Count(value.value, ".") >= 1) {
                int pointloc = Misc::Contains(value.value, ".");
                int prevpointloc = 0;
                while (Misc::Count(value.value, ".") >= 1) {
                    std::string substr = value.value.substr(prevpointloc, pointloc-prevpointloc);
                    value.value = value.value.substr(pointloc + 1, value.value.size()-pointloc-1);
                    prevpointloc = pointloc;
                    if (currlist.find(substr) == currlist.end()) {
                        returnvalue = std::make_shared<Objects::Value>(Builtins::_none());
                        returnvalue->varname = origvalue;
                        returnvalue->isvar = true;
                        returnpair.second = returnvalue;
                        returnpair.first = std::make_shared<Objects::Function>(Objects::Function());
                        return returnpair;
                    }
                    returnvalue = currlist[substr];
                    currlist = returnvalue->_attributes;
                    int pointloc = Misc::Contains(value.value, ".");
                }
                if (returnvalue->_functions.find(value.value) == returnvalue->_functions.end()) {
                    returnvalue = std::make_shared<Objects::Value>(Builtins::_none());
                    returnvalue->varname = origvalue;
                    returnvalue->isvar = true;
                    returnpair.second = returnvalue;
                    returnpair.first = std::make_shared<Objects::Function>(Objects::Function());
                    return returnpair;
                }
                returnpair.first = returnvalue->_functions[value.value];
                returnpair.second = returnvalue;
                return returnpair;
            } else {
                returnvalue = std::make_shared<Objects::Value>(Builtins::_none());
                returnvalue->varname = origvalue;
                returnvalue->isvar = true;
                returnpair.second = returnvalue;
                if (functions.find(origvalue) == functions.end()) {
                    returnpair.first = std::make_shared<Objects::Function>(Objects::Function());
                    return returnpair;
                } else {
                    returnpair.first = functions[origvalue];
                    return returnpair;
                }
            }
        }


        std::pair<Objects::Value, std::shared_ptr<Objects::Function>> CreateClass(Objects::Token value, std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<Objects::Function>>> &classtemp) {
            std::pair<Objects::Value, std::shared_ptr<Objects::Function>> returnvalue;
            returnvalue.first = Builtins::_none();
            returnvalue.second = std::make_shared<Objects::Function>(Objects::Function());
            if (classtemp.find(value.value) != classtemp.end()) {
                returnvalue.first.type = value.value;
                returnvalue.first._functions = classtemp[value.value];
                returnvalue.second = returnvalue.first._functions[value.value];
            }
            return returnvalue;
        }
    }
}