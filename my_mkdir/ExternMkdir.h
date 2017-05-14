//
// Created by d1md1m on 14.05.17.
//

#ifndef LAB_2_SHELL_EXTERNMKDIR_H
#define LAB_2_SHELL_EXTERNMKDIR_H

#include <string.h>
#include <boost/filesystem.hpp>
#include <map>
#include <iostream>

#include "../ExternalFunc.h"


namespace ext {

    class ExternMkdir : public ExternalFunc {

        bool allGoingFine = true;
    public:
        ExternMkdir(const string &name,
                    sh_core::callable_function funct_to_assign,
                    string &help_msg);

        int call(size_t nargs, char **argv) override;

        ~ExternMkdir();

    };
}

#endif //LAB_2_SHELL_EXTERNMKDIR_H
