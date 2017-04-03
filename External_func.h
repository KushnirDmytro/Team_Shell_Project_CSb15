//
// Created by d1md1m on 29.03.17.
//

#ifndef LAB_2_SHELL_EXTERNAL_FUNC_H
#define LAB_2_SHELL_EXTERNAL_FUNC_H

#include <cstddef>
#include <string>
#include <vector>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/range/iterator_range.hpp>

#include "User.h"
#include "Directory.h"
#include "Line_splitter.h"
#include "Interpreter.h"
#include "ConsoleView.h"
#include "FileLaneIterator.h"

#include "Embedded_func.h"


//#include "Extern_LS.h"
//extern Extern_LS *extern_ls_obj;


using namespace std;


namespace fs = boost::filesystem;


class External_func;

//takes some arguments and checks are they in valid list (or of valid kind)
using options_validator = bool (*) (size_t, char**, External_func*);


struct func_opts_flags{};


struct command_option{
    size_t opt_n;
    string name;
    command_option *opt_args;
    options_validator opt_inner_valid;

};

struct options_struct{
    map <string,  command_option> func_opts_map;
    options_validator opt_cross_valid;
};




class Function_options{

    //TODO MAKE PRIVATE WHEN SOLVE INIT PROBLEM
public:
    map <string,  command_option*> *func_opts_map;

    //default value definition
    bool noargs_allowed = true;
    //field for classes to initialize
    options_validator opt_cross_valid = nullptr;
public:

    Function_options(map <string,  command_option*> *func_opts_map);

    command_option* get_option(string potential_arg);

    bool are_options_cross_valid(size_t nargs, char *argv[], External_func* ref_to_owner);

    void str_vec_to_char_arr(vector<string> vec, char**arr);


};


class External_func : public Embedded_func{

public:
    func_opts_flags *opts_flags = nullptr;


private:
protected:

    Function_options *func_opts;

    External_func (const string &name,
                   callable_function funct_to_assign,
                   Function_options *options_ptr,
            //options_validator opt_cross_valid,
                   string &help_msg):
            Embedded_func(name, funct_to_assign,  help_msg){
        this->func_opts = options_ptr;
        // this->opt_cross_valid = opt_cross_valid;
    }

    //options_validator opt_cross_valid;



    /*
     * 1)find options in function's options list
     * 2)assume other words are referring to options arguments
     * 3)validate if option arguments are good
     */
    // bool are_options_valid(){
    //    this->func_opts->are_options_cross_valid(this->nargs, this->vargs);
    /*
    int options_index = 0;
    size_t args_index = 0;
    bool is_arg = false;
    int last_founded_option = -1;

    vector<char*> founded_args;

    while (args_index < this->nargs) {
        options_index = this->func_opts->is_option(this->vargs[args_index]);
        if ( options_index != -1) {
            is_arg = true;
            last_founded_option = options_index;
            if (last_founded_option != -1) {
                if( ! this->func_opts.options[last_founded_option].opt_inner_valid(founded_args.size(), &founded_args[0])) {
                    printf(" ARGUMENT %s FOR OPTION %s IS INVALID \n", this->vargs[nargs], this->func_opts.options->name.c_str() );
                    return false;
                }
            }
            else{
                printf("UNKNONW ARGUMENT %s FOUND BEFORE ANY OPTIONS ", this->vargs[args_index]);
                return false;
            }
        }else {
            if (last_founded_option != -1) {
                is_arg = false;
                founded_args.push_back(this->vargs[args_index]);
            }
        }
        ++args_index;
    }
    return true;
     */
    // }



    bool validate_is_directory(size_t nargs, char** vargs);

public:
//Overriding
    int call(size_t nargs, char **args) override;


};






//==================DEFAULT OBJECTS DECLARATIONS===================

extern User * default_user;

extern Directory *current_directory;

extern Line_splitter* def_line_split;

extern Interpreter* default_interpreter;

extern ConsoleView *console;

extern map <string, Embedded_func*> embedded_lib;



//=================DEFAULT OBJECTS DECLARATIONS END================








#endif //LAB_2_SHELL_EXTERNAL_FUNC_H
