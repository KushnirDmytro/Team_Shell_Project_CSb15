//
// Created by d1md1m on 20.06.17.
//

#ifndef MSHELL_REDUCER_H
#define MSHELL_REDUCER_H

#include <string>
#include <vector>
#include "Utils/Tokenizer.h"
#include "../Env/Env.h"

namespace shell_core {



     const int STANDART_DESK = -1;
     const int READ_SIDE = 0;
     const int WRITE_SIDE = 1;


     struct chennelDesriptStruct{
         int *indeskPtr = new int(STANDART_DESK);
         int *outdeskPtr = new int(STANDART_DESK);
         int *errdeskPtr = new int(STANDART_DESK);
     };
     using arg_desk_pair = std::pair<vector<string*>*, chennelDesriptStruct*>;





    extern env::Env *environment;


         class Reducer {
         public:
             Reducer();
             ~Reducer();

             std::vector<arg_desk_pair*>* reduce(const vector<token>* toks);

         private:
             struct reducerState{
                 bool isConveyerOpened = false;
                 bool isFirstElement = true;
                 bool waitingForVarValue = false;
                 bool waitingForGlobalVar=false;
                 bool ERROR_STATE = false;
             };
             reducerState RS;
             void create_new_exec_unit(arg_desk_pair* unit_addr);
             void handle_variables_assignment(const token* elem, string* variableNameBuf);

             bool closesExecutionUnit(const char ch) const;

         };
     }



#endif //MSHELL_REDUCER_H
