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
    inline int ReducerToTasks::redirectIO(token *elem, char redirFlag){

        int fileDescriptor = -1;
        // ===== WORKING WITH EXISTING DESCRIPTORS
        if (strcmp(elem->first.c_str(), "1") ==0 )
            fileDescriptor = *(execUnitBuf.second.outdeskPtr);
        if (strcmp(elem->first.c_str(), "2") ==0)
            fileDescriptor = *(execUnitBuf.second.outdeskPtr);


        // ===== CREATING NEW FILEDESCRIPTOR
        const char* filename = elem->first.c_str();
        switch (redirFlag){
            case '<': {
                fileDescriptor = open(filename, O_CREAT |O_RDONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                                                                   S_IROTH |  S_IWOTH);
                break;
            }
            case '>': {
                fileDescriptor = open(filename, O_CREAT |O_WRONLY,S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                                                                  S_IROTH |  S_IWOTH);
                break;
            }
            case '2': {
                fileDescriptor = open(filename, O_CREAT |O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                                                                   S_IROTH |  S_IWOTH);
                break;
            }
            case '&': {
                fileDescriptor = open("/dev/null", O_WRONLY);
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
               *(execUnitBuf.second.indeskPtr) = fileDescriptor;
                break;
            }
            case '>':{
                *(execUnitBuf.second.outdeskPtr) = fileDescriptor;
                break;
            }
            case '2':{
                *(execUnitBuf.second.errdeskPtr) = fileDescriptor;
                break;
            }
            case '&':{
                *(execUnitBuf.second.outdeskPtr) = fileDescriptor;
                *(execUnitBuf.second.errdeskPtr) = fileDescriptor;
                break;
            }
            default:{
                RS.ERROR_STATE = true;
                perror("error redirecting, inappropriate stream flag");
            }
        }
        return EXIT_SUCCESS;
    }

    inline int ReducerToTasks::handleVariableCall(const token* elem){
        printf("Env Var state before CALL:\n");
        environment->varManager_->show_local_variables();


        // controll vector opera

        const std::string *varNamePtr = &elem->first;
        const std::string *varValuePtr = nullptr;
        if (environment->varManager_->doesVariableDeclaredLocally(varNamePtr)){
            varValuePtr = environment->varManager_->getLocalVar(varNamePtr);
        }else if(environment->varManager_->doesVariableDeclaredGlobaly(varNamePtr)){
            varValuePtr = environment->varManager_->getGlobalVar(varNamePtr);
            printf("GLOBALL ");
        } else{
            perror("UNKNOWN VAR_NAME");
            return EXIT_FAILURE;
        }
        execUnitBuf.first.push_back(*varValuePtr); // copy value to vector

        printf("varValue got [%s]\n", (*varValuePtr).c_str());

        return EXIT_SUCCESS;
    }


        inline void ReducerToTasks::create_new_exec_unit(arg_desk_pair* exec_unit){
            *exec_unit = *(new std::pair<vector<string>, execInformation>);
//            (*exec_unit)->first = new std::vector < std::string * >;
//            (*exec_unit)->second = new execInformation;
        }


    int ReducerToTasks::justSubstituteVars(const string *args){

        string argsBuf = *args;

        std::stringstream ss;

        size_t  pos = args->find_first_of('$');
        while (pos != std::string::npos){

        }

//                        res->emplace_back(vector_buf);

        toker.setIgnoreDelimiters(false);
        return EXIT_SUCCESS;
    }



    inline void ReducerToTasks::handle_variables_assignment(const token* elem,string* variableNamePtr){
        printf("Env Var state before:\n");
        environment->varManager_->show_local_variables();
//        std::string valueToAssign = elem->first;
//
//        std::vector<arg_desk_pair>* vector_buf; = reduce(toksBuf);
//        result_buf->insert(result_buf->end(), vector_buf->begin(), vector_buf->end());

//
//        if (elem->second == '"'){ // need to substitute
//            justSubstituteVars(&valueToAssign, vector_buf);
//            valueToAssign; // concat to string
//
//        }

        if (RS.waitingForGlobalVar){
            sh_core::environment->varManager_->declareVariableGlobally(new string (*variableNamePtr),
                                                                       new string (elem->first),
                                                                       DO_override_varaibles);
            printf("!GLOBAL! ");

        }
        else {
            sh_core::environment->varManager_->declareVariableLocally(
                    new string(*variableNamePtr),
                    new string(elem->first));
        }
        printf("Varible assigned: N[%s]=>V[%s]\n", (*variableNamePtr).c_str(), elem->first.c_str());
        printf("Env Var state after:\n");
        environment->varManager_->show_local_variables();
        RS.waitingForGlobalVar = false;
        RS.waitingForVarValue = false;
        *variableNamePtr = "";
    }




        std::vector<arg_desk_pair>* ReducerToTasks::reduce(const vector<token> *toks) {
            // creating data structures and allocating buffers
            std::string variableNameBuf;
            for(auto el: *toks){
                std::cout<<"st[" <<el.first << "]---{" << el.second << "}\n";
            }
            std::vector<arg_desk_pair> *res = new std::vector<arg_desk_pair>;

         //   create_new_exec_unit(execUnitBuf);

            // TODO consider do we need it??
            // execInformation* next_operation_descriptor = new execInformation;
            int pipeSides [2];
//            pipeSides[0] = new int(-1);
//            pipeSides[1] = new int(-1);


            if (toks->empty()){
                perror("Empty taks\n");
                return returnResult(res);
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
                    continue;
                }

                if (RS.firstNodeInTask){
                    if (el.second == '\n'){//Need to skip this step, repeated spaces
                        continue;
                    }
                    create_new_exec_unit(&execUnitBuf);
                    if (RS.isConveyerOpened){
                        *(execUnitBuf.second.indeskPtr) = pipeSides[READ_SIDE];
                        RS.isConveyerOpened = false;
                    }
                    RS.firstNodeInTask = false;
                }
                else if (last_node_in_task(el.second) ) {
                    // solving conveyer request
                    if (el.second == '|') {
                        if (!RS.isConveyerOpened) {
                            pipe(pipeSides);
                            *(execUnitBuf.second.outdeskPtr) = pipeSides[WRITE_SIDE];
                            RS.isConveyerOpened = true;
                        }
                        else
                            perror("Cant provide Piping, allready bussy\n");
                        printf("piping from %d -> to %d\n", pipeSides[WRITE_SIDE], pipeSides[READ_SIDE]);
                    }
                    if (el.second == '.') {
                        std::cout << "met END_OF_TASK\n";
                    }

                    res->push_back(execUnitBuf);
                    RS.firstNodeInTask = true;
                    printResState(res);
                    continue;
                }

                // ===================== *END* CHECKING FOR SPECIAL STATES OF INTERPRETER==============

                switch (el.second){
                    case 's': {
                        //execUnitBuf.first.push_back(el.first);
                        break; //just let it get in buf
                    }
                    case 'i': {execUnitBuf.second.exec_mode = EMBEDDED;
                        break;}
                    case 'o': {execUnitBuf.second.exec_mode = EXTERNAL;
                        break;}
                    case '#': {execUnitBuf.second.exec_mode = NOT_EXECUTABLE;
                        break;}
                    case '$':{
                        if (handleVariableCall(&el)){
                            RS.ERROR_STATE = true;
                            perror("variable call fail");
                        }
                        continue; // need not to put name of a variable itself
                    }
                    case 'm': {execUnitBuf.second.exec_mode = MSH_FILE;
                        execUnitBuf.first.push_back( el.first);
                        break;}
                    case 'e':{RS.waitingForGlobalVar = true;} //no break intentionally
                    case 'v':{
                        RS.waitingForVarValue = true;
                        if (el.first.empty()) { RS.ERROR_STATE = true;
                            perror("UNKNOWN FILENAME\n");
                        }
                        execUnitBuf.second.exec_mode = NOT_EXECUTABLE; //this block allready executed
                        variableNameBuf = el.first;
                        continue;
                        sh_core::environment->varManager_->declareVariableLocally(new string (el.first),
                                                                                     new string ("b"));
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
                    case '&':{RS.changeOut = true;
                        RS.changeErr = true;
                        outputRedirectBuf = '&';
                        redirectIO(&el, outputRedirectBuf);
                        break;}
                    case 'f':{
                        if (RS.nextFilenameIsDescriptor){
                            RS.nextFilenameIsDescriptor = false;
                            if(redirectIO(&el, outputRedirectBuf)){
                                perror("Problem redirrecting to file");
                                RS.ERROR_STATE = true;
                            }

                        }
                        break;
                    }
                    case '%':{} //REGEX
                    case '\'':{
                        break;
                    } // doing nothing, just place as it is
                    case '"':{
                        toker.setIgnoreDelimiters(true);
                        const vector<token> *toksBuf = toker.tokenize(&el.first);
                        std::vector<arg_desk_pair>* vector_buf = reduce(toksBuf);
                        res->insert(res->end(), vector_buf->begin(), vector_buf->end());

//                        res->emplace_back(vector_buf);

                        toker.setIgnoreDelimiters(false);
                        continue;
                    } // substitute variables, instead of mutating string place other
                    case '`':{

                        //TODO customize buffer name for each session and delete then
                        el.first = el.first.append(" > tempBufFile 2>&1");

                        const vector<token> *toksBuf = toker.tokenize(&el.first);
                        std::vector<arg_desk_pair>* vector_buf = reduce(toksBuf);

                        //TODO execute it, read from this file to var

                        perror("DUDE, MAKE ME!!!"); //execute as a task then result in value
                        break;}
                    case '!':{
                        perror("Token reports error state\n");
                        RS.ERROR_STATE = true;
                    }
                    default: perror("unknown task token\n");
                }



                if (RS.ERROR_STATE) {
                    perror("Error state detected\n");
                    return returnResult(res);
                }

                if(!el.first.empty()) {
                    printf("BUF_SIZE = %u\n", (unsigned int) execUnitBuf.first.size());
                    printf("Pushing [%s]~{%c} into [",el.first.c_str(), el.second);
                    for(string i:execUnitBuf.first){
                        printf(" {%s}",i.c_str());
                    }
                    printf("]\n");


                    execUnitBuf.first.push_back(el.first);


                    printf("Now buf is [");
                    for(string i:execUnitBuf.first){
                        printf(" {%s}",i.c_str());
                    }
                    printf("]\n");

                }
                else {
                    printf("special TOK: [%c]\n", el.second);
                }

            }
            return returnResult(res);
        }

    void ReducerToTasks::printResState(std::vector<arg_desk_pair> *res)const {
        printf("REZ SIZE{%d}", (int)res->size());
        for(auto i: *res){
            for(auto j: i.first)
                printf("[%s] >WITH> ", j.c_str());
            printf("ex[%d], in[%d] out[%d] err[%d]\n",
                   i.second.exec_mode,
                   *(i.second.indeskPtr),
                   *(i.second.outdeskPtr),
                   *(i.second.errdeskPtr)
            );
        }
    }

    taskList* ReducerToTasks::returnResult(taskList* ret)const{
        printResState(ret);
        return ret;
    }
        ReducerToTasks::~ReducerToTasks() {}
    }


