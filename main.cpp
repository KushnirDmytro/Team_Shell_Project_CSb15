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
#include <fstream>
#include <cstring>


//========================CLASSES IMPORT==================

#include "Env/ConsoleView.h"
#include "Env/User.h"
#include "Env/Directory.h"
#include "Shell_core/Embedded_func.h"
#include "Shell_core/Utils/LineSplitter.h"
#include "Shell_core/LaneInterpreter.h"
#include "Shell_core/Utils/FileLaneIterator.h"
#include "External_func.h"
#include "Extern_LS.h"
#include "Env/Env.h"
#include "Shell_core/LaneInterpreter.h"
//====================CLASSES IMPORT END=====================
#include "algorithm"


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


namespace sh_core {
//=============FUNCTIONS AND STRUCTURES DECLARATIONS=============


    int my_cd(size_t nargs, char **args);

    int my_pwd(size_t nargs, char **args);

    int my_help(size_t nargs, char **args);

    int my_exit(size_t nargs, char **args);

    int my_sh(size_t nargs, char **args);


    sh_core::Embedded_func *my_shell_fileinterpreter;
    sh_core::Embedded_func *my_pwd_obj;
    sh_core::Embedded_func *my_cd_obj;
    sh_core::Embedded_func *my_help_obj;
    sh_core::Embedded_func *my_exit_obj;

}
namespace ext{

    int my_ls(size_t nargs, char **args);
    Extern_LS *extern_ls_obj;
}
//=============FUNCTIONS AND STRUCTURES DECLARATIONS END =============


namespace sh_core {
//TODO ask how to place it inside other namespace
    env::Env *environment;
    sh_core::LaneInterpreter *interpreter;
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
void my_loop()
{
    string line;
    vector<string> args;
    int status;


    do {
        sh_core::environment->console_->displayPromptMsg();
        line = my_read_line();
        if (strlen(line.c_str()) == 0){
            continue;
        }
        status = sh_core::interpreter->processSting(&line);
        //args = mySplitLine(line);
        //status = my_execute(args); //if 0 - finished, exited

    } while (status);
}


//=============ASSIST FUNCTIONS END============

//extern options_struct ls_options;







int main(int argc, char **argv)
{

    //===================DYNAMIC INITIALISATION ======================
    string cd_help_msg = "to change directory type in: cd <directory option_name> \n"
            "<~> = 'HOME' dirrectory if one defined \n"
            "<.> = current dirrectory  \n"
            "<..> = 'parrent directory'  ";
    string pwd_help_msg = "displays fullname of current execution directory";
    string help_help_msg = "just type 'help' to get info about my_Shell help instructions";
    string exit_help_msg = "function 'exit' terminates My_Shell execution";
    string shell_script_interpreter_help_msg = "file interpreter to execute env scripts \n"
            " 'mysh' <filename> to execurte script file";
    sh_core::my_cd_obj = new sh_core::Embedded_func("MY_CD", sh_core::my_cd, cd_help_msg );
    sh_core::my_pwd_obj = new sh_core::Embedded_func("MY_PWD", sh_core::my_pwd, pwd_help_msg );
    sh_core::my_help_obj = new sh_core::Embedded_func("MY_HELP", sh_core::my_help, help_help_msg );
    sh_core::my_exit_obj = new sh_core::Embedded_func("MY_EXIT", sh_core::my_exit, exit_help_msg );
    sh_core::my_shell_fileinterpreter =  new sh_core::Embedded_func("MY_shell_script_interpreter",
                                                                    sh_core::my_sh,
                                                  shell_script_interpreter_help_msg );
 //   my_ls_obj = new Embedded_func("MY_LS", my_ls, cd_help_msg );





//TODO ASK HOW IT WORKS
    //Function_options *ls_func_opts = new Function_options(ls_func_opts_map_ptr);






    //=========================ATTENTION!!!==========++++++!!!!!

    //Options *ls_func_opts = new Options("LS_options");

    ext::extern_ls_obj = new ext::Extern_LS("MY_EXT_LS", ext::my_ls , cd_help_msg);

    //=========================ATTENTION!!!==========++++++!!!!!





    sh_core::embedded_lib= {
            {"cd",   sh_core::my_cd_obj},
            {"pwd",  sh_core::my_pwd_obj},
            {"help", sh_core::my_help_obj},
            {"exit", sh_core::my_exit_obj},
            {"mysh", sh_core::my_shell_fileinterpreter},
            {"ls", ext::extern_ls_obj}

       };

    //env = nullptr;

    sh_core::environment =  new env::Env();
    sh_core::interpreter = new sh_core::LaneInterpreter();

/*
    default_user = new env::User();
    default_interpreter = new env::LaneInterpreter();
    def_line_split = new env::utils::LineSplitter();
    //init_user(&this_user);
    current_directory = new env::Directory();
    console = new env::ConsoleView(current_directory, default_user);
*/
    //===================DYNAMIC INITIALISATION END======================

    // Run command loop.
    my_loop();

    // Perform any shutdown/cleanup.

    //=====================MEMORY CLEAN / SHUTDOWN==========================
   /*
    delete default_user;
    delete current_directory;
    delete console;
    */
    delete sh_core::interpreter;
    delete sh_core::environment;
    //=====================MEMORY CLEAN SHUTDOWN END==========================

    return EXIT_SUCCESS;
}
