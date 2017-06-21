//
// Created by d1md1m on 20.06.17.
//

#ifndef MSHELL_REDUCER_H
#define MSHELL_REDUCER_H

#include <string>
#include <vector>
#include "Tokenizer.h"

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


     namespace utils {






         class Reducer {
         public:
             Reducer();
             ~Reducer();

             std::vector<arg_desk_pair*>* reduce(const vector<token>* toks);

         private:

             bool isConveyerOpened = false;


             bool closesExecutionUnit(const char ch) const;

         };
     }
 }


#endif //MSHELL_REDUCER_H
