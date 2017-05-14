//
// Created by d1md1m on 14.05.17.
//

#ifndef LAB_2_SHELL_EMBBEDDEDLIB_H
#define LAB_2_SHELL_EMBBEDDEDLIB_H


#include <zconf.h>
#include <wait.h>
#include <iostream>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/mpl/size_t.hpp>
#include <string>

using std::string;

namespace sh_core {
//=============FUNCTIONS AND STRUCTURES DECLARATIONS=============


    int myCd(size_t nargs, char **args);

    int myPwd(size_t nargs, char **args);

    int myHelp(size_t nargs, char **args);

    int myExit(size_t nargs, char **args);

    int mySh(size_t nargs, char **args);
}

#endif //LAB_2_SHELL_EMBBEDDEDLIB_H
