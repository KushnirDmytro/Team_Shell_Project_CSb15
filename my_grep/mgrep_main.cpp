// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Created by d1md1m on 14.05.17.
//

#include <sys/wait.h>
#include <vector>
#include <iostream>


//========================CLASSES IMPORT==================
#include "../Env/ConsoleView.h"
#include "../Shell_core/EmbeddedFunc.h"
#include "../Shell_core/Utils/LineSplitter.h"
#include "ExternGrep.h"
//====================CLASSES IMPORT END=====================


//==============================DEFINITIONS====================


using  callable_function =  int (*)(size_t, char **);

namespace sh_core {
    env::Env *environment;
}


namespace extrn{
    //TODO allocate goo static launcher
    int myGrepStaticLauncher(size_t nargs, char **args){
        printf("PLACEHOLDER!!!\n");
        return  EXIT_SUCCESS;
    };

    extrn::ExternGrep *MgrepObject; //forvard static declaration



}


int main(int argc, char **argv)
{

    string mgrep_help = "filters arguments from file or std IN according to test_phrase\n"
            "input : grep [options] PATTERN [FILE...]  \n";


    extrn::MgrepObject = new extrn::ExternGrep("EXTERN_MKDIR", extrn::myGrepStaticLauncher, mgrep_help);

    int result = extrn::MgrepObject->call(static_cast<size_t >(argc), argv);

    //=====================MEMORY CLEAN / SHUTDOWN==========================
    delete extrn::MgrepObject;
    //=====================MEMORY CLEAN SHUTDOWN END==========================

    return result;

    //===================DYNAMIC INITIALISATION END======================


}