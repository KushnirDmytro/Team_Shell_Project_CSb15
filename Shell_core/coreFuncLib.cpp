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
#include <uchar.h>
#include "coreFuncLib.h"
#include "../Env/Env.h"
#include "EmbeddedFunc.h"

//

using std::string;

#define  home_dir_call  "~"
//tests
namespace sh_core{

    string cd_help_msg = "function CD requires argument after it\n"
            "to change directory type in: cd <directory option_name_> \n"
            "<~> = 'HOME' dirrectory if one defined \n"
            "<.> = current dirrectory  \n"
            "<..> = 'parrent directory'  ";
    string pwd_help_msg = "displays fullname of current execution directory";
    string help_help_msg = "just type 'help' to get info about my_Shell help instructions";
    string exit_help_msg = "function 'exit' terminates My_Shell execution";
    string echo_help_msg = "retrns value of next argument from input\n";
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
        printf(" 'mecho' <argument> outputs value of argument given\n");

        printf(" Registered external functions:\n");

        printf(" 'mls' (detailed options mls -h) to list directory contain\n");
        printf(" 'mgrep' <filter_condition> (detailed options mgrep -h) [args list] returnes filtered according to rule args\n");

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
            return EXIT_SUCCESS;
        }

    }

    int mEcho(size_t nargs, char **args) {
        for (size_t indx =1; indx < nargs; ++indx) {
            printf("%s", args[indx]);
            ++indx;
        }
        printf("\n");
        return EXIT_SUCCESS;
    }

    inline fs::path* absPathTo(string arg){
        return  new fs::path (fs::path(environment->dir_->getActualPath()) / fs::path(arg) );
    }

    coreFuncLib::coreFuncLib() {

        external_lib_  = { //initializing full pathnames
                {"mls", absPathTo("mls") },
                {"mrm", absPathTo("mrm")},
                {"mcp", absPathTo("mcp")},
                {"mmv", absPathTo("mmv")},
                {"mmkdir", absPathTo("mmkdir")},
                {"mgrep", absPathTo("mgrep")}
        };

        embedded_lib_= {
                {"mcd", new sh_core::EmbeddedFunc("MY_CD", sh_core::myCd, sh_core::cd_help_msg, false)},
                {"mpwd", new sh_core::EmbeddedFunc("MY_PWD", sh_core::myPwd, sh_core::pwd_help_msg)},
                {"mhelp", new sh_core::EmbeddedFunc("MY_HELP", sh_core::myHelp, sh_core::help_help_msg)},
                {"mexit", new sh_core::EmbeddedFunc("MY_EXIT", sh_core::myExit, sh_core::exit_help_msg)},
                {"mecho", new sh_core::EmbeddedFunc("MY_ECHO", sh_core::mEcho, sh_core::echo_help_msg)},
                {"mmysh", new sh_core::EmbeddedFunc("MY_shell_script_interpreter",
                                                    sh_core::mySh,
                                                    shell_script_interpreter_help_msg)}
        };


    }
    coreFuncLib::~coreFuncLib() {

        for(auto i: embedded_lib_){
            delete i.second;
        }
        for (auto i: external_lib_)
            delete i.second;

    }


    int coreFuncLib::getNumOfMyBuiltins() const{
        return (int) embedded_lib_.size();
    }


    bool coreFuncLib::hasSuchEmbedded(const string *const arg) const{
        auto search_iter = embedded_lib_.find(*arg);
        return  (search_iter != embedded_lib_.end() );
    }

    bool coreFuncLib::hasMyshExtention(const string *const arg) const{
        string atrg1 = *arg;

        fs::path thisFile = fs::path(*arg);

//        std::cout << "Extention: [" << fs::extension(thisFile) << "]" << std::endl;

        if ((fs::exists(thisFile))
            && (strcmp(fs::extension(thisFile).c_str(), ".msh") == 0))
            return true;
        else return false;
    }

    bool coreFuncLib::hasSuchExternal(const string *const arg) const {
        auto search_iter = external_lib_.find(*arg);
        return  (search_iter != external_lib_.end() );
    }

}