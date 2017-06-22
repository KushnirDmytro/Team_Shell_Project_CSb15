//
// Created by d1md1m on 20.06.17.
//

#ifndef MSHELL_REDUCER_H
#define MSHELL_REDUCER_H

#include <string>
#include <vector>
#include "Utils/Tokenizer.h"
#include "../Env/Env.h"

namespace sh_core {

    enum execution_mode{EMBEDDED, EXTERNAL, MSH_FILE, UNIVERSAL, NOT_EXECUTABLE};

     const int STANDART_DESK = -1;
     const int READ_SIDE = 0;
     const int WRITE_SIDE = 1;


     struct execInformation{
         int *indeskPtr = new int(STANDART_DESK);
         int *outdeskPtr = new int(STANDART_DESK);
         int *errdeskPtr = new int(STANDART_DESK);
         execution_mode exec_mode = UNIVERSAL;
     };
     using arg_desk_pair = std::pair<vector<string>, execInformation>;

    using token = std::pair<string, char>;

    extern env::Env *environment;

         class ReducerToTasks {
         public:
             ReducerToTasks();
             ~ReducerToTasks();

             std::vector<arg_desk_pair>* reduce(const vector<token>* toks);

         private:
             struct reducerState{
                 bool isConveyerOpened = false;
                 bool isFirstElement = true;
                 bool waitingForVarValue = false;
                 bool waitingForGlobalVar=false;
                 bool firstNodeInTask=false;
                 bool lastNodeInTask=false;
                 bool ERROR_STATE = false;
                 bool changeIn = false;
                 bool changeOut = false;
                 bool changeErr = false;
                 bool nextFilenameIsDescriptor = false;
             };
             reducerState RS;
             char outputRedirectBuf = '\0';
             arg_desk_pair execUnitBuf;

             void create_new_exec_unit(arg_desk_pair* exec_unit);
             void handle_variables_assignment(const token* elem, string* variableNameBuf);
             void handle_start_new_task(const token* elem);
             void handle_end_task(const token* elem);
             int redirectIt(token* elem, char redirFlag);
             void printResState(std::vector<arg_desk_pair> *res);
             bool last_node_in_task(const char ch) const;

         };
     }

#endif //MSHELL_REDUCER_H