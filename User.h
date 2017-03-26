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

/*
class User{
public:
    void setHome_dirrectory(const boost::filesystem::path &home_dirrectory) {
        User::home_dirrectory = home_dirrectory;
    }

    void setName(const string &name) {
        User::name = name;
    }

    void setPass(const string &pass) {
        User::pass = pass;
    }

    void setUser_rights(const rights &user_rights) {
        User::user_rights = user_rights;
    }

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


private:
    boost::filesystem::path home_dirrectory;
    string name;
    string pass;
    rights user_rights;
    //bool is_defined = false;
public:
    User(){

        /* REFACTOR IF NEEDDED
         * If you require multi-thread safety, you should use getpwuid_r instead
         * of getpwuid like this (from the getpwnam(3) man page):
         */
/*

        char *nameBuf;

        if ((nameBuf = getenv("HOME")) != NULL) {
            nameBuf = getpwuid(getuid())->pw_dir;
            User::home_dirrectory = nameBuf;
            //this.home_dirrectory = nameBuf;
            //cout << *nameBuf <<endl;
        }
        else {
            printf("Homedirectory is undedfined \n");
            User::home_dirrectory = "";
        }
        if ((nameBuf = getenv("USER")) != NULL) {
            nameBuf = getpwuid(getuid())->pw_name;
            User::name = nameBuf;
            //cout << *nameBuf <<endl;
            printf("HELLO %s ! My_Shell is glad to see You ;0) \n", nameBuf);
        }
        else{
            printf("Username is undedfined \n");
        }



    }

};

 */
#endif //LAB_2_SHELL_USER_H
