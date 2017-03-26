//
// Created by d1md1m on 26.03.17.
//

#ifndef LAB_2_SHELL_USER_H
#define LAB_2_SHELL_USER_H


#include <stdlib.h>
#include <stdio.h>

#include <boost/filesystem.hpp>

#include <pwd.h>


using namespace std;



struct rights{
    //prototype
};



class User{
private:
    boost::filesystem::path home_dirrectory;
    string name;
    string pass;
    string hostname;
public:
    const string &getHostname() const;

    void setHostname(const string &hostname);

private:
    rights user_rights;
    //bool is_defined = false;
public:
    void setHome_dirrectory(const boost::filesystem::path &);

    void setName(const string &name);

    void setPass(const string &pass);


    void setUser_rights(const rights &user_rights);

    const boost::filesystem::path &getHome_dirrectory() const {
       return home_dirrectory;
    }

    const string &getName() const {
        return name;
    }

    const string &getPass() const {
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
