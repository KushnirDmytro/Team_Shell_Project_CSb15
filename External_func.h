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

using namespace std;

/*

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

*/

//takes some arguments and checks are they in valid list (or of valid kind)
using options_validator = bool (*) (size_t, char**);

struct command_option{
    size_t opt_n;
    const string name;
    command_option *opt_args;
    options_validator opt_valid;
};

struct options_array{
    command_option* options;
    size_t opt_n;
};




class External_func : public Embedded_func{
private:
    options_array options_arr;

    External_func (const string &name, callable_function funct_to_assign, options_array options, string &help_msg):
            Embedded_func(name, funct_to_assign,  help_msg){
        this->options_arr = options;
    }


    int is_option(string potential_arg) {
        for(int i=0; i < this->options_arr.opt_n; ++i) {
            if (strcmp(this->options_arr.options->name.c_str(), potential_arg.c_str()) == 0 ) {
                return i;
            }
        }
        return -1;
    }


    /*
     * 1)find options in function's options list
     * 2)assume other words are referring to options arguments
     * 3)validate if option arguments are good
     */
    bool are_options_valid(){

        int options_index = 0;
        size_t args_index = 0;
        bool is_arg = false;
        int last_founded_option = -1;

        vector<char*> founded_args;

        while (args_index < this->nargs) {
            options_index = is_option(this->vargs[args_index]);
            if ( options_index != -1) {
                is_arg = true;
                last_founded_option = options_index;
                if (last_founded_option != -1) {
                    if( ! this->options_arr.options[last_founded_option].opt_valid(founded_args.size(), &founded_args[0])) {
                        printf(" ARGUMENT %s FOR OPTION %s IS INVALID \n", this->vargs[nargs], this->options_arr.options->name.c_str() );
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
    }



    bool validate_is_directory(size_t nargs, char** vargs){
        boost::filesystem::path full_path;
        for (size_t i = 0; i < nargs; ++i){
            full_path = boost::filesystem::current_path();
            full_path /= string(vargs[i]);
            if (
                    (!boost::filesystem::is_directory(vargs[i]))
                    ||
                    (! (boost::filesystem::is_directory(full_path) ) )
                    ) {

                return false;
            }
        }
        return true;
    }

//Overriding
    int call(size_t nargs, char **args) override {
        this->nargs = nargs;
        this->vargs = args;
        if (this->are_options_valid()){
            return Embedded_func::call(this->nargs, this->vargs);
        }
        else return 0;
    }

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
