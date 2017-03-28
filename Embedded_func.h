//
// Created by d1md1m on 27.03.17.
//

#ifndef LAB_2_SHELL_EMBEDDEDFUNC_H
#define LAB_2_SHELL_EMBEDDEDFUNC_H

using namespace std;

#include  <string>
#include <map>
#include <cstring>
//#include "default_definitions.h"


using  callable_function =  int (*)(size_t, char **);



#define  home_dir_call  "~"




class Embedded_func{
private:
    string name;
    char** vargs;
    size_t nargs;
    string help_info;
    bool initialized;
    callable_function func;
public:
    Embedded_func(const string &name, callable_function funct_to_assign, string &help_msg){
        this->name=name;
        this->func = funct_to_assign;
        this->help_info = help_msg;
    }


    int search_for_help(size_t nargs, char** &argvector){
        for (int i = 0; i< nargs ; ++i){
            if (( strcmp(argvector[i], "--help") == 0  ) || ( strcmp(argvector[i], "-h") == 0  ) ){
                return 1;
            }
        }
        return 0;
    }

    void output_help(string &helpMsg){
        printf("%s\n", this->help_info.c_str());
    }


    int call(size_t nargs, char **args){
        this->nargs = nargs;
        this->vargs = args;
        this->initialized = true;
        if (this->search_for_help(this->nargs, this->vargs)){
            this->output_help(this->help_info);
            return 1;
        }
        return func (this->nargs, this->vargs);
   //     return 1;
    }

};


extern map <string, Embedded_func*> embedded_lib;







#endif //LAB_2_SHELL_EMBEDDEDFUNC_H
