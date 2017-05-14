//
// Created by d1md1m on 14.05.17.
//

#include "coreFuncLib.h"
#include "../Env/Env.h"
#include "EmbeddedFunc.h"

using std::string;

#define  home_dir_call  "~"

namespace sh_core{


    string cd_help_msg = "function CD requires argument after it\n"
            "to change directory type in: cd <directory option_name_> \n"
            "<~> = 'HOME' dirrectory if one defined \n"
            "<.> = current dirrectory  \n"
            "<..> = 'parrent directory'  ";
    string pwd_help_msg = "displays fullname of current execution directory";
    string help_help_msg = "just type 'help' to get info about my_Shell help instructions";
    string exit_help_msg = "function 'exit' terminates My_Shell execution";
    string shell_script_interpreter_help_msg = "file interpreter to execute env scripts \n"
            " 'mysh' <filename> to execurte script file";



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
        printf(" 'mysh' <script_filename>.sh can launch *.msh scripts interpreting them\n");

        printf(" Registered external functions:\n");

        printf(" 'ls' (detailed options ls -h) to list directory contain\n");

        printf(" 'mv' <filename> <directory>  moves <filename> to <directory> \n");
        printf(" 'rm' FULL filename removes selected file\n");
        printf(" 'cp' <from> <to> copies one file  to another\n");

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

}