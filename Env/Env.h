//
// Created by d1md1m on 09.05.17.
//

#ifndef LAB_2_SHELL_ENV_H
#define LAB_2_SHELL_ENV_H


#include "Directory.h"
#include "User.h"
#include "ConsoleView.h"
#include <map>
//#include "../Shell_core/LaneInterpreter.h"

namespace env {


    class Env {
    public:
        ConsoleView *console_;
        Directory *dir_;
        User *user_;
        std::map<string, string> *variables_;


        class VarManager {
        private: Env *env_;

        public:

            VarManager(Env *env);
            ~VarManager();

            bool doesVariableDeclaredLocally(const std::string *compare) const;

            std::string* getGlobalVar(const std::string* varName);

            std::string* getLocalVar(const std::string* varName);

            bool doesVariableDeclaredGlobaly(const std::string* varName) const;

            int declareVariableGlobally(const std::string* varName, const std::string* varValue, bool toOverride ) const;

            bool declareVariableLocally(const std::string* varName, const std::string* varValue);

            void show_local_variables();

        };
        VarManager *varManager_;



        Env();

        ~Env();
    };
}

#endif //LAB_2_SHELL_ENV_H
