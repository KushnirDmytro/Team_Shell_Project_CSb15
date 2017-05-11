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

    //global state inside this namespase
    //Env *environment;

}