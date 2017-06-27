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
#include "ReducerToTasks.h"


// using token = std::pair<string, char>;





namespace sh_core {

    extern const int STANDART_UNDEF_DESK;// = -1;
    extern const int STANDART_IN_DESK;// = -1;
    extern const int STANDART_OUT_DESK;// = -1;
    extern const int STANDART_ERR_DESK;// = -1;

    extern  const int READ_SIDE;// = 0;
    extern  const int WRITE_SIDE;// = 1;



    struct chennelDesriptStruct{

        int *indeskPtr = new int(STANDART_UNDEF_DESK);
        int *outdeskPtr = new int(STANDART_UNDEF_DESK);
        int *errdeskPtr = new int(STANDART_UNDEF_DESK);
    };
 //   using arg_desk_pair = std::pair<vector<string*>*, chennelDesriptStruct*>;





    using std::string;
    using std::vector;

    class LaneInterpreter {
    private:
        int myExternLauncherChanneled3(char **const args, const execInformation* ch_str , const char* dest);
        class ManagerIODescriptors{

        public:
            ManagerIODescriptors(){
                deskMemmory[0] = deskMemmory[1] = deskMemmory[2] = false;
            };



            execInformation storedDescriptor;
            bool deskMemmory[3];
            int configureIOChannales3(const execInformation *ch_str);
            // decoding desc # formula: (- (desk+3))
            int deallocateSavedDeskriptor(int descNumber);
            int saveDescriptor(int descCode);
            int restoreDeskriptors(
           //         int descCode
            );

        };

        ManagerIODescriptors descriptorManager_;

        utils::LineSplitter *splitter;

        chennelDesriptStruct defaultDescriptors;

        execInformation *hostExecInfo;


        // launcher for custom modules
        //int myExternLauncher(char **args, const char* destination = nullptr) const;


        int myExternLauncherChanneled(char **args, const chennelDesriptStruct* ch_str, const char* destination = nullptr) const;



//        int myExecute(const vector<string>  *const args) const;

        bool doesAllPathesValidAndRefineToAbsolute(vector <fs::path> *args) const;


        int myExecute2(const vector<string> *const args, const chennelDesriptStruct* ch_str) const;

        int myExecute3(const vector<string> *const args, const execInformation* ch_str) ;
    public:


        coreFuncLib *funcLib;
        int interpretScriptFile(const string *const arg) const;

        LaneInterpreter();

        int processSting(string * values) ;

        ~LaneInterpreter();
        //TODO filemasks via regexp
    };

    extern LaneInterpreter *interpreter;

}


#endif //LAB_2_SHELL_INTERPRETER_H
