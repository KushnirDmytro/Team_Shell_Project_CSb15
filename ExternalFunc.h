//
// Created by d1md1m on 29.03.17.
//

#ifndef LAB_2_SHELL_EXTERNAL_FUNC_H
#define LAB_2_SHELL_EXTERNAL_FUNC_H

#include <queue>
#include <map>

#include "Env/Env.h"
#include "Shell_core/EmbeddedFunc.h"


// =====================>class OPTIONS  DECLARATION

namespace extrn {


    using std::vector;
    using std::string;
    using std::map;
    class DefaultOptionsManager;



//takes some arguments and checks are they in valid list (or of valid kind)

    using options_validator = bool (*)(size_t, char **, DefaultOptionsManager *ref_to_owner_object);

    class DefaultOptionsManager {

    protected:

        string option_name_;

        std::map<string, DefaultOptionsManager *> *opts_map_;

        //default value definition
        bool noargs_allowed_ = true;

        bool *default_state_to_write;

        DefaultOptionsManager *getSuboptionFromMap(const string potential_arg) const;

        // NO need to make them protected, class is inner private
    public:

        DefaultOptionsManager(string name,
                              bool * state_of_success  = nullptr,
                              std::map<string, DefaultOptionsManager *> *opts_map_ =
                              new std::map<string, DefaultOptionsManager *> {});
        virtual ~DefaultOptionsManager();


        virtual bool suboptionsAreValid(size_t nargs, char **argv);

        virtual bool areOptionsCrossValid();

        void convertStrQueueToCharArr( std::queue<string> queue, char **arr) const;

        bool argumentlessSuboptionCheck(size_t nargs, char **argv, bool*  report_success) ;

        virtual bool doesSuboptionSArgumentsAreValid(DefaultOptionsManager *opt_to_check, std::queue<string> *arg_buf);

        bool doesMapContain(const string seek_this) const;

        void clearTempPointersArray(size_t arr_size, char **arr_ptr) const;

        virtual void clearFlags();
    };


// =====================>class OPTIONS DECLARATION END



//=======================> class External Funcion

    class ExternalFunc : public sh_core::EmbeddedFunc {

    private:
    protected:


        ExternalFunc(const string &name,
                      sh_core::callable_function funct_to_assign,
                      string &help_msg,
                      DefaultOptionsManager *options_ptr);

        virtual ~ExternalFunc();

        DefaultOptionsManager *func_opts_;


        void clearFlags(DefaultOptionsManager* opt_to_clear);

    public:
//Overriding
        int call(size_t nargs, char **args) override;

    };

//=======================> class External Funcion END

}



#endif //LAB_2_SHELL_EXTERNAL_FUNC_H
