//
// Created by d1md1m on 26.03.17.
//

#ifndef LAB_2_SHELL_USER_H
#define LAB_2_SHELL_USER_H


#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <boost/filesystem.hpp>

#include <pwd.h>



namespace fs = boost::filesystem;

struct rights{
    //prototype
};



class User{
private:
    fs::path home_dirrectory;
    std::string name;
    std::string pass;
    std::string hostname;
public:
    const std::string &getHostname() const;

    void setHostname(const std::string &hostname);

private:
    rights user_rights;
    //bool is_defined = false;
public:
    void setHome_dirrectory(const fs::path &);

    void setName(const std::string &name);

    void setPass(const std::string &pass);


    void setUser_rights(const rights &user_rights);

    const fs::path &getHome_dirrectory() const {
         return home_dirrectory;
    }

    const std::string &getName() const {
        return name;
    }

    const std::string &getPass() const {
        return pass;
    }

    const rights &getUser_rights() const {
        return user_rights;
    }



public:
    User();

};


extern User * default_user;

#endif //LAB_2_SHELL_USER_H
