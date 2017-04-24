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




// =====================>class OPTIONS  DECLARATION

using namespace std;


namespace fs = boost::filesystem;

class Options;

//takes some arguments and checks are they in valid list (or of valid kind)

using options_validator = bool (*) (size_t, char**, Options* ref_to_owner_object);


class Options{

    //TODO MAKE PROTECTED WHEN SOLVE INIT PROBLEM
public:

    string name;

    map <string,  Options*> *opts_map;

    //default value definition
    bool noargs_allowed = true;


    //field for classes to initialize
    //options_validator opt_cross_valid = nullptr;

    Options(string name);

    ~Options();

    Options* get_option(string potential_arg);

    virtual bool are_suboptions_valid(size_t nargs, char **argv);

    virtual bool are_options_cross_valid();

    void str_vec_to_char_arr(vector<string> vec, char**arr);

    bool argumentless_option_check(size_t nargs, char **argv);

    virtual bool suboptionS_arguments_validation(Options* opt_to_check, vector<string>* arg_buf);


};


// =====================>class OPTIONS DECLARATION END



//=======================> class External Funcion

class External_func : public Embedded_func{

private:
protected:

    //Options *func_opts;

    External_func (const string &name,
                   callable_function funct_to_assign,
                   //Options *options_ptr,
                   string &help_msg):
            Embedded_func(name, funct_to_assign,  help_msg){
        //this->func_opts = options_ptr;
    }


    bool validate_is_directory(size_t nargs, char** vargs);

public:
//Overriding
    int call(size_t nargs, char **args) override;


};

//=======================> class External Funcion END





//==================DEFAULT OBJECTS DECLARATIONS===================

extern User * default_user;

extern Directory *current_directory;

extern Line_splitter* def_line_split;

extern Interpreter* default_interpreter;

extern ConsoleView *console;

extern map <string, Embedded_func*> embedded_lib;



//=================DEFAULT OBJECTS DECLARATIONS END================








#endif //LAB_2_SHELL_EXTERNAL_FUNC_H
