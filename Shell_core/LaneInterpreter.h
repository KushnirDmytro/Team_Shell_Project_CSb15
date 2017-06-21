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
#include "Utils/Reducer.h"


// using token = std::pair<string, char>;





namespace sh_core {

    const int STANDART_DESK = -1;
    const int READ_SIDE = 0;
    const int WRITE_SIDE = 1;


    struct chennelDesriptStruct{
        int *indeskPtr = new int(STANDART_DESK);
        int *outdeskPtr = new int(STANDART_DESK);
        int *errdeskPtr = new int(STANDART_DESK);
    };
    using arg_desk_pair = std::pair<vector<string*>*, chennelDesriptStruct*>;





    using std::string;
    using std::vector;

    class LaneInterpreter {
    private:

        utils::LineSplitter *splitter;

        chennelDesriptStruct defaultDescriptors;


        // launcher for custom modules
        //int myExternLauncher(char **args, const char* destination = nullptr) const;


        int myExternLauncherChanneled(char **args, const chennelDesriptStruct* ch_str, const char* destination = nullptr) const;



//        int myExecute(const vector<string>  *const args) const;

        bool doesAllPathesValidAndRefineToAbsolute(vector <fs::path> *args) const;


        int myExecute2(const vector<string> *const args, const chennelDesriptStruct* ch_str) const;
    public:


        coreFuncLib *funcLib;
        int interpretScriptFile(const string *const arg) const;

        LaneInterpreter();

        int processSting(string * values) const;

        ~LaneInterpreter();
        //TODO filemasks via regexp
    };

    extern LaneInterpreter *interpreter;

}


#endif //LAB_2_SHELL_INTERPRETER_H
