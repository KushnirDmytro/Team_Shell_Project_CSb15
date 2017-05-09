//
// Created by d1md1m on 09.05.17.
//

#include "Env.h"


using namespace shell;

Env::Env(){
    dir_ = new Directory();
    user_ = new User() ;
    interpreter_ =  new Interpreter();
    console_ = new ConsoleView(dir_, user_);
}

Env::~Env() {
    delete dir_;
    delete user_;
    delete interpreter_;
    delete console_;
}

Env shell::*env;