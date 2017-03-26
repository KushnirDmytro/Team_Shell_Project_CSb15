
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <boost/filesystem.hpp>

#include <pwd.h>



#include "PromptConsoleInterface.h"
#include "User.h"
#include "Directory.h"


//const char *homedir;


using namespace std;


#define  home_dir_call  "~"

User default_user;





class Directory{
public:

    const boost::filesystem::path &getActual_path() const {
        return actual_path;
    }

    void setActual_path(const boost::filesystem::path &actual_path) {
        Directory::actual_path = actual_path;
    }

    bool isPath_was_changed() const {
        return path_was_changed;
    }

    void setPath_was_changed(bool path_was_changed) {
        Directory::path_was_changed = path_was_changed;
    }

private:

    boost::filesystem::path actual_path;

    bool path_was_changed;

public:
    Directory(){
        this->refresh_path();
        this->setPath_was_changed(true);
    }

    int refresh_path(){
        try
        {
            this->setActual_path(boost::filesystem::current_path());
        }
        catch (boost::filesystem::filesystem_error &e)
        {
            std::cerr << e.what() << '\n';
        }
        return EXIT_SUCCESS;
    }

    //by default shows info about this object, but can do for any
    bool contains_home( /*boost::filesystem::path &dir = getActual_path() */  User *this_user = &default_user){
        if (this_user->getHome_dirrectory() != ""){

            if ( this->getActual_path().string().find(this_user->getHome_dirrectory().string()) != string::npos){
                //    printf ("TEST>>>>>>>>>>>>>CONTAINS HOME<<<<<<<<<<<< \n");
                return true;
            }
            //printf ("TEST>>>>>>>>>>>>>CONTAINS NO!!!  HOME<<<<<<<<<<<< \n");
        }
        return false;
    }
};


Directory *current_directory;



class ConsoleView{
private:
    Directory *current_directoryPtr;

    string path_buffer;
    const u_int16_t max_path_length = 30; //yes, it is "Magic"

public:

    ConsoleView(Directory *directory_adr){
        this->setCurrent_directoryPtr(directory_adr);
    }

    const u_int16_t getMax_path_length() const {
        return max_path_length;
    }


    Directory *getCurrent_directoryPtr() const {
        return current_directoryPtr;
    }

    void setCurrent_directoryPtr(Directory *current_directoryPtr) {
        this->current_directoryPtr = current_directoryPtr;
    }




    const string &getPath_buffer() const {
        return path_buffer;
    }

    void setPath_buffer(const string &path_buffer) {
        this->path_buffer = path_buffer;
    }


    void refresh_path_buffer(){
        this->setPath_buffer(this->current_directoryPtr->getActual_path().c_str());
    }

    void display_path(){

        string pref = "";
        string postf = "$";
        size_t was_trimmed = 0;


        if (this->current_directoryPtr->isPath_was_changed()){
            this->setPath_buffer(boost::filesystem::current_path().c_str());
        }

        if ( this->current_directoryPtr->contains_home() ) {
            pref.append("~");
        }

        if ((this->getPath_buffer().length() > this->getMax_path_length()) || this->current_directoryPtr->contains_home() ){
            was_trimmed = trim_path_to_size( &(this->path_buffer) , this->getMax_path_length());
        }
        if (was_trimmed){
            pref.append("...");
        }

        printf("\n%s%s%s", pref.c_str(), this->getPath_buffer().c_str(), postf.c_str());

        this->current_directoryPtr->setPath_was_changed(false);

    }



    size_t trim_path_to_size(string *path, unsigned int size, User *this_user = &default_user){
        size_t position;
        size_t was_trimmed = 0;
        boost::filesystem::path path_buf = *path;

        if (this->current_directoryPtr->contains_home(this_user)){
            *path = path->substr(this_user->getHome_dirrectory().string().length());
            //cout << "TEST>>>>>>>>>>?????????????" <<endl;
            //cout << *path << endl;
        }
        string path_delimiter = "/";
        while (path->length() > size){
            position = path->find(path_delimiter);
            if (position != string::npos){
                *path = path->substr(position+1);
                was_trimmed += position+1;
                //cout << "path_trimmed ___" << *path << endl;
            }
        }
        return was_trimmed;
    }


};



ConsoleView *console;







// forward declarations of built-in commands
int my_cd(char **args);
int my_pwd(char **args);
int my_help(char **args);
int my_exit(char **args);


//it could be map, but for such amount of functions it looked obsolete
const char (*my_builtin_str[]) = {
        "pwd",
        "cd",
        "help",
        "exit"
};

int (*builtin_func[]) (char **) = {
        &my_pwd,
        &my_cd,
        &my_help,
        &my_exit
};

int num_my_builtins() {
    return sizeof(my_builtin_str) / sizeof(char *);
}

//Builtin implementation


int my_pwd(char **args)
{
    current_directory->refresh_path();
    printf("%s", current_directory->getActual_path().c_str());
    return 1;
}


int my_cd(char **args)
{
    if (args[1] == NULL) { //has to have at least one arg
        fprintf(stderr, "my_Shell: expected argument to \"cd\"\n");
    } else {
        string str(args[1]);
        if (str == home_dir_call){
            if ( boost::filesystem::is_directory( default_user.getHome_dirrectory() ) ){
                boost::filesystem::current_path(default_user.getHome_dirrectory());
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

    cout<< "builtIns #" << num_my_builtins() << endl;

    for (int i = 0; i < num_my_builtins(); i++) {
        cout<< "builtIn #" << i << " = "<< my_builtin_str[i] <<endl;
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
        console->display_path();
        line = my_read_line();
        args = my_split_line(line);
        status = my_execute(args);

    } while (status);
}



int main(int argc, char **argv)
{
    default_user;

    //init_user(&this_user);
    current_directory = new Directory();
    console = new ConsoleView(current_directory);

    // Load config files, if any.

    //pass init
    //this_path.path_buffer = boost::filesystem::current_path().c_str();



    // Run command loop.
    my_loop();


    // Perform any shutdown/cleanup.

    return EXIT_SUCCESS;
}
