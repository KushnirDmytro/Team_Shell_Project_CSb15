//
// Created by d1md1m on 09.05.17.
//

#ifndef LAB_2_SHELL_ENV_H
#define LAB_2_SHELL_ENV_H

#include "ConsoleView.h"
//#include "../Shell_core/LaneInterpreter.h"

namespace env {

    class Env {
    public:
        ConsoleView *console_;
        Directory *dir_;
        User *user_;

        Env();

        ~Env();
    };
}

#endif //LAB_2_SHELL_ENV_H
