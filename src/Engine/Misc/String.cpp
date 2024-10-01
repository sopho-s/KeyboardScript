#include "String.h"
#include <cstdlib>

namespace Engine {
    namespace Misc {
        int Contains(std::string string, std::string key) {
            for (int i = 0; i < string.length() - key.length(); i++) {
                bool isin = true;
                for (int t = 0; t < key.length(); t++) {
                    if (string[i+t] != key[t]) {
                        isin = false;
                        break;
                    }
                }
                if (isin) {
                    return i;
                }
            }
            return -1;
        }


        int Count(std::string string, std::string key) {
            int count = 0;
            for (int i = 0; i < string.length() - key.length(); i++) {
                bool isin = true;
                for (int t = 0; t < key.length(); t++) {
                    if (string[i+t] != key[t]) {
                        isin = false;
                        break;
                    }
                }
                if (isin) {
                    count++;
                }
            }
            return count;
        }


        std::string Random(int length) {
            std::srand(128390128);
            std::string randomstr = "";
            for (int i = 0; i < length; i++) {
                randomstr += 'a' + rand()%26;
            }
            return randomstr;
        }
    }
}