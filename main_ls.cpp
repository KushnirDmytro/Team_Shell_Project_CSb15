// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Created by d1md1m on 08.05.17.
//


#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <pwd.h>
#include <map>
#include <fstream>
#include <cstring>

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





// HOW TO CURE SPACED FILENAMES???
void cure_spaced_filenemas(size_t nargs,char* vargs[]){
    for (size_t i = 0; i < nargs; ++i){
        //TODO
        //USE BOOST::FILESYSTEM::PATH to recognise start, mid, end of directory
    }
}



vector<boost::filesystem::path> regex_match_directories(string regex){
    //TODO
    //process via iterators all possible pathes that match such expression
    vector<boost::filesystem::path> proceed_buffer;
    vector<boost::filesystem::path> directories;
    return directories;
}



//=============FUNCTIONS AND STRUCTURES DECLARATIONS=============

int my_ls(size_t nargs, char **args);
int my_cd(size_t nargs, char **args);
int my_pwd(size_t nargs, char **args);
int my_help(size_t nargs, char **args);
int my_exit(size_t nargs, char **args);
int my_sh(size_t nargs, char **args);

callable_function my_cd_addr = my_cd;
callable_function my_pwd_addr = my_pwd;
callable_function my_help_addr = my_help;
callable_function my_exit_addr = my_exit;
callable_function my_sh_addr = my_sh;
callable_function my_ls_addr = my_ls;


Embedded_func *my_shell_fileinterpreter;
Embedded_func *my_pwd_obj;
Embedded_func *my_cd_obj;
Embedded_func *my_help_obj;
Embedded_func *my_exit_obj;
Embedded_func *my_ls_obj;
Extern_LS *extern_ls_obj;

//=============FUNCTIONS AND STRUCTURES DECLARATIONS END =============





//=============ASSIST FUNCTIONS============


string my_read_line(void)
{
    string buffer;
    getline(cin,buffer);
    //buffer+=" ";
    return buffer;
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
