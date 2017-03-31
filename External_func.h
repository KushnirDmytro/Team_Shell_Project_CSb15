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


namespace fs = boost::filesystem;



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
    options_validator opt_inner_valid;
};

struct options_struct{
    map <string,  command_option> func_opts_map;
    options_validator opt_cross_valid;
};

class Function_options{
private:
    map <string,  command_option*> *func_opts_map;

    //default value definition
    bool noargs_allowed = true;
    //field for classes to initialize
    options_validator opt_cross_valid = nullptr;
public:

    Function_options(map <string,  command_option*> *func_opts_map){
        *this->func_opts_map = *func_opts_map;
    }

    command_option* get_option(string potential_arg) {
        if (this->func_opts_map->find(potential_arg)
            ==
            this->func_opts_map->end())
            return nullptr;
        else
            return this->func_opts_map->at(potential_arg);
    }


    bool are_options_cross_valid(size_t nargs, char *argv[]){
        if (nargs == 1){
            if (this->noargs_allowed)
                return true;
            else{
                printf("FOUNDED NO ARGUMENTS\n");
                return false;
            }
        }

        //!IMPORTANT!!!
        --nargs;


        if (opt_cross_valid == nullptr){
            cout << "cross_validator is not initialised" << endl;
            return true;
        }
        vector<string> args_vec;
        args_vec.insert(args_vec.end(), &argv[1], &argv[nargs]);


        //==========CHECK =========
        for (int i = 0; i < nargs; ++i){
            printf("function get  ===>%s<===  \n", argv[i]);
        }
        for(auto i: args_vec)
            cout<< "vector inserted word ===>" << i <<"<===" << endl;
        //==========CHECK =========


        command_option *prev_founded_option = nullptr;
        command_option *new_founded_option = nullptr;


        vector<string> arg_buf;

        for(auto iter_arg_name: args_vec){
            //founded_option = get_option(i);
            new_founded_option = get_option(iter_arg_name);
            if (new_founded_option == nullptr) {
                if (prev_founded_option == nullptr) {
                    printf("EROOR, FIRST ARGUMENT ===>%s<=== IS NOT OPTION\n", iter_arg_name.c_str());
                    return false;
                } else {
                    arg_buf.push_back(iter_arg_name);
                }
            }
            else{
                if(prev_founded_option == nullptr){
                    prev_founded_option = new_founded_option;
                }
                else{
                    char* temp_buf[arg_buf.size()];
                    str_vec_to_char_arr(arg_buf, temp_buf);
                    if (!prev_founded_option->opt_inner_valid(args_vec.size(), temp_buf)){
                        printf("ARGUMENT CHECK FAILED AT OPTION %s\n", iter_arg_name.c_str());
                        return false;
                    }
                }
            }
        }
        printf("ARGUMENT CHECK DONE \n");
        return true;
    }




    void str_vec_to_char_arr(vector<string> vec, char**arr){
        for (int i =0; i < vec.size(); ++i){
            strcpy(arr[i], vec[i].c_str());
        }
    }


};


class External_func : public Embedded_func{

private:
    Function_options *func_opts;
protected:
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
    bool are_options_valid(){
        this->func_opts->are_options_cross_valid(this->nargs, this->vargs);
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

public:
//Overriding
    int call(size_t nargs, char **args) override {
        this->nargs = nargs;
        this->vargs = args;
        if (this->func_opts->are_options_cross_valid(this->nargs, this->vargs)){
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
