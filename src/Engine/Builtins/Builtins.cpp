#include "Builtins.h"

namespace Engine {
    namespace Builtins {
        Objects::Value ToString(Objects::Value value) {
            if (value.type == "string") {
                return value;
            } else if (value.type == "int") {
                value._string = std::to_string((float)value._int);
                value._int = 0;
                value.type = "string";
                return value;
            } else if (value.type == "bool") {
                value._string = (value._bool) ? "true" : "false";
                value._bool = true;
                value.type = "string";
                return value;
            } else if (value.type == "float") {
                value._string = std::to_string(value._float);
                value._float = 0;
                value.type = "string";
                return value;
            }
            return value;
        }


        Objects::Value ADD(Objects::Value value1, Objects::Value value2) {
            Objects::Value returnvalue;
            if (value1.type == "string" || value2.type == "string") {
                returnvalue._string = Builtins::ToString(value1)._string + Builtins::ToString(value2)._string;
                returnvalue.type = "string";
                returnvalue.isvar = false;
            } else if (value1.type == "int" && value2.type == "int") {
                returnvalue._int = value1._int + value2._int;
                returnvalue.type = "int";
                returnvalue.isvar = false;
            } else if ((value1.type == "float" || value1.type == "int") && (value2.type == "float" || value2.type == "int")) {
                returnvalue._float = (float)value1._int + (float)value2._int + value1._float + value2._float;
                returnvalue.type = "float";
                returnvalue.isvar = false;
            }
            return returnvalue;
        }


        Objects::Value SUB(Objects::Value value1, Objects::Value value2) {
            Objects::Value returnvalue;
            if (value1.type == "int" && value2.type == "int") {
                returnvalue._int = value1._int - value2._int;
                returnvalue.type = "int";
                returnvalue.isvar = false;
            } else if ((value1.type == "float" || value1.type == "int") && (value2.type == "float" || value2.type == "int")) {
                returnvalue._float = - (float)value2._int + (float)value1._int - value2._float + value1._float;
                returnvalue.type = "float";
                returnvalue.isvar = false;
            }
            return returnvalue;
        }


        Objects::Value DIV(Objects::Value value1, Objects::Value value2) {
            Objects::Value returnvalue;
            if ((value1.type == "float" || value1.type == "int") && (value2.type == "float" || value2.type == "int")) {
                returnvalue._float = ((float)value1._int + value1._float) / ((float)value2._int + value2._float);
                returnvalue.type = "float";
                returnvalue.isvar = false;
            }
            return returnvalue;
        }


        Objects::Value MUL(Objects::Value value1, Objects::Value value2) {
            Objects::Value returnvalue;
            if (value1.type == "int" && value2.type == "int") {
                returnvalue._int = value2._int * value1._int;
                returnvalue.type = "int";
                returnvalue.isvar = false;
            } else if ((value1.type == "float" || value1.type == "int") && (value2.type == "float" || value2.type == "int")) {
                returnvalue._float = ((float)value2._int + value2._float) * ((float)value1._int + value1._float);
                returnvalue.type = "float";
                returnvalue.isvar = false;
            }
            return returnvalue;
        }


        Objects::Value EQUAL(Objects::Value value1, Objects::Value value2) {
            Objects::Value temp;
            temp.type = "bool";
            temp._bool = ToString(value1)._string == ToString(value2)._string;
            return temp;
        }


        Objects::Value NOTEQUAL(Objects::Value value1, Objects::Value value2) {
            Objects::Value temp;
            temp.type = "bool";
            temp._bool = ToString(value1)._string != ToString(value2)._string;
            return temp;
        }


        Objects::Value GREATEREQUAL(Objects::Value value1, Objects::Value value2) {
            Objects::Value temp;
            temp.type = "bool";
            if (value1.type == "int" && value2.type == "int") {
                temp._bool = value1._int >= value2._int;
            } else if (value1.type == "float" && value2.type == "int") {
                temp._bool = value1._float >= value2._int;
            } else if (value1.type == "float" && value2.type == "float") {
                temp._bool = value1._float >= value2._float;
            } else if (value1.type == "int" && value2.type == "int") {
                temp._bool = value1._int >= value2._float;
            }
            return temp;
        }


