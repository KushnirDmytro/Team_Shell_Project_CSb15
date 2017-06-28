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
#include "ExternMkdir.h"
//====================CLASSES IMPORT END=====================


//==============================DEFINITIONS====================


using  callable_function =  int (*)(size_t, char **);

namespace sh_core {
    env::Env *environment;
}


namespace extrn{
    int myMkdirStaticLauncher(size_t nargs, char **args);

    extrn::ExternGrep *MkdirObject; //forvard static declaration

//just activator-function
    int myMkdirStaticLauncher(size_t nargs, char **argv) {



        boost::system::error_code error_code;
        fs::path pathBuf;
        for(int i = 1; i < nargs; ++i) {
            pathBuf = fs::current_path() ;
            pathBuf /= fs::path(argv[i]);
            if (fs::exists(pathBuf)){
                printf("[%s] already exists, skipping it\n", pathBuf.c_str());
                continue;
            }
            if ( ! fs::create_directory(pathBuf, error_code)) {
                perror(error_code.message().c_str());
            }else{
                printf("directory [%s] successfully created \n", pathBuf.c_str());
            }
        }

        return 0;
    }

}


int main(int argc, char **argv)
{

    string mkdir_help = "Argument required for this function\n"
            "input mkdir <directoryName>  to create your directory (or directories)\n";


    extrn::MkdirObject = new extrn::ExternGrep("EXTERN_MKDIR", extrn::myMkdirStaticLauncher, mkdir_help);

    int result = extrn::MkdirObject->call(static_cast<size_t >(argc), argv);

    //=====================MEMORY CLEAN / SHUTDOWN==========================
    delete extrn::MkdirObject;
    //=====================MEMORY CLEAN SHUTDOWN END==========================

    return result;

    //===================DYNAMIC INITIALISATION END======================


}