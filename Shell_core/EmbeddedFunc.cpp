// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Created by d1md1m on 27.03.17.
//


#include "EmbeddedFunc.h"

namespace sh_core {


    EmbeddedFunc::EmbeddedFunc(const string &name, callable_function funct_to_assign, const string &help_msg) {
        name_ = name;
        func_ = funct_to_assign;
        help_info_ = help_msg;
    }


    int EmbeddedFunc::needToPrintHelp(const size_t nargs, char **&argvector) {

        bool argumentsAreEmpty = (nargs == 0) ||
                ( (nargs == 1) &&  ( !noargs_allowed_ )  ) ;

        if (argumentsAreEmpty) return 1;

        for (int i = 0; i < nargs; ++i) {
            if ((strcmp(argvector[i], "--help") == 0) || (strcmp(argvector[i], "-h") == 0)) {
                return 1;
            }

        }
        return 0;
    }

    void EmbeddedFunc::outputHelp(const string &helpMsg) const {
        printf("%s\n", help_info_.c_str());
    }


    int EmbeddedFunc::call(size_t nargs, char **args) {
        if (initialVargs_ == NULL) { //only when it had not been initialized before in call-stack
            initialNargs_ = nargs;
            initialVargs_ = args;
        }
        if (needToPrintHelp(initialNargs_, initialVargs_)) {
            outputHelp(help_info_);
            return 1;
        }
        return func_(initialNargs_, initialVargs_);
    }
}