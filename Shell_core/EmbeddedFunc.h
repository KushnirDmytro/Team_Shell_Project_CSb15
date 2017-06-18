//
// Created by d1md1m on 27.03.17.
//

#ifndef LAB_2_SHELL_EMBEDDEDFUNC_H
#define LAB_2_SHELL_EMBEDDEDFUNC_H


#include  <string>
#include <cstring>
#include <boost/filesystem.hpp>
#include "../Env/Env.h"
// TODO make environment as an variable being passed

namespace fs = boost::filesystem;


namespace sh_core {
    using std::string;


    using callable_function =  int (*)(size_t, char **);

#define  home_dir_call  "~"


    class EmbeddedFunc {
    protected:

        string name_;
        char **initialVargs_;
        size_t initialNargs_;
        string help_info_;
        callable_function func_;
        bool noargs_allowed_ = true;


        bool needToPrintHelp(const size_t nargs, char **&argvector);

        void outputHelp(const string &helpMsg) const;

        void cleanAfterExecution();

    public:
        EmbeddedFunc(const string &name, callable_function funct_to_assign,
                     const string &help_msg,
                     bool noargs_allowed = true);


        virtual int call(size_t nargs, char **args);

//        struct Printer { // nested class
//            static int fd[2]; // files descriptor array for pipeline
//        };

    };

    extern env::Env *environment;

}
#endif //LAB_2_SHELL_EMBEDDEDFUNC_H
