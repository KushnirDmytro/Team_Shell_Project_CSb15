//
// Created by d1md1m on 14.05.17.
//

#ifndef LAB_2_SHELL_EMBBEDDEDLIB_H
#define LAB_2_SHELL_EMBBEDDEDLIB_H

#include <string>
#include "EmbeddedFunc.h"

using std::string;


namespace fs = boost::filesystem;

namespace sh_core {
//=============FUNCTIONS AND STRUCTURES DECLARATIONS=============

    class coreFuncLib{
    public:
        coreFuncLib();
        ~coreFuncLib();

        std::map<string, EmbeddedFunc *> embedded_lib_;

        std::map<string, fs::path*> external_lib_;

        bool hasSuchEmbedded(const string * const arg) const;

        bool hasSuchExternal(const string * const arg) const;

        bool hasMyshExtention(const string *const arg) const;

        int getNumOfMyBuiltins() const;
    };
    int myCd(size_t nargs, char **args);

    int myPwd(size_t nargs, char **args);

    int myHelp(size_t nargs, char **args);

    int myExit(size_t nargs, char **args);

    int mySh(size_t nargs, char **args);

    int mEcho(size_t nargs, char **args);

}

#endif //LAB_2_SHELL_EMBBEDDEDLIB_H
