
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <boost/filesystem.hpp>

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

const char *homedir;





using namespace std;


#define  home_dir_call  "~"

struct rights{
    //prototype
};

struct user_data{
    boost::filesystem::path home_dirrectory;
    string name;
    string pass;
    rights user_rights;
    bool is_defined = false;
};

user_data this_user;


struct  system_path{
    const u_int16_t max_path_length = 30;

    boost::filesystem::path actual_path;

    string path_buffer;

    bool path_was_changed = false;
};


system_path this_path;

void init_user(user_data *user){
    boost::filesystem::path buf;
    buf = this_path.actual_path;
    chdir("~");
    printf("TEEEST!!! \n");

    /* REFACTOR IF NEEDDED
     * If you require multi-thread safety, you should use getpwuid_r instead
     * of getpwuid like this (from the getpwnam(3) man page):
     */

    char *nameBuf;

    if ((nameBuf = getenv("HOME")) != NULL) {
        nameBuf = getpwuid(getuid())->pw_dir;
        this_user.home_dirrectory = nameBuf;
        cout << *nameBuf <<endl;
    }
    else {
        printf("Homedirectory is undedfined \n");
        this_user.home_dirrectory = "";
    }
    if ((nameBuf = getenv("USER")) != NULL) {
        nameBuf = getpwuid(getuid())->pw_name;
        this_user.name = nameBuf;
        cout << *nameBuf <<endl;
        printf("HELLO %s ! My_Shell is glad to see You ;0) \n", nameBuf);
    }
    else{
        printf("Username is undedfined \n");
    }



    cout << this_user.name <<endl;
    cout << this_user.home_dirrectory <<endl;

    //cout << "HOME" << getenv("HOME") <<endl;

    //cout << "MANPATH =>" << getenv("MANPATH") <<endl;

    //cout << "USER" << getenv("USER") <<endl;

    //cout << this_user.home_dirrectory;
    //cout << homedir <<endl;
    //cout << getpwuid(getuid())->pw_name << endl;
    //boost::filesystem::current_path();
};







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

int refresh_path(){
    try
    {
        this_path.actual_path = boost::filesystem::current_path();
        this_path.path_buffer = boost::filesystem::current_path().string();
    }
    catch (boost::filesystem::filesystem_error &e)
    {
        std::cerr << e.what() << '\n';
    }
    return EXIT_SUCCESS;
}

int my_pwd(char **args)
{
    refresh_path();
    printf("%s", this_path.actual_path.c_str());
    return 1;
}


int my_cd(char **args)
{
    if (args[1] == NULL) { //has to have at least one arg
        fprintf(stderr, "my_Shell: expected argument to \"cd\"\n");
    } else {
        string str(args[1]);
        if (str == home_dir_call){
            if (boost::filesystem::is_directory(this_user.home_dirrectory)){
                boost::filesystem::current_path(this_user.home_dirrectory);
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
    this_path.actual_path = boost::filesystem::current_path();
    this_path.path_was_changed = true;
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





bool contains_home(boost::filesystem::path *dir){

    if (this_user.home_dirrectory != ""){
       // printf ("TEST>>>>>>>>>>>>>CONTAINS HOME<<<<<<<<<<<< \n");
        //printf("%s \n", dir->c_str());
        //printf("%s \n", this_user.home_dirrectory.c_str());

        if ( (dir->string()).find(this_user.home_dirrectory.string()) != string::npos){
        //    printf ("TEST>>>>>>>>>>>>>CONTAINS HOME<<<<<<<<<<<< \n");
            return true;
        }
        //printf ("TEST>>>>>>>>>>>>>CONTAINS NO!!!  HOME<<<<<<<<<<<< \n");
    }
    return false;
}

size_t trim_path_to_size(string *path, unsigned int size){
    size_t position;
    size_t was_trimmed = 0;

    boost::filesystem::path path_buf = *path;

    if (contains_home(&path_buf)){
        *path = path->substr(this_user.home_dirrectory.string().length());
        cout << "TEST>>>>>>>>>>?????????????" <<endl;
        cout << *path << endl;
    }
    string path_delimiter = "/";
    while (path->length() > size){
        position = path->find(path_delimiter);
        if (position != string::npos){
            *path = path->substr(position+1);
            was_trimmed += position+1;
            cout << "path_trimmed ___" << *path << endl;
        }
    }
    return was_trimmed;
}

void display_path(){

    string pref = "";
    string postf = "$";
    size_t was_trimmed = 0;

    if (this_path.path_was_changed){
        this_path.path_buffer = boost::filesystem::current_path().c_str();

    }

    if ((contains_home(&this_path.actual_path))) {
        pref.append("~");
    }

    if (this_path.path_buffer.length() > this_path.max_path_length){
        was_trimmed = trim_path_to_size(&this_path.path_buffer, this_path.max_path_length);
    }
    if (was_trimmed){
        pref.append("...");
    }

    printf("\n%s%s%s", pref.c_str(), this_path.path_buffer.c_str(), postf.c_str());

    this_path.path_was_changed = false;

}

//execuion loop
void my_loop(void)
{
    string line;
    vector<string> args;
    int status;


    do {
        display_path();
        line = my_read_line();
        args = my_split_line(line);
        status = my_execute(args);

    } while (status);
}


void init_path(system_path* path){
    path->actual_path = boost::filesystem::current_path();
    refresh_path();
}

int main(int argc, char **argv)
{

    init_user(&this_user);

    init_path(&this_path);



    // Load config files, if any.

    //pass init
    //this_path.path_buffer = boost::filesystem::current_path().c_str();



    // Run command loop.
    my_loop();


    // Perform any shutdown/cleanup.

    return EXIT_SUCCESS;
}
