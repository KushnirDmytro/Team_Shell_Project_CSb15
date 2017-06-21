//
// Created by d1md1m on 20.06.17.
//

#include <iostream>
#include "Reducer.h"
#include "Utils/Tokenizer.h"
#include "../Env/Env.h"


#include <cstring>
#include <vector>
#include <string>
#include <boost/filesystem.hpp>
#include <sstream>
#include <iostream>
#include "EmbeddedFunc.h"
#include "coreFuncLib.h"


#include "Utils/Tokenizer.h"
#include "LaneInterpreter.h"

namespace shell_core {

    const bool DO_override_varaibles = true;
    const bool DONT_override_variables = false;

        Reducer::Reducer() {}

        bool Reducer::closesExecutionUnit(const char ch) const {
            return  ( ch=='\n' || ch=='.' || ch=='|' );
        }


        inline void Reducer::create_new_exec_unit(arg_desk_pair* unit_addr){
            unit_addr = new std::pair < std::vector < std::string * > *, chennelDesriptStruct * >;
            unit_addr->first = new std::vector < std::string * >;
            unit_addr->second = new chennelDesriptStruct;
        }

    inline void Reducer::handle_variables_assignment(const token* elem,string* variableNameBuf){
        sh_core::environment->varManager_->
                declareVariableLocally(new string (elem->first),
                                       new string (*variableNameBuf));
        if (RS.waitingForGlobalVar){
            sh_core::environment->varManager_->declareVariableGlobally(new string (elem->first),
                                                                       new string (*variableNameBuf),
                                                                       DO_override_varaibles);
        }
        RS.waitingForGlobalVar = false;
        RS.waitingForVarValue = false;
        *variableNameBuf = "";
    }

        std::vector<arg_desk_pair*>* Reducer::reduce(const vector<token> *toks) {
            std::string variableNameBuf;

            for(auto el: *toks){
                std::cout<<"st[" <<el.first << "]---{" << el.second << "}\n";
            }
            std::vector<arg_desk_pair*> *res = new std::vector<arg_desk_pair* >;

            arg_desk_pair *new_execution_unit = nullptr;
            create_new_exec_unit(new_execution_unit);


            //TODO consider do we need it??
            chennelDesriptStruct* next_operation_descriptor = new chennelDesriptStruct;

            int *pipeSides [2];


            if (!toks->empty()){

            } else{
                perror("Empty taks\n");
                return res;
            }

            auto it = toks->front();

            bool  first_node = true;

            for (auto el : *toks ){

                if (RS.waitingForVarValue) {
                    //TODO make unpacking of element value
                    handle_variables_assignment(&el, &variableNameBuf);
                }

                if (first_node){
                    if (el.second == '\n'){//Need to skip this step, repeated spaces
                        continue;
                    }

                    create_new_exec_unit(new_execution_unit);

                    if (RS.isConveyerOpened){
                        new_execution_unit->second->indeskPtr = pipeSides[READ_SIDE];
                        RS.isConveyerOpened = false;
                    }
                    first_node = false;
                }

                switch (el.second){
                    case 's': {
                        new_execution_unit->first->push_back(new string(el.first));
                        break;
                    }
                    case 'e':{RS.waitingForGlobalVar = true;}
                    case 'v':{
                        RS.waitingForVarValue = true;
                        if (el.first.empty()) { RS.ERROR_STATE = true;
                            perror("UNKNOWN FILENAME\n");
                        }
                        variableNameBuf = el.first;
                        break;
                        sh_core::environment->varManager_->declareVariableLocally(new string (el.first),
                                                                                     new string ("b"));
                    }


                    default: perror("unknown task token\n");
                }

                if ( closesExecutionUnit(el.second) ) {

                    // solving conveyer request

                    if (el.second == '|') {
                        if (!RS.isConveyerOpened)
                            RS.isConveyerOpened = true;
                        else
                            perror("Cant provide Piping, allready bussy\n");
                        pipe(*pipeSides);
                    }

                    if (el.second == '.') {
                        res->push_back(new_execution_unit);
                        std::cout << "met END_OF_TASK\n";
                        break;
                    }

                }




            }

            return res;

        }

        Reducer::~Reducer() {}

    }

