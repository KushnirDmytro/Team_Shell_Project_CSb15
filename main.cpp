// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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


//========================CLASSES IMPORT==================

#include "ConsoleView.h"
#include "Embedded_func.h"
#include "Line_splitter.h"
#include "Interpreter.h"
#include "FileLaneIterator.h"
#include "External_func.h"
#include "Extern_LS.h"
#include "lib_emb.h"
//====================CLASSES IMPORT END=====================



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






//=============ASSIST FUNCTIONS============


string my_read_line(void)
{
    string buffer;
    getline(cin,buffer);
    //buffer+=" ";
    return buffer;
}


//execuion loop
void my_loop(void)
{
    string line;
    vector<string> args;
    int status;


    do {
        console->display_all();
        line = my_read_line();
        if (strlen(line.c_str()) == 0){
            continue;
        }
        status = default_interpreter->proceed_sting(&line);
        //args = my_split_line(line);
        //status = my_execute(args); //if 0 - finished, exited

    } while (status);
}


//=============ASSIST FUNCTIONS END============

//extern options_struct ls_options;



int main(int argc, char **argv)
{
    //===================DYNAMIC INITIALISATION ======================
    string cd_help_msg = "to change directory type in: cd <directory option_name> \n<~> = 'HOME' dirrectory if one defined \n<.> = current dirrectory  \n<..> = 'parrent directory'  ";
    string pwd_help_msg = "displays fullname of current execution directory";
    string help_help_msg = "just type 'help' to get info about my_Shell help instructions";
    string exit_help_msg = "function 'exit' terminates My_Shell execution";
    string shell_script_interpreter_help_msg = "file interpreter to execute shell scripts \n 'mysh' <filename> to execurte script file";


    embedded_lib = {
            {"cd",  new Embedded_func("MY_CD", my_cd, cd_help_msg )},
            {"pwd",  new Embedded_func("MY_PWD", my_pwd, pwd_help_msg )},
            {"help", new Embedded_func("MY_HELP", my_help, help_help_msg )},
            {"exit", new Embedded_func("MY_EXIT", my_exit, exit_help_msg )},
            {"mysh", new Embedded_func("MY_shell_script_interpreter", my_sh, shell_script_interpreter_help_msg )}
    };

    default_user = new User();
    default_interpreter = new Interpreter();
    def_line_split = new Line_splitter();
    current_directory = new Directory();
    console = new ConsoleView(current_directory);

    //===================DYNAMIC INITIALISATION END======================

    // Run command loop.
    my_loop();

    // Perform any shutdown/cleanup.
    //=====================MEMORY CLEAN / SHUTDOWN==========================
    delete default_user;
    delete current_directory;
    delete console;
    delete def_line_split;
    delete default_interpreter;
    for(auto i:embedded_lib){
        delete i.second;
    }
    //=====================MEMORY CLEAN SHUTDOWN END==========================

    return EXIT_SUCCESS;
}
