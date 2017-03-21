
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <boost/filesystem.hpp>



using namespace std;



// forward declarations of built-in commands
int my_cd(char **args);
int my_help(char **args);
int my_exit(char **args);


//it could be map, but for such amount of functions it looked obsolete
const char (*my_builtin_str[]) = {
        "cd",
        "help",
        "exit"
};

int (*builtin_func[]) (char **) = {
        &my_cd,
        &my_help,
        &my_exit
};

int num_my_builtins() {
    return sizeof(my_builtin_str) / sizeof(char *);
}

//Builtin implementation


int my_cd(char **args)
{
    if (args[1] == NULL) { //has to have at least one arg
        fprintf(stderr, "my_Shell: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) { // checking for return of default c++ func
            perror("my_Shell failed to change dir");
        }
    }
    return 1;
}


//shows help info
int my_help(char **args)
{

    printf("Write command and arguments, if needed, then press Enter\n");
    printf("To get detailed information, write <command name> -help or <command name> -h:\n");

    for (int i = 0; i < num_my_builtins(); i++) {
        printf("  %s\n", my_builtin_str[i]);
    }

    return 1;
}

//just exits, that is it
int my_exit(char **args)
{
    printf("my_Shell says GoodBye to You and wishes a good day ;O) ");
    return 0;
}



// launcher for custom modules
int my_launcher(char **args)
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

    for(size_t i = 0; i < args->size(); ++i)
    {
        cargs[i] = new char[(*args)[i].size() + 1];
        strcpy(cargs[i], (*args)[i].c_str());
    }

    return 0;
}

int my_execute(vector<string> args)
{

    char** cargs = new char*[args.size()];

    str_vector_to_chars(&args , cargs);

//    vector<string>* test = &args;
 //   test->size();
  //  printf("argsSize = %", test->size());


    /*
    for(size_t i = 0; i < args.size(); ++i)
    {
        cargs[i] = new char[args[i].size() + 1];
        strcpy(cargs[i], args[i].c_str());
    }
*/


    for (int i = 0; i < num_my_builtins(); i++) {
        if (strcmp(cargs[0], my_builtin_str[i]) == 0) {
            return (*builtin_func[i])(cargs);
        }
    }

    return my_launcher(cargs);



    // An empty command was entered (became obsolete with getline)
    /*
    if (cargs[0] == NULL) {
        printf("Empty command was entered! \n");
        return 1;
    }
     */


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

    while ((pos = input_str.find(delim)) != string::npos) {
        tok = input_str.substr(0, pos);
        args.push_back(tok);
        input_str.erase(0, pos + delim.length());
    }
    return args;
}


//execuion loop
void my_loop(void)
{
    string line;
    vector<string> args;
    int status;

    do {
        printf("> ");
        line = my_read_line();
        args = my_split_line(line);
        status = my_execute(args);

    } while (status);
}

int main(int argc, char **argv)
{
    // Load config files, if any.

    // Run command loop.
    my_loop();

    // Perform any shutdown/cleanup.

    return EXIT_SUCCESS;
}
