// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Created by d1md1m on 28.03.17.
//

#include "LaneInterpreter.h"

namespace fs = boost::filesystem;

/*
 *

namespace ext{
    int my_ls(size_t nargs, char **args);
   // ext::ExternLS *extern_ls_obj;

    //TODO solve it when splitting onto several EXEs
//just activator-function
    int my_ls(size_t nargs, char **argv) {
        return sh_core::interpreter->extern_ls_obj->my_ls_inner(nargs, argv);
        //my_ls_inner(nargs_, argv);
    }

}
*/

namespace sh_core {
//=============FUNCTIONS AND STRUCTURES DECLARATIONS=============

    int myCd(size_t nargs, char **args);

    int myPwd(size_t nargs, char **args);

    int myHelp(size_t nargs, char **args);

    int myExit(size_t nargs, char **args);

    int mySh(size_t nargs, char **args);
}

namespace sh_core {

    //===================DYNAMIC INITIALISATION ======================

    LaneInterpreter::LaneInterpreter() {

        string cd_help_msg = "to change directory type in: cd <directory option_name_> \n"
                "<~> = 'HOME' dirrectory if one defined \n"
                "<.> = current dirrectory  \n"
                "<..> = 'parrent directory'  ";
        string pwd_help_msg = "displays fullname of current execution directory";
        string help_help_msg = "just type 'help' to get info about my_Shell help instructions";
        string exit_help_msg = "function 'exit' terminates My_Shell execution";
        string shell_script_interpreter_help_msg = "file interpreter to execute env scripts \n"
                " 'mysh' <filename> to execurte script file";

        //=========================ATTENTION!!!==========++++++!!!!!

       // extern_ls_obj = new ext::ExternLS("MY_EXT_LS", ext::my_ls , cd_help_msg);

        embedded_lib= {
                {"cd", new sh_core::EmbeddedFunc("MY_CD", sh_core::myCd, cd_help_msg)},
                {"pwd", new sh_core::EmbeddedFunc("MY_PWD", sh_core::myPwd, pwd_help_msg)},
                {"help", new sh_core::EmbeddedFunc("MY_HELP", sh_core::myHelp, help_help_msg)},
                {"exit", new sh_core::EmbeddedFunc("MY_EXIT", sh_core::myExit, exit_help_msg)},
                {"mysh", new sh_core::EmbeddedFunc("MY_shell_script_interpreter",
                                                   sh_core::mySh,
                                                   shell_script_interpreter_help_msg)},
               // {"ls", extern_ls_obj}
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





//====================BUILT-IN COMMANDS ============

//show current directory
    int myPwd(size_t nargs, char **args) {
        environment->dir_->refreshPath();
        printf("%s", environment->dir_->getActualPath().c_str());
        return 1;
    }

//changes directory
    int myCd(size_t nargs, char **args) {
        if (args[1] == NULL) { //has to have at least one arg
            fprintf(stderr, "my_Shell: expected argument to \"cd\"\n");

        } else {
            string str(args[1]);

            if (str == home_dir_call) {

                if (fs::is_directory(environment->user_->getHome_dirrectory())) {
                    fs::current_path(environment->user_->getHome_dirrectory());
                }
            }
            else

            if (fs::is_directory(args[1])) {
                fs::current_path(args[1]);
            }
            else {
                //TODO filesystem errors
                //boost::filesystem::filesystem_error()
                perror("\n my_Shell failed to change dir_ \t");
            }

        }

        environment->dir_->setActualPath(fs::current_path());
        environment->dir_->setPathWasChanged(true);
        return 1;
    }

//shows help info
    int myHelp(size_t nargs, char **args) {

        printf("Write command and arguments, if needed, then press 'Enter'\n");
        printf("To get detailed information, write <command option_name_> --help or <command option_name_> -h:\n");
        printf("List of Commands:\n");
        printf(" 'pwd' -- > returns current directory | arguments <NULL>\n");
        printf(" 'cd' [pass]  -- > changes current execution dirrectory to [pass] 1 argument required\n");
        printf(" 'mysh' <script_filename>.sh can launch *.sh scripts interpreting them\n");

        printf(" In case of external function option_name_ inputed (some Shell extentions) it will be executed if founded\n");

        printf(" 'exit' terminates executions of Shell program\n");

        printf("\n");

        return 1;
    }

//just exits, that is it
    int myExit(size_t nargs, char **args) {
        printf("my_Shell says GoodBye to You and wishes a good day ;O) ");
        return 0;
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