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

class Function_options;

//takes some arguments and checks are they in valid list (or of valid kind)
using options_validator = bool (*) (size_t, char**, Function_options* ref_to_owner_object);


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

    func_opts_flags *opts_flags = nullptr;

    map <string,  command_option*> *func_opts_map;


    //default value definition
    bool noargs_allowed = true;
    //field for classes to initialize
    options_validator opt_cross_valid = nullptr;
public:

    Function_options(map <string,  command_option*> *func_opts_map);
    ~Function_options();

    command_option* get_option(string potential_arg);

    bool are_options_cross_valid(size_t nargs, char *argv[]);

    void str_vec_to_char_arr(vector<string> vec, char**arr);


};


class External_func : public Embedded_func{



private:
protected:

    Function_options *func_opts;

    External_func (const string &name,
                   callable_function funct_to_assign,
                   Function_options *options_ptr,
                   string &help_msg):
            Embedded_func(name, funct_to_assign,  help_msg){
        this->func_opts = options_ptr;
    }



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
