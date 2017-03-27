
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <boost/filesystem.hpp>

#include <pwd.h>
#include <map>


#include "ConsoleView.h"
#include "User.h"
#include "Directory.h"


//const char *homedir;


using namespace std;


#define  home_dir_call  "~"

User *default_user;


Directory *current_directory;


ConsoleView *console;


using  callable_function =  int (*)(size_t, char **);



// forward declarations of built-in commands

int my_cd(size_t nargs, char **args);
int my_pwd(size_t nargs, char **args);
int my_help(size_t nargs, char **args);
int my_exit(size_t nargs, char **args);

callable_function my_cd_addr = my_cd;
callable_function my_pwd_addr = my_pwd;
callable_function my_help_addr = my_help;
callable_function my_exit_addr = my_exit;




class Embedded_func{
private:
    string name;
    char** vargs;
    size_t nargs;
    string help_info;
    bool initialized;
    callable_function func;
public:
    Embedded_func(const string &name, callable_function funct_to_assign, string &help_msg){
        this->name=name;
        this->func = funct_to_assign;
        this->help_info = help_msg;
    }


    int search_for_help(size_t nargs, char** &argvector){
        for (int i = 0; i< nargs ; ++i){
            if (( strcmp(argvector[i], "--help") == 0  ) || ( strcmp(argvector[i], "-h") == 0  ) ){
                return 1;
            }
        }
        return 0;
    }

    void output_help(string &helpMsg){
        printf("%s\n", this->help_info.c_str());
    }


    int call(size_t nargs, char **args){
        this->nargs = nargs;
        this->vargs = args;
        this->initialized = true;
        if (this->search_for_help(this->nargs, this->vargs)){
            this->output_help(this->help_info);
            return 1;
        }
        func (this->nargs, this->vargs);
        return 1;
    }

};


Embedded_func *my_pwd_obj;
Embedded_func *my_cd_obj;
Embedded_func *my_help_obj;
Embedded_func *my_exit_obj;

map <string, Embedded_func*> embedded_lib;


//it could be map, but for such amount of functions it looked obsolete
/*
//TODO mapHERE
const char (*my_builtin_str[]) = {
        "pwd",
        "cd",
        "help",
        "exit"
};
*/


int num_my_builtins() {
    return (int) embedded_lib.size();
}


//Embedded_func *builtin_lib[4];
/*{
        my_pwd_obj,
        my_cd_obj,n
        my_help_obj,
        my_exit_obj
};*/

/*
int (*builtin_func[]) (unsigned int , char ** ) = {
        &my_pwd,
        &my_cd,
        &my_help,
        &my_exit
};
 */
//Builtin implementation


int my_pwd(size_t nargs, char **args)
{
    current_directory->refresh_path();
    printf("%s", current_directory->getActual_path().c_str());
    return 1;
}




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

    printf("Write command and arguments, if needed, then press Enter\n");
    printf("To get detailed information, write <command name> -help or <command name> -h:\n");

/*    for (int i = 0; i < num_my_builtins(); i++) {
        printf("  %s\n", my_builtin_str[i]);
    }
*/
    return 1;
}

//just exits, that is it
int my_exit(size_t nargs, char **args)
{
    printf("my_Shell says GoodBye to You and wishes a good day ;O) ");
    return 0;
}



// launcher for custom modules
int my_extern_launcher(char **args)
{
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
        } while (!WIFEXITED(status) && !WIFSIGNALED(status)); //checks for valid scenarios of exiting (we borrowed it)
    }

    return 1;
}


//produced Kovalchuk, Refactored & extracted by Kushnir
int str_vector_to_chars(vector<string> *args, char** cargs){
    size_t i;

    for(i = 0; i < args->size(); ++i)
    {
        cargs[i] = new char[(*args)[i].size() + 1];
        strcpy(cargs[i], (*args)[i].c_str());
    }
    cargs[i] = NULL;
    return 0;
}

int my_execute(vector<string> args)
{

    char** cargs = new char*[args.size() + 1];
    unsigned int args_number = (int) args.size();


    cout << "NUMBER OF ARGS FOUND: " <<args_number << endl;
    str_vector_to_chars(&args , cargs);

    cout<< "builtIns #" << num_my_builtins() << endl;

    for (int i = 0; i < num_my_builtins(); i++) {
//        cout<< "builtIn #" << i << " = "<< my_builtin_str[i] <<endl;
        auto search_iter = embedded_lib.find(cargs[0]);
        if (search_iter != embedded_lib.end() ) // case when we have such a func in our lib
         {
            return (search_iter->second)->call(args_number, cargs);
         }
    }

    return my_extern_launcher(cargs);


}


string my_read_line(void)
{
    string buffer;
    getline(cin,buffer);
    buffer+=" ";
    return buffer;
}

// left it here = line delimiters " \t\r\n\a"
vector <string> my_split_line(string input_str)
{
    vector<string>args;

    string delim = " "; //for delimer
    size_t pos = 0;
    string tok; // for toke

    unsigned long start;

    start = input_str.find_first_not_of(" ");

    input_str = input_str.substr(start);

    //cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" <<endl;
    while ((pos = input_str.find(delim)) != string::npos) {
        tok = input_str.substr(0, pos);
      //  cout << tok << endl;
        args.push_back(tok);
        input_str.erase(0, pos + delim.length());
    }
    return args;
}


/*
class Interpreter{

}
*/

//execuion loop
void my_loop(void)
{
    string line;
    vector<string> args;
    int status;


    do {
        console->display_all();
        line = my_read_line();
        args = my_split_line(line);
        status = my_execute(args); //if 0 - finished, exited

    } while (status);
}



int main(int argc, char **argv)
{
    string cd_help_msg = "SOME CD HELP";
    my_cd_obj = new Embedded_func("MY_CD", my_cd, cd_help_msg );
    my_pwd_obj = new Embedded_func("MY_PWD", my_pwd, cd_help_msg );
    my_help_obj = new Embedded_func("MY_HELP", my_help, cd_help_msg );
    my_exit_obj = new Embedded_func("MY_EXIT", my_exit, cd_help_msg );


    embedded_lib= {
            { "cd",  my_cd_obj},
           { "pwd", my_pwd_obj },
            { "help", my_help_obj },
            { "exit", my_exit_obj }
    };


/*    builtin_lib[0] =
            my_pwd_obj;

    builtin_lib[1] =
            my_cd_obj;

    builtin_lib[2] =
            my_help_obj;
    builtin_lib[3] =
            my_exit_obj;

*/
    default_user = new User();

    //init_user(&this_user);
    current_directory = new Directory();
    console = new ConsoleView(current_directory);


    // Load config files, if any.

    //test
    /*
    char* args[] ={
            "cd",
            "~"
    };
    my_cd_obj->call(2, args);
     */
    // Run command loop.
    my_loop();


    // Perform any shutdown/cleanup.
    delete default_user;
    delete current_directory;
    delete console;
    return EXIT_SUCCESS;
}
