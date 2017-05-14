// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Created by d1md1m on 09.05.17.
//

#include "Env.h"


namespace env {

    Env::Env() {
        dir_ = new Directory();
        user_ = new User();
        console_ = new ConsoleView(dir_, user_);
    }

    Env::~Env() {
        delete dir_;
        delete user_;
        delete console_;
    }

    Env *environment;

}