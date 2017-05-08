//
// Created by d1md1m on 08.05.17.
//

#include "lib_emb.h"



User * default_user;

Directory *current_directory;

Line_splitter* def_line_split;

Interpreter* default_interpreter;

ConsoleView *console;

map <string, Embedded_func*> embedded_lib;


//====================BUILT-IN COMMANDS ============


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
                if (st.length() == 0)
                    continue;
                status = default_interpreter->proceed_sting(&st);
                if (!status){
                    delete iter;
                    return 0;
                };

            }
            delete iter;
        }
        else if (boost::filesystem::is_regular_file(file_path) ){
            FileLaneIterator *iter = new FileLaneIterator(file_path);
            string st;
            cout << "------------FILE READING IN PROCESS------------------" << endl;
            if(iter->fileIsReady()){
                iter->getNextString(&st);
                cout << st << endl;
                delete iter;
                return  default_interpreter->proceed_sting(&st);
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

