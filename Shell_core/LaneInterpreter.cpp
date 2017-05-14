// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Created by d1md1m on 28.03.17.
//

#include "LaneInterpreter.h"

namespace fs = boost::filesystem;


namespace sh_core {

    extern string shell_script_interpreter_help_msg;
    extern string cd_help_msg;
    extern string pwd_help_msg;
    extern string help_help_msg;
    extern string exit_help_msg;

    //===================DYNAMIC INITIALISATION ======================

    LaneInterpreter::LaneInterpreter() {



        embedded_lib= {
                {"cd", new sh_core::EmbeddedFunc("MY_CD", sh_core::myCd, sh_core::cd_help_msg, false)},
                {"pwd", new sh_core::EmbeddedFunc("MY_PWD", sh_core::myPwd, sh_core::pwd_help_msg)},
                {"help", new sh_core::EmbeddedFunc("MY_HELP", sh_core::myHelp, sh_core::help_help_msg)},
                {"exit", new sh_core::EmbeddedFunc("MY_EXIT", sh_core::myExit, sh_core::exit_help_msg)},
                {"mysh", new sh_core::EmbeddedFunc("MY_shell_script_interpreter",
                                                   sh_core::mySh,
                                                   shell_script_interpreter_help_msg)}
        };

        splitter = new utils::LineSplitter();
    }

    LaneInterpreter::~LaneInterpreter() {
        for(auto i: embedded_lib){
           delete i.second;
        }
        delete splitter;
    }
//TODO filemasks


// launcher for custom modules
    int LaneInterpreter::myExternLauncher(char **const args) const{
        pid_t pid, wpid;
        int status;

        pid = fork();
        if (pid == 0) {
            //  we are in Child process
            if (execv(args[0], args) == -1) {
                perror("my_Shell failed to launch this file");
            }
            exit(EXIT_FAILURE);
        } else if (pid < 0) {
            // Error forking
            perror("my_Shell failed to fork");
        } else {
            // Parent process
            do { //TODO provide test for invalid scenarios of exec (a.e. failed launch file and stay in shell copy)
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

        return 1;
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


    int LaneInterpreter::getNumOfMyBuiltins() const{
        return (int) embedded_lib.size();
    }

    int LaneInterpreter::myExecute(const vector<string> *const args) const{

        char **cargs = new char *[args->size() + 1];
        unsigned int args_number = (int) args->size();


        std::cout << "NUMBER OF ARGS FOUND: " << args_number << std::endl;
        splitter->convertStrVectorToChars(args, cargs);

        for (int i = 0; i < getNumOfMyBuiltins(); i++) {

            auto search_iter = embedded_lib.find(cargs[0]);
            if (search_iter != embedded_lib.end()) // case when we have such a func_ in our lib
            {
                //=============CALLING INNER FUNCTION <=======================
                return (search_iter->second)->call(args_number, cargs);
            }
        }

        // CALLING EXTERN FUNC <======================
        return myExternLauncher(cargs);


    }

    int LaneInterpreter::processSting(string *values) const{

        const vector<string> args = splitter->mySplitLine(values);

        return myExecute(&args);
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
            std::cout << "------------FILE READING IN PROCESS------------------" << std::endl;
            int i =0;
            int status;
            while(iter->fileIsReady()){
                iter->getNextString(&st);
                printf("String #%d red \n", i++);
                std::cout << st << std::endl;
                if (st.length() == 0)
                    continue;
                status = interpreter->processSting(&st);
                if (!status){
                    delete iter;
                    return 0;
                };
            }
            delete iter;
        }
        else if (fs::is_regular_file(file_path) ){
            utils::FileLaneIterator *iter = new utils::FileLaneIterator(file_path);
            string st;
            std::cout << "------------FILE READING IN PROCESS------------------" << std::endl;
            if(iter->fileIsReady()){
                iter->getNextString(&st);
                std::cout << st << std::endl;
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