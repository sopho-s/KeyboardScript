#include "Errors.h"


namespace Engine {
    namespace Logging {
        int errorcount = 0;
        int warncount = 0;
        int infocount = 0;
        bool infooff = false;
        bool warnoff = false;
        bool erroff = false;


        void Log(Error err) {
            errorcount++;
            if (err.column != -1 && !erroff) {
                std::cerr << "[-] ERROR CODE " << std::to_string(err.code) << ": " << err.what << " occured at column: " << err.column + 1 << " row: " << err.row + 1 << std::endl;
            } else if (!erroff) {
                std::cerr << "[-] ERROR CODE " << std::to_string(err.code) << ": " << err.what << std::endl;
            }
        }


        void Log(Warning warn) {
            warncount++;
            if (!warnoff) {
                std::cout << "[-] WARNING CODE " << std::to_string(warn.code) << ": " << warn.what << " occured at column: " << warn.column << " row: " << warn.row << std::endl;
            }
        }


        void Log(std::string info) {
            infocount++;
            if (!infooff) {
                std::cout << "[-] INFO: " << info << std::endl;
            }
        }


        void Log(char _char) {
            infocount++;
            if (!infooff) {
                std::cout << "[-] INFO: " << _char << std::endl;
            }
        }

        
        void Log(Token token) {
            infocount++;
            if (!infooff) {
                std::cout << "[-] INFO: " << token.column << " " << token.row << " " << token.type << " " << token.value << std::endl;
            }
        }

        
        void Log(int value) {
            infocount++;
            if (!infooff) {
                std::cout << "[-] INFO: " << std::to_string(value) << std::endl;
            }
        }


        /*void Log(bool value) {
            infocount++;
            if (!infooff) {
                if (value) {
                    std::cout << "[-] INFO: true" << std::endl;
                } else {
                    std::cout << "[-] INFO: false" << std::endl;
                }
            }
        }*/


        int GetErrorCount() {
            return errorcount;
        }


        int GetWarnCount() {
            return warncount;
        }


        int GetInfoCount() {
            return infocount;
        }


        void SetArgs(bool perroff, bool pwarnoff, bool pinfooff) {
            erroff = perroff;
            warnoff = pwarnoff;
            infooff = pinfooff;
        }
    }
}