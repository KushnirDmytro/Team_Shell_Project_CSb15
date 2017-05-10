//
// Created by d1md1m on 28.03.17.
//

#ifndef LAB_2_SHELL_INTERPRETER_H
#define LAB_2_SHELL_INTERPRETER_H

#include <zconf.h>
#include <wait.h>
#include "Utils/LineSplitter.h"
//#include "../Embedded_func.h"
#include "iostream"

using namespace std;

namespace env {

    class Interpreter {
    private:
        utils::LineSplitter *splitter;


    public:
        Interpreter();


        // launcher for custom modules
        int myExternLauncher(char **args) const;

        int getNumOfMyBuiltins() const;

        int myExecute(const vector<string> *const args) const;

        int processSting(string * values) const;

        ~Interpreter();
        //TODO filemasks
    };
}


#endif //LAB_2_SHELL_INTERPRETER_H
