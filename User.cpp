//
// Created by d1md1m on 26.03.17.
//

#include "User.h"

using namespace std;

User::User() {
    /* REFACTOR IF NEEDDED
         * If you require multi-thread safety, you should use getpwuid_r instead
         * of getpwuid like this (from the getpwnam(3) man page):
         */
    char *nameBuf;

    if ((nameBuf = getenv("HOME")) != NULL) {
        nameBuf = getpwuid(getuid())->pw_dir;
        this->home_dirrectory = nameBuf;
        //this.home_dirrectory = nameBuf;
        //cout << *nameBuf <<endl;
    }
    else {
        printf("Homedirectory is undedfined \n");
        this->home_dirrectory = "";
    }
    if ((nameBuf = getenv("USER")) != NULL) {
        nameBuf = getpwuid(getuid())->pw_name;
        this->name = nameBuf;
        //cout << *nameBuf <<endl;
        printf("HELLO %s ! My_Shell is glad to see You ;0) \n", nameBuf);
    }
    else{
        printf("Username is undedfined \n");
    }
}



void User::setHome_dirrectory(const boost::filesystem::path &home_dirrectory) {
    this->home_dirrectory = home_dirrectory;
}

void User::setName(const string &name) {
    this->name = name;
}

void User::setPass(const string &pass) {
    this->pass = pass;
}

void User::setUser_rights(const rights &user_rights) {
    this->user_rights = user_rights;
}



