//
// Created by d1md1m on 27.03.17.
//

#ifndef LAB_2_SHELL_EMBEDDEDFUNC_H
#define LAB_2_SHELL_EMBEDDEDFUNC_H


#include  <string>
#include <map>
#include <cstring>
#include <boost/filesystem/operations.hpp>


#include "../Env/Env.h"
#include "Utils/FileLaneIterator.h"


namespace fs = boost::filesystem;


namespace sh_core {
    using std::string;


    using callable_function =  int (*)(size_t, char **);

#define  home_dir_call  "~"


    class EmbeddedFunc {
    protected:

        string name_;
        char **vargs_;
        size_t nargs_;
        string help_info_;
        callable_function func_;
        bool noargs_allowed_ = true;

    public:
        EmbeddedFunc(const string &name, callable_function funct_to_assign,const string &help_msg);

        int searchForHelp(const size_t nargs, char **&argvector);

        void outputHelp(const string &helpMsg) const;

        virtual int call(size_t nargs, char **args);

    };

    extern env::Env *environment;

}
#endif //LAB_2_SHELL_EMBEDDEDFUNC_H
