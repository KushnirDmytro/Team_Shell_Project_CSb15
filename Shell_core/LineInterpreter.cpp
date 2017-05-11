// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Created by d1md1m on 28.03.17.
//

#include "LineInterpreter.h"

//extern map <string, Embedded_func*> embedded_lib;

namespace sh_core {



//=============FUNCTIONS AND STRUCTURES DECLARATIONS=============

    int my_ls(size_t nargs, char **args);
    int my_cd(size_t nargs, char **args);
    int my_pwd(size_t nargs, char **args);
    int my_help(size_t nargs, char **args);
    int my_exit(size_t nargs, char **args);
    int my_sh(size_t nargs, char **args);


    Embedded_func *my_shell_fileinterpreter;
    Embedded_func *my_pwd_obj;
    Embedded_func *my_cd_obj;
    Embedded_func *my_help_obj;
    Embedded_func *my_exit_obj;
    Extern_LS *extern_ls_obj;

//=============FUNCTIONS AND STRUCTURES DECLARATIONS END =============





    LineInterpreter::LineInterpreter() {

        embedded_lib = new map <const string, Embedded_func*> {
                {"cd",   my_cd_obj},
                {"pwd",  my_pwd_obj},
                {"help", my_help_obj},
                {"exit", my_exit_obj},
                {"mysh", my_shell_fileinterpreter},
                {"ls", extern_ls_obj}
        };

        external_lib = new map <const string, const fs::path*>{
        };

    }

    LineInterpreter::~LineInterpreter() {
    }
//TODO filemasks


// launcher for custom modules
    int LineInterpreter::myExternLauncher(char **const args) const{
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
            do {
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


    int LineInterpreter::getNumOfMyBuiltins() const{
        return (int) embedded_lib->size();
    }

    int LineInterpreter::myExecute(const vector<string> *const args) const{

        char **cargs = new char *[args->size() + 1];
        unsigned int args_number = (int) args->size();


        cout << "NUMBER OF ARGS FOUND: " << args_number << endl;
        splitter.convertStrVectorToChars(args, cargs);

        for (int i = 0; i < getNumOfMyBuiltins(); i++) {

            auto search_iter = embedded_lib->find(cargs[0]);
            if (search_iter != embedded_lib->end()) // case when we have such a func in our lib
            {
                //=============CALLING INNER FUNCTION <=======================
                return (search_iter->second)->call(args_number, cargs);
            }
        }

        // CALLING EXTERN FUNC <======================
        return myExternLauncher(cargs);


    }

    int LineInterpreter::processSting(string *values) const{

        const vector<string> args = splitter.mySplitLine(values);

        return myExecute(&args);
    }





//====================BUILT-IN COMMANDS ============

//show current directory
    int my_pwd(size_t nargs, char **args)
    {
        environment->dir_->refreshPath();
        printf("%s", environment->dir_->getActualPath().c_str());
        return 1;
    }

//changes directory
    int my_cd(size_t nargs, char **args)
    {
        if (args[1] == NULL) { //has to have at least one arg
            fprintf(stderr, "my_Shell: expected argument to \"cd\"\n");
        } else {
            string str(args[1]);
            if (str == home_dir_call){
                if ( boost::filesystem::is_directory( environment->user_->getHome_dirrectory() ) ){
                    boost::filesystem::current_path(environment->user_->getHome_dirrectory());
                }
            }
            else if (boost::filesystem::is_directory(args[1])){
                boost::filesystem::current_path(args[1]);
            }
            else{
                //TODO filesystem errors
                //boost::filesystem::filesystem_error()
                perror("\n my_Shell failed to change dir_ \t");
            }

        }
        environment->dir_->setActualPath(boost::filesystem::current_path());
        environment->dir_->setPathWasChanged(true);
        return 1;
    }

//shows help info
    int my_help(size_t nargs, char **args)
    {
        printf("Write command and arguments, if needed, then press 'Enter'\n");
        printf("To get detailed information, write <command option_name> --help or <command option_name> -h:\n");
        printf("List of Commands:\n");
        printf(" 'pwd' -- > returns current directory | arguments <NULL>\n");
        printf(" 'cd' [pass]  -- > changes current execution dirrectory to [pass] 1 argument required\n");
        printf(" 'mysh' <script_filename>.sh can launch *.sh scripts interpreting them\n");

        printf(" In case of external function option_name inputed (some Shell extentions) it will be executed if founded\n");

        printf(" 'exit' terminates executions of Shell program\n");

        printf("\n");

        return 1;
    }

//just exits, that is it
    int my_exit(size_t nargs, char **args)
    {
        printf("my_Shell says GoodBye to You and wishes a good day ;O) ");
        return 0;
    }

/*
    int my_sh(size_t nargs, char **args) {
        cout << "HELLO, FIX ME, YOU DUMBASS!\n" ;
        return 1;
    }
*/


//TODO solve hierarchy problem


//executes in this env external ".msh" files
    //acts as manager for splitting file and feeding lines to LanaInterpreter
int my_sh(size_t nargs, char **args)
{
    if (nargs > 1){
        string file_path;
        file_path = environment->dir_->getActualPath().string();
        file_path.append("/");
        file_path.append(args[1]);
        if (boost::filesystem::is_regular_file(args[1]) ){
            sh_core::utils::FileLaneIterator *iter = new sh_core::utils::FileLaneIterator(args[1]);
            string st;
            cout << "------------FILE READING IN PROCESS------------------" << endl;
            int i =0;
            int status;
            while(iter->fileIsReady()){
                iter->getNextString(&st);
                printf("String #%d red \n", i++);
                cout << st << endl;
                // st.append(" ");
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
        else if (boost::filesystem::is_regular_file(file_path) ){
            utils::FileLaneIterator *iter = new utils::FileLaneIterator(file_path);
            string st;
            cout << "------------FILE READING IN PROCESS------------------" << endl;
            if(iter->fileIsReady()){
                iter->getNextString(&st);
                cout << st << endl;
                delete iter;
                return  interpreter->processSting(&st);
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


//====================BUILT-IN COMMANDS END============




}