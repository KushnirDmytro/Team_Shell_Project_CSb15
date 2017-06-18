//
// Created by d1md1m on 14.05.17.
//


#include <boost/mpl/size_t.hpp>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <boost/filesystem.hpp>


#include <climits>
#include "coreFuncLib.h"
#include "../Env/Env.h"
#include "EmbeddedFunc.h"

//

using std::string;

#define  home_dir_call  "~"
//test
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
            " 'mmysh' <filename> to execurte script file";


//====================BUILT-IN COMMANDS ============

//show current directory
    int myPwd(size_t nargs, char **args) {
        environment->dir_->refreshPath();
        printf("%s\n", environment->dir_->getActualPath().c_str());
        return  EXIT_SUCCESS;
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
        return EXIT_SUCCESS;
    }

//shows help info
    int myHelp(size_t nargs, char **args) {

        printf("Write command and arguments, if needed, then press 'Enter'\n");
        printf("To get detailed information, write <command option_name_> --help or <command option_name_> -h:\n");
        printf("List of Commands:\n");
        printf(" 'mpwd' -- > returns current directory | arguments <NULL>\n");
        printf(" 'mcd' [pass]  -- > changes current execution dirrectory to [pass] 1 argument required\n");
        printf(" 'mmysh' <script_filename>.sh can launch *.msh scripts interpreting them\n");

        printf(" Registered external functions:\n");

        printf(" 'mls' (detailed options ls -h) to list directory contain\n");

        printf(" 'mmv' <filename> <directory>  moves <filename> to <directory> \n");
        printf(" 'mrm' FULL filename removes selected file\n");
        printf(" 'mcp' <from> <to> copies one file  to another\n");

        printf(" In case of external function option_name_ inputed (some Shell extentions) it will be executed if founded\n");

        printf(" 'mexit' terminates executions of Shell program\n");

        printf("\n");

        return EXIT_SUCCESS;
    }

//just exits, that is it
    int myExit(size_t nargs, char **args) {

        if (nargs == 1) {
            printf("my_Shell says GoodBye to You and wishes a good day ;O) ");
            return EXIT_FAILURE;
        }

        if (nargs == 2) {
            size_t numberLen = strlen(args[1]);
            int valueBuf = 0;
            if (numberLen > 4) {
                printf("RETURN CODE NUMBER SEEMS TOO BIG [%s], consider max possible %d\n", args[1], 255);
                return  EXIT_SUCCESS;
            }
            for (int symbolIndx = 0; symbolIndx < numberLen; ++symbolIndx){
                char currentSymbol = args[1][symbolIndx];

                if (!isdigit(currentSymbol)){
                    printf("Unrecognised EXIT return code (suppose positive integer) : [%s]\n", args[1]);
                    return EXIT_SUCCESS;
                }
                valueBuf += atoi(&currentSymbol) * pow(10, numberLen - 1 - symbolIndx) ;
                if (valueBuf > 255) {
                    printf("RETURN CODE NUMBER SEEMS TOO BIG [%s], consider max possible %d\n", args[1], 255);
                    return  EXIT_SUCCESS;
                }
            }
            printf("my_Shell says GoodBye to You and wishes a good day ;O) ");

            return valueBuf;
        }

        else {
            printf("unknown arguments pattern on EXIT [%s]\n  try 'mexit --help' for instructions\n", *args);
            return EXIT_SUCCESS;а
        }

    }

}