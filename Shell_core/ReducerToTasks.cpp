// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Created by d1md1m on 18.06.17.
//
#include <iostream>


#include <cstring>
#include <vector>
#include <string>
#include <boost/filesystem.hpp>
#include <sstream>
#include <iostream>
#include <fcntl.h>

#include "../Env/Env.h"
#include "EmbeddedFunc.h"
#include "coreFuncLib.h"
#include "Utils/Tokenizer.h"
#include "ReducerToTasks.h"


namespace sh_core {


    const bool DO_override_varaibles = true;
    const bool DONT_override_variables = false;

        ReducerToTasks::ReducerToTasks() {}

        bool ReducerToTasks::last_node_in_task(const char ch) const {
            return  ( ch=='\n' || ch=='.' || ch=='|' );
        }

    inline void ReducerToTasks::handle_start_new_task(const token* elem){

    }
    inline void ReducerToTasks::handle_end_task(const token* elem){

    }
    inline int ReducerToTasks::redirectIt(token* elem, char redirFlag){


        int fileDescriptor = -1;
        // ===== WORKING WITH EXISTING DESCRIPTORS
        if (strcmp(elem->first.c_str(), "1") ==0 )
            fileDescriptor = *(execUnitBuf->second->outdeskPtr);
        if (strcmp(elem->first.c_str(), "2") ==0)
            fileDescriptor = *(execUnitBuf->second->outdeskPtr);


        // ===== CREATING NEW FILEDESCRIPTOR
        const char* filename = elem->first.c_str();
        switch (redirFlag){
            case '<': {
                fileDescriptor = open(filename, O_CREAT |O_RDONLY);
                break;
            }
            case '>': {
                fileDescriptor = open(filename, O_CREAT |O_WRONLY);
                break;
            }
            case '2': {
                fileDescriptor = open(filename, O_CREAT |O_WRONLY);
                break;
            }
            case '&': {
                fileDescriptor = open("dev/null", O_WRONLY);
                break;
            }
            default: {
                fileDescriptor = -1;
                perror("unknown flag for IO redirect\n");
            }
        }


        if (fileDescriptor < 0){
            perror("error opening file");
            return EXIT_FAILURE;
        }


        switch (redirFlag){
            case '<':{
               *(execUnitBuf->second->indeskPtr) = fileDescriptor;
                break;
            }
            case '>':{
                *(execUnitBuf->second->outdeskPtr) = fileDescriptor;
                break;
            }
            case '2':{
                *(execUnitBuf->second->errdeskPtr) = fileDescriptor;
                break;
            }
            case '&':{
                *(execUnitBuf->second->outdeskPtr) = fileDescriptor;
                *(execUnitBuf->second->errdeskPtr) = fileDescriptor;
                break;
            }
            default:{
                RS.ERROR_STATE = true;
                perror("error redirecting, inappropriate stream flag");
            }
        }
        return EXIT_SUCCESS;
    }


        inline void ReducerToTasks::create_new_exec_unit(arg_desk_pair** exec_unit){
            *exec_unit = new std::pair < std::vector < std::string * > *, execInformation * >;
            (*exec_unit)->first = new std::vector < std::string * >;
            (*exec_unit)->second = new execInformation;
        }

