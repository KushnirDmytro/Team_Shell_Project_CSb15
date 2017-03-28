//
// Created by d1md1m on 27.03.17.
//

#include <boost/filesystem/operations.hpp>
#include "Embedded_func.h"
//#include "Directory.h"

//using namespace std;


// HOW  TO ISOLATE THIS FUNCTIONS HERE ???

/*

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
*/