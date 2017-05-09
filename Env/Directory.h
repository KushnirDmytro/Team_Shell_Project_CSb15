//
// Created by d1md1m on 26.03.17.
//

#ifndef LAB_2_SHELL_DIRECTORY_H
#define LAB_2_SHELL_DIRECTORY_H

#include <boost/filesystem/path.hpp>
#include <iostream>
#include <string>


#include "User.h"


namespace fs = boost::filesystem;


//extern Env *env;

//extern User* default_user;

class Directory{
public:

    const fs::path &getActual_path() const;

    void setActual_path(const fs::path &actual_path); ;

    bool isPath_was_changed() const ;;

    void setPath_was_changed(bool path_was_changed); ;

private:

    fs::path actual_path;

    bool path_was_changed;

public:
    Directory();

    int refresh_path();

    //by default shows info about this object, but can do for any
    bool contains_his_home( /*boost::filesystem::path &dir = getActual_path() */  User *this_user);
};



#endif //LAB_2_SHELL_DIRECTORY_H
