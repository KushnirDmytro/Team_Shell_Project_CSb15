// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Created by d1md1m on 09.05.17.
//



#include "Directory.h"
#include "User.h"
#include "ConsoleView.h"
#include "Env.h"

namespace env {

    Env::Env() {
        dir_ = new Directory();
        user_ = new User();
        console_ = new ConsoleView(dir_, user_);
        variables_ = new std::map<string, string>();
        varManager_ = new VarManager(this);
    }


    Env::~Env() {
        delete dir_;
        delete user_;
        delete console_;
        delete variables_;
        delete varManager_;
    }

    Env::VarManager::VarManager(Env *env){
        env_ = env;
    }

    Env::VarManager::~VarManager(){}

    bool Env::VarManager::doesVariableDeclaredLocally(const std::string *compare) const{
        auto search_iter = env_->variables_->find(*compare);
        return  (search_iter != env_->variables_->end() );
    }

    bool Env::VarManager::doesVariableDeclaredGlobaly(const std::string* varName) const{
        char *x = getenv(varName->c_str());
        return (x!=NULL); //true if var is declared
    }

    std::string* Env::VarManager::getGlobalVar(const std::string* varName) {
        if (doesVariableDeclaredGlobaly(varName))
            return new string(getenv(varName->c_str()));
        else return new string("UNDEFINED_VAR");
    }


    std::string* Env::VarManager::getLocalVar(const std::string* varName){
        if (doesVariableDeclaredLocally(varName)) {
            printf("map at [%s] has [%s] \n", (*varName).c_str(), (env_->variables_->at(*varName)).c_str());

            return new string(env_->variables_->at(*varName));
        }
        else return new string("UNDEFINED_VAR");
    }


    int Env::VarManager::declareVariableGlobally(const std::string* varName, const std::string* varValue, bool toOverride ) const{
        if (env_->user_->getUser_rights().toChangaGlobalVariables ) {
            if (setenv(varName->c_str(), varValue->c_str(), toOverride)){
                perror("failed to write system variable");
                return EXIT_FAILURE;
            }
            return EXIT_SUCCESS;
            } else return EXIT_FAILURE;
    }

    bool Env::VarManager::declareVariableLocally(const std::string* varName, const std::string* varValue){

        std::pair<std::map<string, string>::iterator, bool> a =
                env_->variables_->insert(std::pair<string, string>(*varName, *varValue));
        return a.second;
    }

    void Env::VarManager::show_local_variables(){
        for(auto it:(*env_->variables_)){
            printf("[%s] => {%s}\n", it.first.c_str(), it.second.c_str());
        }
    }


// ================= ORIGINAL DECLARATION ===================
    Env *environment;

}