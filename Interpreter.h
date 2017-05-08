//
// Created by d1md1m on 28.03.17.
//

#ifndef LAB_2_SHELL_INTERPRETER_H
#define LAB_2_SHELL_INTERPRETER_H

#include <zconf.h>
#include <wait.h>
#include "Line_splitter.h"
#include "Embedded_func.h"
#include "iostream"

using namespace std;



class Interpreter{
private:
    Line_splitter splitter;
public:
    Interpreter();


    // launcher for custom modules
    int my_extern_launcher(char **args);

    int num_my_builtins();

    int my_execute(vector<string> args);

    int proceed_sting(string* values);

    ~Interpreter();
    //TODO filemasks
};


#endif //LAB_2_SHELL_INTERPRETER_H
