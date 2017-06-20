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



const int STANDART_DESK = -1;
const int READ_SIDE = 0;
const int WRITE_SIDE = 1;
// using token = std::pair<string, char>;


struct chennelDesriptStruct{
    int indesk = STANDART_DESK;
    int outdesk = STANDART_DESK;
    int errdesk = STANDART_DESK;
};

using arg_desk_pair = std::pair<vector<string*>*, chennelDesriptStruct*>;


namespace sh_core {






    using std::string;
    using std::vector;

    class LaneInterpreter {
    private:

        std::map<string, EmbeddedFunc *> embedded_lib_;

        std::map<string, fs::path*> external_lib_;

        utils::LineSplitter *splitter;

        chennelDesriptStruct defaultDescriptors;


        // launcher for custom modules
        //int myExternLauncher(char **args, const char* destination = nullptr) const;


        int myExternLauncherChanneled(char **args, const chennelDesriptStruct* ch_str, const char* destination = nullptr) const;

        int getNumOfMyBuiltins() const;

//        int myExecute(const vector<string>  *const args) const;

        bool doesAllPathesValidAndRefineToAbsolute(vector <fs::path> *args) const;


        int myExecute2(const vector<string> *const args, const chennelDesriptStruct* ch_str) const;
    public:

        bool hasSuchEmbedded(const string * const arg) const;

        bool hasSuchExternal(const string * const arg) const;

        bool hasMyshExtention(const string *const arg) const;

        int interpretScriptFile(const string *const arg) const;

        LaneInterpreter();

        int processSting(string * values) const;

        ~LaneInterpreter();
        //TODO filemasks
    };

    extern LaneInterpreter *interpreter;

}


#endif //LAB_2_SHELL_INTERPRETER_H
