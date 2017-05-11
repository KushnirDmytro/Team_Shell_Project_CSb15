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


    int EmbeddedFunc::searchForHelp(const size_t nargs, char **&argvector) {
        for (int i = 0; i < nargs; ++i) {
            if ((strcmp(argvector[i], "--help") == 0) || (strcmp(argvector[i], "-h") == 0)) {
                return 1;
            }
        }
        return 0;
    }

    void EmbeddedFunc::outputHelp(const string &helpMsg) const{
        printf("%s\n", help_info_.c_str());
    }


    int EmbeddedFunc::call(size_t nargs_, char **args_) {
        nargs = nargs_;
        vargs = args_;
        if (searchForHelp(nargs, vargs)) {
            outputHelp(help_info_);
            return 1;
        }
        return func_(nargs, vargs);
        //     return 1;
    }




//====================BUILT-IN COMMANDS END============

}