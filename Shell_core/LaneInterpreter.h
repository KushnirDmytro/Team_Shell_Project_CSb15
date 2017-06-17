//
// Created by d1md1m on 28.03.17.
//

#ifndef LAB_2_SHELL_INTERPRETER_H
#define LAB_2_SHELL_INTERPRETER_H

#include <vector>
#include <string.h>
#include <map>

#include "Utils/LineSplitter.h"
#include "coreFuncLib.h"
#include "EmbeddedFunc.h"


namespace sh_core {

    using std::string;
    using std::vector;

    class LaneInterpreter {
    private:

        std::map<string, EmbeddedFunc *> embedded_lib_;

        std::map<string, fs::path*> external_lib_;

        utils::LineSplitter *splitter;

        // launcher for custom modules
        int myExternLauncher(char **args, const char* dest = nullptr) const;

        int getNumOfMyBuiltins() const;

        int myExecute(const vector<string>  *const args) const;

        bool doesAllPathesValidAndRefineToAbsolute(vector <fs::path> *args) const;

        bool hasSuchEmbedded(const string * const arg) const;

        bool hasSuchExternal(const string * const arg) const;


    public:
        bool hasMyshExtention(const string *const arg) const;

        int interpetScriptFile(const string * const arg) const;

        LaneInterpreter();


        int processSting(string * values) const;

        ~LaneInterpreter();
        //TODO filemasks
    };

    extern LaneInterpreter *interpreter;

}


#endif //LAB_2_SHELL_INTERPRETER_H
