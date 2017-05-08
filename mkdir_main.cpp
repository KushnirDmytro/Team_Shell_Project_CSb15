// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Created by d1md1m on 08.05.17.
//

#include <sys/wait.h>

#include <vector>
#include <iostream>

#include "ConsoleView.h"
#include "User.h"
#include "Directory.h"
#include "Embedded_func.h"
#include "Line_splitter.h"
#include "Interpreter.h"
#include "FileLaneIterator.h"
#include "External_func.h"
#include "Extern_LS.h"


using namespace std;

int my_mkdir(int nargs, char **vargs){

    return 1;
}


int main(int nargs, char **vargs ){


    string my_mkdir_msg = "this is mkdir help msg";


    default_user = new User();
    default_interpreter = new Interpreter();
    def_line_split = new Line_splitter();
    current_directory = new Directory();
    console = new ConsoleView(current_directory);


    External_func *extern_ls_obj  = new External_func("MY_EXT_MKDIR", my_mkdir , my_mkdir_msg);

}