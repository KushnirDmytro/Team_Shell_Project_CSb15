// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Created by d1md1m on 27.03.17.
//


#include "Embedded_func.h"


using namespace std;



Embedded_func::Embedded_func(const string &name_, callable_function funct_to_assign_, string &help_msg_){
    name=name_;
    func = funct_to_assign_;
    help_info = help_msg_;
}


int Embedded_func::search_for_help(size_t nargs, char** &argvector){
    for (int i = 0; i< nargs ; ++i){
        if (( strcmp(argvector[i], "--help") == 0  ) || ( strcmp(argvector[i], "-h") == 0  ) ){
            return 1;
        }
    }
    return 0;
}

void Embedded_func::output_help(string &helpMsg){
    printf("%s\n", help_info.c_str());
}


int Embedded_func::call(size_t nargs_, char **args_){
    nargs = nargs_;
    vargs = args_;
//    this->initialized = true;
    if (search_for_help(nargs, vargs)){
        output_help(help_info);
        return 1;
    }
    return func (nargs, vargs);
    //     return 1;
}





