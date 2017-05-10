// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//
// Created by d1md1m on 26.03.17.
//

#include "User.h"
#include "Env.h"


namespace env {

    User::User() {
        /* REFACTOR IF NEEDDED
             * If you require multi-thread safety, you should use getpwuid_r instead
             * of getpwuid like this (from the getpwnam(3) man page):
             */
        char *nameBuf;

        if ((nameBuf = getenv("HOME")) != NULL) {
            nameBuf = getpwuid(getuid())->pw_dir;
            home_dirrectory = nameBuf;
        }
        else {
            printf("Homedirectory is undedfined \n");
            home_dirrectory = "";
        }

        if ((nameBuf = getenv("USER")) != NULL) {
            nameBuf = getpwuid(getuid())->pw_name;
            name = nameBuf;
            printf("HELLO %s ! My_Shell is glad to see You ;0) \n", nameBuf);
        }
        else {
            printf("Username is undedfined \n");
            setName("");
        }

        char hostname[HOST_NAME_MAX];

        if (gethostname(hostname, HOST_NAME_MAX)) {
            perror("Hostname is Unknown");
            setHostname("");
        } else {
            setHostname(hostname);
        }


    }


    void User::setHome_dirrectory(const fs::path &home_dirrectory_) {
        home_dirrectory = home_dirrectory_;
    }

    void User::setName(const string &name_) {
        name = name_;
    }

    void User::setPass(const string &pass_) {
        pass = pass_;
    }

    void User::setUser_rights(const rights &user_rights_) {
        user_rights = user_rights_;
    }

    const string &User::getHostname() const {
        return hostname;
    }

    void User::setHostname(const string &hostname_) {
        hostname = hostname_;
    }
}


