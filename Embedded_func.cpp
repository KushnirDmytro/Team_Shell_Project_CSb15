//
// Created by d1md1m on 27.03.17.
//

#include <boost/filesystem/operations.hpp>
#include "Embedded_func.h"
#include "Directory.h"
#include "ConsoleView.h"
#include "Line_splitter.h"
#include "Interpreter.h"
#include "FileLaneIterator.h"

using namespace std;




User * default_user;

Directory *current_directory;

Line_splitter* def_line_split;

Interpreter* default_interpreter;

ConsoleView *console;

map <string, Embedded_func*> embedded_lib;






Embedded_func::Embedded_func(const string &name, callable_function funct_to_assign, string &help_msg){
    this->name=name;
    this->func = funct_to_assign;
    this->help_info = help_msg;
}


int Embedded_func::search_for_help(size_t nargs, char** &argvector){
    for (int i = 0; i< nargs ; ++i){
        if (( strcmp(argvector[i], "--help") == 0  ) || ( strcmp(argvector[i], "-h") == 0  ) ){
            return 1;
        }
    }
    return 0;
}

void Embedded_func::output_help(string &helpMsg){
    printf("%s\n", this->help_info.c_str());
}


int Embedded_func::call(size_t nargs, char **args){
    this->nargs = nargs;
    this->vargs = args;
    this->initialized = true;
    if (this->search_for_help(this->nargs, this->vargs)){
        this->output_help(this->help_info);
        return 1;
    }
    return func (this->nargs, this->vargs);
    //     return 1;
}







//====================BUILT-IN COMMANDS ============

//show current directory
int my_pwd(size_t nargs, char **args)
{
    current_directory->refresh_path();
    printf("%s", current_directory->getActual_path().c_str());
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
            if ( boost::filesystem::is_directory( default_user->getHome_dirrectory() ) ){
                boost::filesystem::current_path(default_user->getHome_dirrectory());
            }
        }
        else if (boost::filesystem::is_directory(args[1])){
            boost::filesystem::current_path(args[1]);
        }
        else{
            //TODO filesystem errors
            //boost::filesystem::filesystem_error()
            perror("\n my_Shell failed to change dir \t");
        }

    }
    current_directory->setActual_path(boost::filesystem::current_path());
    current_directory->setPath_was_changed(true);
    return 1;
}

//shows help info
int my_help(size_t nargs, char **args)
{

    printf("Write command and arguments, if needed, then press 'Enter'\n");
    printf("To get detailed information, write <command name> --help or <command name> -h:\n");
    printf("List of Commands:\n");
    printf(" 'pwd' -- > returns current directory | arguments <NULL>\n");
    printf(" 'cd' [pass]  -- > changes current execution dirrectory to [pass] 1 argument required\n");
    printf(" 'mysh' <script_filename>.sh can launch *.sh scripts interpreting them\n");

    printf(" In case of external function name inputed (some Shell extentions) it will be executed if founded\n");

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

//executes in this shell external ".msh" files
int my_sh(size_t nargs, char **args)
{
    if (nargs > 1){
        string file_path;
        file_path = current_directory->getActual_path().string();
        file_path.append("/");
        file_path.append(args[1]);
        if (boost::filesystem::is_regular_file(args[1]) ){
            FileLaneIterator *iter = new FileLaneIterator(args[1]);
            string st;
            cout << "------------FILE READING IN PROCESS------------------" << endl;
            int i =0;
            int status;
            while(iter->fileIsReady()){
                iter->getNextString(&st);
                printf("String #%d red \n", i++);
                cout << st << endl;
                // st.append(" ");
                status = default_interpreter->proceed_sting(&st);
                if (!status){
                    return 0;
                };

            }
        }
        else if (boost::filesystem::is_regular_file(file_path) ){
            FileLaneIterator *iter = new FileLaneIterator(file_path);
            string st;
            cout << "------------FILE READING IN PROCESS------------------" << endl;
            while(iter->fileIsReady()){
                iter->getNextString(&st);
                cout << st << endl;
                return default_interpreter->proceed_sting(&st);
            }
        }
        else{
            perror(args[1]);
            perror(file_path.c_str());
        }


    }
    return 1;
}

//====================BUILT-IN COMMANDS END============