        Objects::Value LESSEREQUAL(Objects::Value value1, Objects::Value value2) {
            Objects::Value temp;
            temp.type = "bool";
            if (value1.type == "int" && value2.type == "int") {
                temp._bool = value1._int <= value2._int;
            } else if (value1.type == "float" && value2.type == "int") {
                temp._bool = value1._float <= value2._int;
            } else if (value1.type == "float" && value2.type == "float") {
                temp._bool = value1._float <= value2._float;
            } else if (value1.type == "int" && value2.type == "int") {
                temp._bool = value1._int <= value2._float;
            }
            return temp;
        }


        Objects::Value LESSER(Objects::Value value1, Objects::Value value2) {
            Objects::Value temp;
            temp.type = "bool";
            if (value1.type == "int" && value2.type == "int") {
                temp._bool = value1._int < value2._int;
            } else if (value1.type == "float" && value2.type == "int") {
                temp._bool = value1._float < value2._int;
            } else if (value1.type == "float" && value2.type == "float") {
                temp._bool = value1._float < value2._float;
            } else if (value1.type == "int" && value2.type == "int") {
                temp._bool = value1._int < value2._float;
            }
            return temp;
        }


        Objects::Value GREATER(Objects::Value value1, Objects::Value value2) {
            Objects::Value temp;
            temp.type = "bool";
            if (value1.type == "int" && value2.type == "int") {
                temp._bool = value1._int > value2._int;
            } else if (value1.type == "float" && value2.type == "int") {
                temp._bool = value1._float > value2._int;
            } else if (value1.type == "float" && value2.type == "float") {
                temp._bool = value1._float > value2._float;
            } else if (value1.type == "int" && value2.type == "int") {
                temp._bool = value1._int > value2._float;
            } else {
                return RaiseException("type \"" + value1.type + "\" may not be compared with \"" + value2.type + "\"", 2);
            }
            return temp;
        }


        Objects::Value ASSIGN(Objects::Value value1, Objects::Value value2, std::map<std::string, Objects::Value*> &variables) {
            if (!value1.isvar) {
                return RaiseException("you may not assign a value to an immediate", 1);
            }
            Logging::Log("ASSIGNMENT " + value1.varname);
            Logging::Log("ASSIGNMENT " + value2._string);
            Objects::Value assignment = value2;
            assignment.isvar = true;
            assignment.varname = value1.varname;
            assignment._functions = value2._functions;
            variables[assignment.varname] = new Objects::Value();
            *variables[assignment.varname] = assignment;
            return Objects::Value();
        }


        Objects::Value Copy(Objects::Value pvalue) {
            Objects::Value value;
            value.varname = pvalue.varname;
            value.type = pvalue.type;
            value._int = pvalue._int;
            value._float = pvalue._float;
            value._bool = pvalue._bool;
            value._string = pvalue._string;
            value._attributes = pvalue._attributes;
            value._functions = pvalue._functions;
            value.isvar = true;
            return value;
        }

        
        Objects::Value print(Objects::Value printv) {
            std::cout << ToString(printv)._string << std::endl;
            return Objects::Value();
        }


        Objects::Value input() {
            std::string temp;
            std::cin >> temp;
            Objects::Value tempvalue = _string();
            tempvalue._string = temp;
            return tempvalue;
        }


        Objects::Value raise(Objects::Value value) {
            if (value.type == "exception") {
                value.isexception = true;
                return value;
            }
            return RaiseException(ToString(value)._string, -1);
        }


        Objects::Value _typeof(Objects::Value value) {
            Objects::Value returnvalue = _string();
            returnvalue._string = value.type;
            return returnvalue;
        }


        Objects::Value BuiltinCall(std::string funcname, std::map<std::string, Objects::Value*> parameters) {
            if (funcname == "print") {
                return print(*parameters["printv"]);
            } else if (funcname == "input") {
                return input();
            } else if (funcname == "typeof") {
                return _typeof(*parameters["var"]);
            } else if (funcname == "raise") {
                return raise(*parameters["what"]);
            } else if (funcname == "toString") {
                return ToString(*parameters["var"]);
            }
            return Objects::Value();
        }
        

