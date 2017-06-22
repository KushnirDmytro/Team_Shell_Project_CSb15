// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Created by d1md1m on 28.03.17.
//


#include <wait.h>
#include <iostream>
#include <vector>
#include <string.h>

#include <boost/filesystem.hpp>
#include "Utils/LineSplitter.h"
#include "coreFuncLib.h"
#include "Utils/FileLaneIterator.h"
#include "ReducerToTasks.h"


#include "LaneInterpreter.h"

namespace fs = boost::filesystem;

namespace sh_core {


    extern string shell_script_interpreter_help_msg;
    extern string cd_help_msg;
    extern string pwd_help_msg;
    extern string help_help_msg;
    extern string exit_help_msg;
    extern string echo_help_msg;

    //===================DYNAMIC INITIALISATION ======================

    inline fs::path* absPathTo(string arg){
        return  new fs::path (fs::path(environment->dir_->getActualPath()) / fs::path(arg) );
    }

    LaneInterpreter::LaneInterpreter() {
        funcLib = new coreFuncLib();

        splitter = new utils::LineSplitter();
    }
//tests
    LaneInterpreter::~LaneInterpreter() {
        delete(funcLib);

        delete splitter;
    }
//TODO filemasks






    int configureIOChannales(const chennelDesriptStruct *ch_str){

        if( *ch_str->indeskPtr != STANDART_DESK )
        {
            close (*ch_str->outdeskPtr);  /* first close the write end of the pipe */
            if(dup2(*(ch_str->indeskPtr), STDIN_FILENO) == -1){ /* stdin == read end of the pipe (side of the pipe where data is read)*/
                perror( "dup2 failed on STD IN" );
                return EXIT_FAILURE;
            }
            close(*(ch_str->indeskPtr));

        }
        if(*ch_str->outdeskPtr != STANDART_DESK) /* stdout == write end of the pipe */
        {

            close(*ch_str->indeskPtr); /* first close the read end of the pipe */
            if(dup2(*ch_str->outdeskPtr, STDOUT_FILENO) == -1){ /* stdout == write end of the pipe (side of the pipe in which data is written)*/
                perror( "dup2 failed of STD OUT" );
                return EXIT_FAILURE;
            }
            close(*ch_str->outdeskPtr);

        }

        if(*ch_str->errdeskPtr != STANDART_DESK) /* stdout == write end of the pipe */
        {
            //close(p[0]); /* first close the read end of the pipe */
            if(dup2(*ch_str->errdeskPtr, STDERR_FILENO) == -1){ /* stdERR == write end of the pipe (side of the pipe in which errordata is written)*/
                perror( "dup2 failed of STD ERR" );
                return EXIT_FAILURE;
            }
            close(*ch_str->errdeskPtr);

        }
        return EXIT_SUCCESS;
    }

    inline void closeParrentDescriptors(const chennelDesriptStruct* ch_str ){
        if (*ch_str->indeskPtr != STANDART_DESK)
            close(*ch_str->indeskPtr);
        if (*ch_str->outdeskPtr != STANDART_DESK)
            close(*ch_str->outdeskPtr);
        if (*ch_str->errdeskPtr != STANDART_DESK)
            close(*ch_str->errdeskPtr);
    }

// launcher for custom modules
    int LaneInterpreter::myExternLauncherChanneled(char **const args, const chennelDesriptStruct* ch_str , const char* dest) const{
        if (dest == nullptr)
            dest = args[0];

        pid_t pid, wpid;
        int status;


        pid = fork();
        if (pid == 0) {


            //  we are in Child process

            if (configureIOChannales(ch_str)){ // <= channeling here
                return EXIT_FAILURE;
            }



            if (execvp(dest, args) == -1) {
                perror("my_Shell failed to launch this file");
            }
            exit(EXIT_SUCCESS);
        } else if (pid < 0) {
            // Error forking
            perror("my_Shell failed to fork");
        } else {
            // Parent process
           closeParrentDescriptors(ch_str);


            do { //TODO provide tests for invalid scenarios of exec (a.e. failed launch file and stay in shell copy)
                wpid = waitpid(pid, &status, WUNTRACED);
                /*
                 * WUNTRACED
                 * also return if a child has stopped (but not traced via ptrace(2)).
                 * Status for traced children which have stopped is provided even if this option is not specified.
                 * WIFEXITED(status)
                 * returns true if the child terminated normally, that is, by calling exit(3) or _exit(2),
                 * or by returning from main().
                 * WIFSIGNALED(status)
                 * returns true if the child process was terminated by a signal.
                 */
            } while (!WIFEXITED(status) &&
                     !WIFSIGNALED(status)); //checks for valid scenarios of exiting (we borrowed it)
        }

        return EXIT_SUCCESS;
    }



    bool LaneInterpreter::doesAllPathesValidAndRefineToAbsolute(vector <fs::path> *args) const{
        fs::path full_path;
        for (auto iter: *args) {
            if (fs::exists(iter))
                continue;
            else {
                full_path = sh_core::environment->dir_->getActualPath();
                full_path /= iter;
                if (fs::exists(full_path)) {
                    iter = fs::path(full_path);
                    continue;
                }
                else{
                    printf("Failed verification of arguments on: %s \n", iter.string().c_str());
                    return false;
                }
            }
        }
        return true;
    }




