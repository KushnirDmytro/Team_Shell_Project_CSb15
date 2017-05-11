//
// Created by d1md1m on 28.03.17.
//

#ifndef LAB_2_SHELL_INTERPRETER_H
#define LAB_2_SHELL_INTERPRETER_H

#include <zconf.h>
#include <wait.h>
#include "iostream"

#include "Utils/LineSplitter.h"
//#include "../Embedded_func.h"
#include "Embedded_func.h"
#include "../Extern_LS.h"

namespace sh_core {


    class LineInterpreter {
    private:
        const utils::LineSplitter splitter;

        const map <const string, Embedded_func*> *embedded_lib;

        const map <const string, const fs::path*> *external_lib;


    public:
        LineInterpreter();

        // launcher for custom modules
        int myExternLauncher(char **args) const;

        int getNumOfMyBuiltins() const;

        int myExecute(const vector<string> *const args) const;

        int processSting(string * values) const;

        ~LineInterpreter();
        //TODO filemasks
    };

    extern LineInterpreter *interpreter;

}


#endif //LAB_2_SHELL_INTERPRETER_H
