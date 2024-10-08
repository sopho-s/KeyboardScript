#pragma once
#include <string>
#include "../Objects/Objects.h"
#include "../Logging/Errors.h"
#include <iostream>
#include <time.h>
#include <math.h>
#include "../Misc/String.h"

namespace Engine {
    namespace Builtins {
        Objects::Value ToString(Objects::Value value);


        Objects::Value ToInt(Objects::Value value);


        Objects::Value ADD(Objects::Value value1, Objects::Value value2);


        Objects::Value SUB(Objects::Value value1, Objects::Value value2);


        Objects::Value MUL(Objects::Value value1, Objects::Value value2);


        Objects::Value DIV(Objects::Value value1, Objects::Value value2);


        Objects::Value MOD(Objects::Value value1, Objects::Value value2);


        Objects::Value EQUAL(Objects::Value value1, Objects::Value value2);


        Objects::Value NOTEQUAL(Objects::Value value1, Objects::Value value2);


        Objects::Value GREATEREQUAL(Objects::Value value1, Objects::Value value2);


        Objects::Value LESSEREQUAL(Objects::Value value1, Objects::Value value2);


        Objects::Value LESSER(Objects::Value value1, Objects::Value value2);


        Objects::Value GREATER(Objects::Value value1, Objects::Value value2);


        Objects::Value ASSIGN(Objects::Value value1, Objects::Value value2, std::unordered_map<std::string, std::shared_ptr<Objects::Value>> &variables);


        Objects::Value Copy(Objects::Value value);


        Objects::Value print(Objects::Value value);


        Objects::Value pow(Objects::Value value1, Objects::Value value2);


        Objects::Value input();


        Objects::Value raise(Objects::Value value);


        Objects::Value _typeof(Objects::Value value);


        Objects::Value _timesec();


        Objects::Value BuiltinCall(std::string funcname, std::unordered_map<std::string, std::shared_ptr<Objects::Value>> parameters);


        Objects::Value _string();


        Objects::Value _int();


        Objects::Value _bool();


        Objects::Value _float();


        Objects::Value _none();


        Objects::Value RaiseException(std::string pwhat, int pcode);


        Objects::Value* FindValue(Objects::Value value, std::unordered_map<std::string, std::shared_ptr<Objects::Value>> &variables);


        void AddBuiltins(std::unordered_map<std::string, std::shared_ptr<Objects::Function>>& functions, std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<Objects::Function>>>& classtemps);
    }
}