//
// Created by d1md1m on 08.05.17.
//

#ifndef LAB_2_SHELL_LIB_EMB_H
#define LAB_2_SHELL_LIB_EMB_H


#include <boost/filesystem/operations.hpp>

#include "Directory.h"
#include "ConsoleView.h"
#include "Line_splitter.h"
#include "FileLaneIterator.h"
#include "Interpreter.h"

using namespace std;




//=============FUNCTIONS AND STRUCTURES DECLARATIONS=============

int my_ls(size_t nargs, char **args);
int my_cd(size_t nargs, char **args);
int my_pwd(size_t nargs, char **args);
int my_help(size_t nargs, char **args);
int my_exit(size_t nargs, char **args);
int my_sh(size_t nargs, char **args);


//=============FUNCTIONS AND STRUCTURES DECLARATIONS END =============



#endif //LAB_2_SHELL_LIB_EMB_H
