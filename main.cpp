
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <pwd.h>
#include <map>
#include <fstream>


#include "ConsoleView.h"
#include "User.h"
#include "Directory.h"
#include "Embedded_func.h"


//const char *homedir;


using namespace std;




int my_cd(size_t nargs, char **args);
int my_pwd(size_t nargs, char **args);
int my_help(size_t nargs, char **args);
int my_exit(size_t nargs, char **args);
int my_sh(size_t nargs, char **args);



#define  home_dir_call  "~"

User *default_user;

class Interpreter;

class FileLaneIterator;




class FileLaneIterator{
private:
    ifstream infile;
    bool isGood;
public:

    FileLaneIterator(string filename){
        if (boost::filesystem::exists(filename)){
            infile.open(filename);

            //cout << "ISOPEN:  " << infile.is_open() <<endl;
            if (infile.is_open()){
                isGood = true;
                printf("FILE {%s} IS OPENED\n", filename.c_str());
            }
            else{
                printf("%s Could not be open\n", filename.c_str());
            }
        }
        else{
            perror("Such a file does not found\n");
            isGood = false;
        }

        //cout << "ISOPEN" << infile.is_open() <<endl;
        //cout << "EOF:  "  << infile.eof() << endl;
        //cout << "GOOD:  "  << infile.good() << endl;
        //ifstream infile(filename);
    }; //initialize via passing filename to open


    bool fileIsReady(){
        return (infile.good() && infile.is_open());
    }

    void getNextString(string *buf){

      //  cout << "ISOPEN" << infile.is_open() <<endl;
       // cout << "EOF:  "  << infile.eof() << endl;
       // cout << "GOOD:  "  << infile.good() << endl;
       // getchar();
        if (infile.is_open() && !infile.eof()){

            std::getline(infile, *buf);
            //infile.getline(buf, 512);
        }
        else {
            isGood = false;

        }
    }

    ~FileLaneIterator(){
        if(infile.is_open()){
            infile.close();
        }
    }
};


class Line_splitter{
public:
    Line_splitter(){

    }

    // left it here = line delimiters " \t\r\n\a"
    vector <string> my_split_line(string input_str)
    {
        input_str.append(" ");
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




    ~Line_splitter(){

    }
};


class Interpreter{
private:
    Line_splitter *splitter;
public:
    Interpreter(){
        this->splitter = new Line_splitter();
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


    int num_my_builtins() {
        return (int) embedded_lib.size();
    }

    int my_execute(vector<string> args)
    {

        char** cargs = new char*[args.size() + 1];
        unsigned int args_number = (int) args.size();


        cout << "NUMBER OF ARGS FOUND: " <<args_number << endl;
        splitter->str_vector_to_chars(&args , cargs);

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

    int proceed_sting(string* values){

        vector<string> args = splitter->my_split_line(*values);


        return my_execute(args);
    }

    ~Interpreter(){

    }
    //TODO check for function options
    //TODO filemasks
};



Interpreter* default_interpreter;

Line_splitter* def_line_split;

Directory *current_directory;


ConsoleView *console;


using  callable_function =  int (*)(size_t, char **);


// forward declarations of built-in commands

map <string, Embedded_func*> embedded_lib;


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

    return 1;
}

//just exits, that is it
int my_exit(size_t nargs, char **args)
{
    printf("my_Shell says GoodBye to You and wishes a good day ;O) ");
    return 0;
}

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




string my_read_line(void)
{
    string buffer;
    getline(cin,buffer);
    //buffer+=" ";
    return buffer;
}


//execuion loop
void my_loop(void)
{
    string line;
    vector<string> args;
    int status;


    do {
        console->display_all();
        line = my_read_line();
        status = default_interpreter->proceed_sting(&line);
        //args = my_split_line(line);
        //status = my_execute(args); //if 0 - finished, exited

    } while (status);
}



int main(int argc, char **argv)
{
    string cd_help_msg = "SOME CD HELP";
    my_cd_obj = new Embedded_func("MY_CD", my_cd, cd_help_msg );
    my_pwd_obj = new Embedded_func("MY_PWD", my_pwd, cd_help_msg );
    my_help_obj = new Embedded_func("MY_HELP", my_help, cd_help_msg );
    my_exit_obj = new Embedded_func("MY_EXIT", my_exit, cd_help_msg );
    my_shell_fileinterpreter = new Embedded_func("MY_FILEINTERPRETER", my_sh, cd_help_msg);

    embedded_lib= {
            { "cd",  my_cd_obj},
            { "pwd", my_pwd_obj },
            { "help", my_help_obj },
            { "exit", my_exit_obj },
            { "mysh", my_shell_fileinterpreter}

    };


    default_user = new User();
    default_interpreter = new Interpreter();
    def_line_split = new Line_splitter();
    //init_user(&this_user);
    current_directory = new Directory();
    console = new ConsoleView(current_directory);

    // Run command loop.
    my_loop();


    // Perform any shutdown/cleanup.
    delete default_user;
    delete current_directory;
    delete console;
    return EXIT_SUCCESS;
}
