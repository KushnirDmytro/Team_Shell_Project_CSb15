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

#include "Shell_core/ReducerToTasks.h"
//====================CLASSES IMPORT END=====================


//==============================DEFINITIONS====================


using  callable_function =  int (*)(size_t, char **);

//============================DEFINITIONS END======================




//=============FUNCTIONS AND STRUCTURES DECLARATIONS END =============


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
int my_loop()
{
    string line;
    vector<string> args;
    int status;

    do {
        sh_core::environment->console_->displayPromptMsg();
        line = my_read_line();
        if (strlen(line.c_str()) == 0){ //handling situation of empty line been red
            continue;

            //TODO handler for returning "0"
        }
        status = sh_core::interpreter->processSting(&line);

    } while (!status);
    return status;
}


//=============ASSIST FUNCTIONS END============




// TODO EXIT problem : impossible to return 0
// TODO other problem : trouble with concept of multythread execution (pass env as a var)

int main(int argc, char **argv)
{

//    sh_core::ReducerToTasks *red2 = new sh_core::ReducerToTasks();



    // TODO download old Env if User is registered
    sh_core::environment =  new env::Env();
    sh_core::interpreter = new sh_core::LaneInterpreter();
    //===================DYNAMIC INITIALISATION END======================


    //TODO restyle external sriptFile analysys
    if (argc == 2){
        string *line = new string(argv[1]);
        int result = sh_core::interpreter->processSting(line);
        return result;
    }
    if (argc > 2){
        printf("Too many arguments (one script-file name at time pls)!\n");
        return EXIT_FAILURE;
    }


    //TODO Returning ZERO
        // Run command loop.
    int execution_result_code = my_loop();

    //=====================MEMORY CLEAN / SHUTDOWN==========================

    delete sh_core::interpreter;
    delete sh_core::environment;
    //=====================MEMORY CLEAN SHUTDOWN END==========================

    return execution_result_code;
}
