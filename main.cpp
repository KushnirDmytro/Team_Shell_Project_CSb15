
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


//========================CLASSES IMPORT==================

#include "ConsoleView.h"
//#include "User.h"
#include "Directory.h"
#include "Embedded_func.h"
#include "Line_splitter.h"
#include "Interpreter.h"
#include "FileLaneIterator.h"
#include "External_func.h"
#include "Extern_LS.h"
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


//execuion loop
void my_loop(void)
{
    string line;
    vector<string> args;
    int status;


    do {
        console->display_all();
        line = my_read_line();
        status = default_interpreter->proceed_sting(&line);
        //args = my_split_line(line);
        //status = my_execute(args); //if 0 - finished, exited

    } while (status);
}


//=============ASSIST FUNCTIONS END============

extern options_struct ls_options;










int main(int argc, char **argv)
{

    //===================DYNAMIC INITIALISATION ======================
    string cd_help_msg = "SOME CD HELP";
    my_cd_obj = new Embedded_func("MY_CD", my_cd, cd_help_msg );
    my_pwd_obj = new Embedded_func("MY_PWD", my_pwd, cd_help_msg );
    my_help_obj = new Embedded_func("MY_HELP", my_help, cd_help_msg );
    my_exit_obj = new Embedded_func("MY_EXIT", my_exit, cd_help_msg );

 //   my_ls_obj = new Embedded_func("MY_LS", my_ls, cd_help_msg );





//TODO ASK HOW IT WORKS
    //Function_options *ls_func_opts = new Function_options(ls_func_opts_map_ptr);

    Function_options *ls_func_opts = new Function_options(nullptr);


    extern_ls_obj = new Extern_LS("MY_EXT_LS", my_ls ,  ls_func_opts , cd_help_msg);

/*
    (const string &name,
    callable_function funct_to_assign,
    options_struct options,
    string &help_msg):
*/

   // extern_ls_obj = new Extern_LS("MY_LS", my_ls, ls_options, cd_help_msg);

    //my_shell_fileinterpreter = new Embedded_func("MY_FILEINTERPRETER", my_sh, cd_help_msg);

    embedded_lib= {
            {"cd",   my_cd_obj},
            {"pwd",  my_pwd_obj},
            {"help", my_help_obj},
            {"exit", my_exit_obj},
            {"mysh", my_shell_fileinterpreter},
            {"ls", extern_ls_obj}

    };


    default_user = new User();
    default_interpreter = new Interpreter();
    def_line_split = new Line_splitter();
    //init_user(&this_user);
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
    //=====================MEMORY CLEAN SHUTDOWN END==========================

    return EXIT_SUCCESS;
}
