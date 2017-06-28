// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Created by d1md1m on 14.05.17.
//


#include <string>
#include "ExternMkdir.h"


namespace extrn{
    ExternGrep::ExternGrep(const string &name,
                sh_core::callable_function funct_to_assign,
                string &help_msg):ExternalFunc(name,
            funct_to_assign,
            help_msg,
    new DefaultOptionsManager("MkdirOptsObject", &allGoingFine)){

        noargs_allowed_ = false;
    }

    int  ExternGrep::call(size_t nargs, char **argv){
        initialNargs_ = nargs;
        initialVargs_ = argv;
        int result_code = sh_core::EmbeddedFunc::call(nargs, argv);
        return result_code;
    };

    ExternGrep::~ExternGrep(){};
}