    int LaneInterpreter::interpretScriptFile(const string *const scriptName) const{
        const int supposedArgsNumber = 2;
        vector<string> *argsBuf = new vector<string>;
        argsBuf->push_back("mysh");
        argsBuf->push_back(*scriptName);
        char **charArgsBuf = new char *[argsBuf[0].size() + (*scriptName).length() + 1];
        sh_core::interpreter->splitter->convertStrVectorToChars(argsBuf, charArgsBuf);
        int result = funcLib->embedded_lib_.at("mmysh")->call(supposedArgsNumber, charArgsBuf);
        return result;
    };


    int LaneInterpreter::myExecute2(const vector<string> *const args, const chennelDesriptStruct* ch_str) const{

        char **cargs = new char *[args->size() + 1];
        size_t args_number = args->size();

        // we'll need this debugging part 2
        //std::cout << "NUMBER OF ARGS FOUND: " << args_number << std::endl;
        splitter->convertStrVectorToChars(args, cargs);

        string possibleFunc = string(cargs[0]);


        int result;
        if (funcLib->hasSuchEmbedded(&possibleFunc)) // case when we have such a func_ in our lib
        {
            //=============CALLING INNER FUNCTION <=======================
            if (configureIOChannales(ch_str)){
                perror("failed on channel switch");
                return EXIT_FAILURE;
            }
            result = funcLib->embedded_lib_.at(possibleFunc)->call(args_number, cargs);
        }
        else {
            if (funcLib->hasMyshExtention(&possibleFunc)) {

                if (configureIOChannales(ch_str)){
                    perror("failed on channal switch");
                    return EXIT_FAILURE;
                }
                result = interpretScriptFile(&possibleFunc);

            }
            else{ // CALLING EXTERN FUNC <======================
                if (funcLib->hasSuchExternal(&possibleFunc)) {
                    result = myExternLauncherChanneled(cargs, ch_str, funcLib->external_lib_.at(possibleFunc)->string().c_str());
                    //using full pathname instead of just local one
                } else
                    result = myExternLauncherChanneled(cargs, ch_str );
            }
        }

        delete cargs;
        return result;
    }


    int LaneInterpreter::processSting(string *values) const{

        ReducerToTasks *r2t = new ReducerToTasks();

// ==================== testing module ===============
        int file_desk[2];

        pipe(file_desk);

        chennelDesriptStruct *chdOUT_LS = new chennelDesriptStruct();
        *(chdOUT_LS->outdeskPtr) = file_desk[WRITE_SIDE];
        chennelDesriptStruct *chdIN_WC = new chennelDesriptStruct();
        *(chdIN_WC->indeskPtr) = file_desk[READ_SIDE];

        //arg_desk_pair *forLSout1 = new arg_desk_pair;

        vector<string> *argvecLS = new vector<string>;
        argvecLS->push_back(string("ls"));

        vector<string> *argvecWC = new vector<string>;
        argvecWC->push_back(string("wc"));



        int test_result_ls = myExecute2(argvecLS, chdOUT_LS);

        int test_result_wc = myExecute2(argvecWC, chdIN_WC);
        std::cout << "test_result_ls :"<< test_result_ls<< std::endl;

        std::cout << "test_result_wc :"<< test_result_wc<< std::endl;
        utils::Tokenizer *toker = new utils::Tokenizer();



// ==================== testing module ===============

        // TODO change for tokenizer
        const vector<string> args = splitter->mySplitLine(values);

        // TODO check this suspicious place for bugs

        return myExecute2(&args, &defaultDescriptors);
    }


//executes in this env external ".msh" files
int mySh(size_t nargs, char **args)
{
    if (nargs > 1){
        string file_path;

        file_path = environment->dir_->getActualPath().string();
        file_path.append("/");
        file_path.append(args[1]);

        if (fs::is_regular_file(args[1]) ){
            sh_core::utils::FileLaneIterator *iter = new sh_core::utils::FileLaneIterator(args[1]);
            string st;
            //std::cout << "------------FILE READING IN PROCESS------------------" << std::endl;
            int i =0;
            int status;
            while(iter->fileIsReady()){
                iter->getNextString(&st);
                //printf("String #%d red \n", i++);
                std::cout << st << std::endl; //left only this echo
                if (st.length() == 0)
                    continue;
                status = interpreter->processSting(&st);
                if (status){
                    delete iter;
                    return status;
                };
            }
            delete iter;
        }
        else if (fs::is_regular_file(file_path) ){
            utils::FileLaneIterator *iter = new utils::FileLaneIterator(file_path);
            string st;
            //std::cout << "------------FILE READING IN PROCESS------------------" << std::endl;
            if(iter->fileIsReady()){
                iter->getNextString(&st);
                //std::cout << st << std::endl;
                delete iter;
                return interpreter->processSting(&st);
            }
            delete iter;
        }
        else{
            perror(args[1]);
            perror(file_path.c_str());
        }
    }
    return 1;
}





}