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

    const boost::filesystem::path &getActual_path() const;

    void setActual_path(const boost::filesystem::path &actual_path); ;

    bool isPath_was_changed() const ;;

    void setPath_was_changed(bool path_was_changed); ;

private:

    boost::filesystem::path actual_path;

    bool path_was_changed;

public:
    Directory();

    int refresh_path();

    //by default shows info about this object, but can do for any
    bool contains_home( /*boost::filesystem::path &dir = getActual_path() */ const User *this_user = default_user);
};



#endif //LAB_2_SHELL_DIRECTORY_H
