//
// Created by d1md1m on 26.03.17.
//

#ifndef LAB_2_SHELL_DIRECTORY_H
#define LAB_2_SHELL_DIRECTORY_H

#include <boost/filesystem/path.hpp>
#include <iostream>
#include "User.h"

using namespace std;


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
    bool contains_home( /*boost::filesystem::path &dir = getActual_path() */  User *this_user = default_user){
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



#endif //LAB_2_SHELL_DIRECTORY_H
