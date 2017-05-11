//
// Created by d1md1m on 27.03.17.
//

#ifndef LAB_2_SHELL_EMBEDDEDFUNC_H
#define LAB_2_SHELL_EMBEDDEDFUNC_H


//namespace fs = boost::filesystem;

#include  <string>
#include <map>
#include <cstring>
//#include "default_definitions.h"


#include <boost/filesystem/operations.hpp>


#include "../Env/Env.h"
#include "Utils/FileLaneIterator.h"



namespace sh_core{

using  callable_function =  int (*)(size_t, char **);



#define  home_dir_call  "~"




class Embedded_func{
protected:
    string name;
    char** vargs;
    size_t nargs;
    string help_info;
    //bool initialized;
    callable_function func;
public:
    Embedded_func(const string &name, callable_function funct_to_assign, string &help_msg);

    int search_for_help(size_t nargs, char** &argvector);

    void output_help(string &helpMsg);

    virtual int call(size_t nargs, char **args);

};


extern std::map <string, Embedded_func*> embedded_lib;


extern env::Env *environment;

}
#endif //LAB_2_SHELL_EMBEDDEDFUNC_H
