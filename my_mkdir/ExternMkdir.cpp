// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Created by d1md1m on 14.05.17.
//

#include "ExternMkdir.h"


namespace ext{
    ExternMkdir::ExternMkdir(const string &name,
                sh_core::callable_function funct_to_assign,
                string &help_msg):ExternalFunc(name,
            funct_to_assign,
            help_msg,
    new DefaultOptionsManager("MkdirOptsObject", &allGoingFine)){

        noargs_allowed_ = false;
    }

    int  ExternMkdir::call(size_t nargs, char **argv){
        initialNargs_ = nargs;
        initialVargs_ = argv;
        return sh_core::EmbeddedFunc::call(nargs, argv);
    };

    ExternMkdir::~ExternMkdir(){};
}