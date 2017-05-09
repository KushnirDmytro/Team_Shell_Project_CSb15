//
// Created by d1md1m on 09.05.17.
//

#include "Env.h"


using namespace shell;

Env::Env(){
    dir = new Directory();
    usr = new User() ;
    interp =  new Interpreter();
  //  cons = new ConsoleView(dir);
}

Env::~Env() {
    delete dir;
    delete usr;
    delete interp;
  //  delete cons;
}

Env *env;