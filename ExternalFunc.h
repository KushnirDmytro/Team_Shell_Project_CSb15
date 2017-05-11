//
// Created by d1md1m on 29.03.17.
//

#ifndef LAB_2_SHELL_EXTERNAL_FUNC_H
#define LAB_2_SHELL_EXTERNAL_FUNC_H

#include <cstddef>
#include <string>
#include <vector>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/range/iterator_range.hpp>

#include <queue>

#include "Env/Env.h"


#include "Shell_core/EmbeddedFunc.h"



// =====================>class OPTIONS  DECLARATION

using namespace std;

namespace ext {

    class DefaultOptionsManager;

//takes some arguments and checks are they in valid list (or of valid kind)

    using options_validator = bool (*)(size_t, char **, DefaultOptionsManager *ref_to_owner_object);


    class DefaultOptionsManager {

        //TODO MAKE PROTECTED WHEN SOLVE INIT PROBLEM
    public:

        string option_name;

        map<string, DefaultOptionsManager *> *opts_map;

        //default value definition
        bool noargs_allowed = true;


        //field for classes to initialize
        //options_validator opt_cross_valid = nullptr;

        DefaultOptionsManager(string name);

        ~DefaultOptionsManager();

        DefaultOptionsManager *get_option(string potential_arg);

        virtual bool are_suboptions_valid(size_t nargs, char **argv);

        virtual bool are_options_cross_valid();

        void str_queue_to_char_arr(queue<string> queue, char **arr);

        bool argumentless_option_check(size_t nargs, char **argv);

        virtual bool suboptionS_arguments_validation(DefaultOptionsManager *opt_to_check, queue<string> *arg_buf);

        bool map_contains(string seek_this);
    };


// =====================>class OPTIONS DECLARATION END



//=======================> class External Funcion

    class ExternalFunc : public sh_core::EmbeddedFunc {

    private:
    protected:

        //DefaultOptionsManager *func_opts;

        ExternalFunc(const string &name,
                      sh_core::callable_function funct_to_assign,
                //DefaultOptionsManager *options_ptr,
                      string &help_msg) :
                sh_core::EmbeddedFunc(name, funct_to_assign, help_msg) {
            //this->func_opts = options_ptr;
        }


        bool isValidDirectory(size_t nargs, char **vargs);

    public:
//Overriding
        int call(size_t nargs, char **args) override;


    };

//=======================> class External Funcion END

}



#endif //LAB_2_SHELL_EXTERNAL_FUNC_H