    inline void ReducerToTasks::handle_variables_assignment(const token* elem,string* variableNameBuf){
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




        std::vector<arg_desk_pair*>* ReducerToTasks::reduce(const vector<token> *toks) {
            // creating data structures and allocating buffers
            std::string variableNameBuf;
            for(auto el: *toks){
                std::cout<<"st[" <<el.first << "]---{" << el.second << "}\n";
            }
            std::vector<arg_desk_pair*> *res = new std::vector<arg_desk_pair* >;

         //   create_new_exec_unit(execUnitBuf);

            // TODO consider do we need it??
            // execInformation* next_operation_descriptor = new execInformation;
            int *pipeSides [2];
            if (toks->empty()){
                perror("Empty taks\n");
                return res;
            }
            RS.firstNodeInTask = true;


// ==================== PREFORMING INTERPRETING STEP ==========================
            for (auto el : *toks ){

    // ===================== CHECKING FOR SPECIAL STATES OF INTERPRETER==============

                if(RS.nextFilenameIsDescriptor){
                    if( el.second != 'f'){
                        printf("Waiting for filedeskriptor, got [%c]\n attempting to use as file", el.second);
                        el.second = 'f'; //risky but may work with some notations
                    }
                }

                if (RS.waitingForVarValue) {
                    //TODO make unpacking of element value
                    handle_variables_assignment(&el, &variableNameBuf);
                }

                if (RS.firstNodeInTask){
                    if (el.second == '\n'){//Need to skip this step, repeated spaces
                        continue;
                    }
                    create_new_exec_unit(&execUnitBuf);
                    if (RS.isConveyerOpened){
                        execUnitBuf->second->indeskPtr = pipeSides[READ_SIDE];
                        RS.isConveyerOpened = false;
                    }
                    RS.firstNodeInTask = false;
                }
                else if (last_node_in_task(el.second) ) {
                    // solving conveyer request
                    if (el.second == '|') {
                        if (!RS.isConveyerOpened) {
                            execUnitBuf->second->outdeskPtr = pipeSides[WRITE_SIDE];
                            RS.isConveyerOpened = true;
                        }
                        else
                            perror("Cant provide Piping, allready bussy\n");
                        pipe(*pipeSides);
                    }
                    if (el.second == '.') {
                        std::cout << "met END_OF_TASK\n";
                        res->push_back(execUnitBuf);
                    }

                    RS.firstNodeInTask = true;
                    printResState(res);
                    continue;
                }

                // ===================== *END* CHECKING FOR SPECIAL STATES OF INTERPRETER==============

                switch (el.second){
                    case 's': {
                        execUnitBuf->first->push_back(new string(el.first));
                        break;
                    }
                    case 'i': {execUnitBuf->second->exec_mode = EMBEDDED;
                        break;}
                    case 'o': {execUnitBuf->second->exec_mode = EXTERNAL;
                        break;}
                    case '#': {execUnitBuf->second->exec_mode = NOT_EXECUTABLE;
                        break;}
                    case 'm': {execUnitBuf->second->exec_mode = MSH_FILE;
                        break;}
                    case 'e':{RS.waitingForGlobalVar = true;} //no break intentionally
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
                    case 'E':{

                    }
                    case 'V':{

                    }
                    case '<':{RS.changeIn = true;
                        RS.nextFilenameIsDescriptor =  true;
                        outputRedirectBuf = '<';
                        break;}
                    case '>':{RS.changeOut = true;
                        RS.nextFilenameIsDescriptor =  true;
                        outputRedirectBuf = '>';
                        break;}
                    case '2':{RS.changeErr = true;
                        RS.nextFilenameIsDescriptor =  true;
                        outputRedirectBuf = '2';
                        break;}
                    case '&':{RS.changeIn = true;
                        RS.nextFilenameIsDescriptor =  true;
                        outputRedirectBuf = '&';
                        break;}
                    case 'f':{
                        if (RS.nextFilenameIsDescriptor){
                            RS.nextFilenameIsDescriptor = false;
                            if(redirectIt(&el, outputRedirectBuf)){
                                perror("Problem redirrecting to file");
                                RS.ERROR_STATE = true;
                            }

                        }
                    }
                    case '%':{}
                    case '\'':{}
                    case '"':{}
                    case '`':{ perror("DUDE, MAKE ME!!!");
                        break;}
                    default: perror("unknown task token\n");
                }



                if (RS.ERROR_STATE) {
                    perror("Error state detected\n");
                    return res;
                }

                if(!el.first.empty())
                    execUnitBuf->first->push_back(&el.first);

            }

            return res;
        }

    void ReducerToTasks::printResState(std::vector<arg_desk_pair*> *res) {
        for(auto i: *res){
            for(auto j: *(i->first))
                printf("[%s] >WITH> ", j->c_str());
            printf("ex[%d], in[%d] out[%d] err[%d]\n",
                   i->second->exec_mode,
                   *(i->second->indeskPtr),
                   *(i->second->outdeskPtr),
                   *(i->second->errdeskPtr)
            );
        }
    }


        ReducerToTasks::~ReducerToTasks() {}
    }