        Objects::Value _string() {
            Objects::Value returnvalue;
            returnvalue.type = "string";
            returnvalue._functions["ASSIGN"] = Objects::Function("ASSIGN", "", 2);
            returnvalue._functions["ADD"] = Objects::Function("ADD", "", 2);
            returnvalue._functions["EQUAL"] = Objects::Function("EQUAL", "", 2);
            returnvalue._functions["NOTEQUAL"] = Objects::Function("NOTEQUAL", "", 2);
            return returnvalue;
        }


        Objects::Value _int() {
            Objects::Value returnvalue;
            returnvalue.type = "int";
            returnvalue._functions["ADD"] = Objects::Function("ADD", "", 2);
            returnvalue._functions["SUB"] = Objects::Function("SUB", "", 2);
            returnvalue._functions["MUL"] = Objects::Function("MUL", "", 2);
            returnvalue._functions["DIV"] = Objects::Function("DIV", "", 2);
            returnvalue._functions["ASSIGN"] = Objects::Function("ASSIGN", "", 2);
            returnvalue._functions["EQUAL"] = Objects::Function("EQUAL", "", 2);
            returnvalue._functions["NOTEQUAL"] = Objects::Function("NOTEQUAL", "", 2);
            returnvalue._functions["GREATEREQUAL"] = Objects::Function("GREATEREQUAL", "", 2);
            returnvalue._functions["LESSEREQUAL"] = Objects::Function("LESSEREQUAL", "", 2);
            returnvalue._functions["LESSER"] = Objects::Function("LESSER", "", 2);
            returnvalue._functions["GREATER"] = Objects::Function("GREATER", "", 2);
            return returnvalue;
        }


        Objects::Value _bool() {
            Objects::Value returnvalue;
            returnvalue._functions["ASSIGN"] = Objects::Function("ASSIGN", "", 2);
            returnvalue._functions["EQUAL"] = Objects::Function("EQUAL", "", 2);
            returnvalue._functions["NOTEQUAL"] = Objects::Function("NOTEQUAL", "", 2);
            returnvalue.type = "bool";
            return returnvalue;
        }


        Objects::Value _float() {
            Objects::Value returnvalue;
            returnvalue.type = "float";
            returnvalue._functions["ADD"] = Objects::Function("ADD", "", 2);
            returnvalue._functions["SUB"] = Objects::Function("SUB", "", 2);
            returnvalue._functions["MUL"] = Objects::Function("MUL", "", 2);
            returnvalue._functions["DIV"] = Objects::Function("DIV", "", 2);
            returnvalue._functions["ASSIGN"] = Objects::Function("ASSIGN", "", 2);
            returnvalue._functions["EQUAL"] = Objects::Function("EQUAL", "", 2);
            returnvalue._functions["NOTEQUAL"] = Objects::Function("NOTEQUAL", "", 2);
            returnvalue._functions["GREATEREQUAL"] = Objects::Function("GREATEREQUAL", "", 2);
            returnvalue._functions["LESSEREQUAL"] = Objects::Function("LESSEREQUAL", "", 2);
            returnvalue._functions["LESSER"] = Objects::Function("LESSER", "", 2);
            returnvalue._functions["GREATER"] = Objects::Function("GREATER", "", 2);
            return returnvalue;
        }


        Objects::Value _none() {
            Objects::Value returnvalue;
            returnvalue.type = "none";
            returnvalue._functions["ASSIGN"] = Objects::Function("ASSIGN", "", 2);
            return returnvalue;
        }


        Objects::Value RaiseException(std::string pwhat, int pcode) {
            Objects::Value returnvalue;
            returnvalue.type = "exception";
            Objects::Value what;
            what.type = "string";
            what.varname = "what";
            what._string = pwhat;
            returnvalue._attributes["what"] = new Objects::Value();
            *returnvalue._attributes["what"] = what;
            Objects::Value code;
            code.type = "int";
            code.varname = "code";
            code._int = pcode;
            returnvalue._attributes["code"] = new Objects::Value();
            *returnvalue._attributes["code"] = code;
            returnvalue.isexception = true;
            return returnvalue;
        }
    }
}