// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Created by d1md1m on 28.03.17.
//

#include "Interpreter.h"


Interpreter::Interpreter(){

}


// launcher for custom modules
int Interpreter::my_extern_launcher(char **args)
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


int Interpreter::num_my_builtins() {
    return (int) embedded_lib.size();
}

int Interpreter::my_execute(vector<string> args)
{

    char** cargs = new char*[args.size() + 1];
    unsigned int args_number = (int) args.size();


    cout << "NUMBER OF ARGS FOUND: " <<args_number << endl;
    splitter.str_vector_to_chars(&args , cargs);

    //cout<< "builtIns #" << num_my_builtins() << endl;

    for (int i = 0; i < num_my_builtins(); i++) {
//        cout<< "builtIn #" << i << " = "<< my_builtin_str[i] <<endl;
        auto search_iter = embedded_lib.find(cargs[0]);
        if (search_iter != embedded_lib.end() ) // case when we have such a func in our lib
        {
            //=============CALLING INNER FUNCTION <=======================
            return (search_iter->second)->call(args_number, cargs);
        }
    }

    // CALLING EXTERN FUNC <======================
    return my_extern_launcher(cargs);


}

int Interpreter::proceed_sting(string* values){

    vector<string> args = splitter.my_split_line(*values);


    return my_execute(args);
}

Interpreter::~Interpreter(){
}
//TODO filemasks