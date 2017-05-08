// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Created by d1md1m on 08.05.17.
//


#include <sys/wait.h>

#include <vector>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <map>

#include "ConsoleView.h"
#include "User.h"
#include "Directory.h"
#include "Embedded_func.h"
#include "Line_splitter.h"
#include "Interpreter.h"
#include "FileLaneIterator.h"
#include "External_func.h"
#include "Extern_LS.h"



//==============================DEFINITIONS====================

using namespace std;


#define  home_dir_call  "~"


using  callable_function =  int (*)(size_t, char **);

//============================DEFINITIONS END======================



User * default_user;

Directory *current_directory;

Line_splitter* def_line_split;

Interpreter* default_interpreter;

ConsoleView *console;

map <string, Embedded_func*> embedded_lib;



//=============FUNCTIONS AND STRUCTURES DECLARATIONS END =============





//=============ASSIST FUNCTIONS============



Extern_LS *extern_ls_obj;
int my_ls(size_t nargs, char **argv)
{

    return extern_ls_obj->my_ls_inner(nargs, argv);
    //my_ls_inner(nargs, argv);
}


int main(int argc, char **argv){


    string my_ls_msg = "this is ls help msg";


    default_user = new User();
    default_interpreter = new Interpreter();
    def_line_split = new Line_splitter();
    //init_user(&this_user);
    current_directory = new Directory();
    console = new ConsoleView(current_directory);


    extern_ls_obj  = new Extern_LS("MY_EXT_LS", my_ls , my_ls_msg);


    extern_ls_obj->call(argc, argv);
}
