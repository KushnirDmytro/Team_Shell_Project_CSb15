// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>
#include "boost/asio.hpp"

//========================CLASSES IMPORT==================
#include "Env/ConsoleView.h"
#include "Shell_core/EmbeddedFunc.h"
#include "Shell_core/Utils/LineSplitter.h"
#include "Shell_core/LaneInterpreter.h"
//====================CLASSES IMPORT END=====================


//==============================DEFINITIONS====================


using  callable_function =  int (*)(size_t, char **);

//============================DEFINITIONS END======================




//=============FUNCTIONS AND STRUCTURES DECLARATIONS END =============

//TODO REMASTER mpwd
// TODO REMASTER mexit
// TODO make auto_call embedded interpreter if argument is [.msh] script
namespace sh_core {
    env::Env *environment;
    sh_core::LaneInterpreter *interpreter;
}


//=============ASSIST FUNCTIONS============


string my_read_line(void)
{
    string buffer;
    getline(std::cin,buffer);
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
        if (strlen(line.c_str()) == 0){ //handling situation of empty line been red
            continue;
        }
        status = sh_core::interpreter->processSting(&line);

    } while (status);
}


//=============ASSIST FUNCTIONS END============







int main(int argc, char **argv)
{

    sh_core::environment =  new env::Env();
    sh_core::interpreter = new sh_core::LaneInterpreter();
    //===================DYNAMIC INITIALISATION END======================

    // Run command loop.
    my_loop();

    //=====================MEMORY CLEAN / SHUTDOWN==========================

    delete sh_core::interpreter;
    delete sh_core::environment;
    //=====================MEMORY CLEAN SHUTDOWN END==========================

    return EXIT_SUCCESS;
}
