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


namespace ext{
    int myMkdirStaticLauncher(size_t nargs, char **args);

    ext::ExternMkdir *MkdirObject; //forvard static declaration

//just activator-function
    int myMkdirStaticLauncher(size_t nargs, char **argv) {

        if (nargs > 1){
            printf("Error, can create only one directory");
        }

        boost::system::error_code error_code;
        fs::path *pathBuf;
        for(int i = 1; i < nargs-1; ++i) {
            pathBuf = new fs::path(argv[i]);
            if (fs::exists(*pathBuf)){
                printf("%s already exists, skipping it\n", pathBuf->c_str());
                continue;
            }
            if ( ! fs::create_directory(*pathBuf, error_code))
                perror(error_code.message().c_str());
        }

        return 1;
    }

}


int main(int argc, char **argv)
{

    string mkdir_help = "Argument required for this function\n"
            "input mkdir <directoryName>  to create your directory";


    ext::MkdirObject = new ext::ExternMkdir("EXTERN_MKDIR", ext::myMkdirStaticLauncher, mkdir_help);

    sh_core::environment =  new env::Env();

    int result = ext::MkdirObject->call(static_cast<size_t >(argc), argv);

    //=====================MEMORY CLEAN / SHUTDOWN==========================
    delete ext::MkdirObject;
    delete sh_core::environment;
    //=====================MEMORY CLEAN SHUTDOWN END==========================

    return result;

    //===================DYNAMIC INITIALISATION END======================


}