// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Created by d1md1m on 29.03.17.
//


#include "ExternalFunc.h"
#include "ExternLS.h"

namespace ext {

    DefaultOptionsManager::DefaultOptionsManager(string name_,
                                                 bool * state_of_success,
                                                 std::map<string, DefaultOptionsManager *> *opts_map) {
        //TODO options embedd
        default_state_to_write = state_of_success;
        opts_map_ = opts_map;
        option_name_ = name_;
    }

    DefaultOptionsManager::~DefaultOptionsManager() {

        for(auto i: *opts_map_){
            delete i.second;
        }
        delete opts_map_;
    }

    inline DefaultOptionsManager *DefaultOptionsManager::getSuboptionFromMap(
            const string potential_arg) const{

        if (!doesMapContain(potential_arg))
            return nullptr;
        else
            return opts_map_->at(potential_arg);
    }


//checks for crossvalidations of flags setting
    bool DefaultOptionsManager::areOptionsCrossValid() {
        printf("Purely default crosscheck, no aditional restrictions set\n");
        return true;
    }

    bool DefaultOptionsManager::argumentlessSuboptionCheck(size_t nargs, char **argv, bool* report_succsess) {
        if (nargs == 0) {

            if (noargs_allowed_) { //case when no args is ok
                if (report_succsess != nullptr)
                    *report_succsess = true;
                return true;
            }
            else {
                printf("FOUNDED NO ARGUMENTS\n");
                return false;
            }
        }
            //case when operations should be performed by other function
        else return suboptionsAreValid(nargs, argv);
    }

//default validator for suboptions and it's flags
    bool DefaultOptionsManager::suboptionsAreValid(size_t nargs, char **argv) {

        if (nargs == 0) {
            return argumentlessSuboptionCheck(nargs, argv, default_state_to_write);
        }

        std::queue<string> ls_argumens_queue;
        for (size_t i = 0; i < nargs; ++i)
            ls_argumens_queue.push(string(argv[i]));

        DefaultOptionsManager *option_to_check = nullptr;
        std::queue<string> arg_buf;
        string current_arg_name;

        while (!ls_argumens_queue.empty()) {

            current_arg_name = ls_argumens_queue.front().c_str();

            if (doesMapContain(current_arg_name)) {
                //very first argument found case
                if (option_to_check != nullptr) {

                    if (!doesSuboptionSArgumentsAreValid(option_to_check, &arg_buf)) {
                        std::cout << "check_failed on option "
                                  << option_to_check->option_name_ << std::endl;
                        return false;
                    }

                    while (!arg_buf.empty())
                        arg_buf.pop();

                }

                option_to_check = getSuboptionFromMap(current_arg_name);

            }
            else {

                if (option_to_check == nullptr) {
                    std::cout << "ERROR:"
                              << current_arg_name << " is unextpected start of arguments sequence for "
                         << option_name_ << std::endl;
                    return false;
                }

                arg_buf.push(current_arg_name);

            }

            ls_argumens_queue.pop();
        }

        if (option_to_check != nullptr) {

            if (!doesSuboptionSArgumentsAreValid(option_to_check, &arg_buf)) {
                std::cout << "check_failed on option "
                          << option_to_check->option_name_
                          << std::endl;
                return false;
            }

        }

        std::cout << "CROSS_VALIDATION" << std::endl;

        if (areOptionsCrossValid()) {
            printf("ARGUMENT CHECK DONE \n");
            return true;
        }
        else return false;
    };


    inline void DefaultOptionsManager::clearTempPointersArray(size_t arr_size, char **arr_ptr) const{
        for (size_t i = 0; i < arr_size; ++i)
            delete arr_ptr[i];
    }

    bool DefaultOptionsManager::doesSuboptionSArgumentsAreValid(
            DefaultOptionsManager *opt_to_check,
            std::queue<string> *arg_buf) {

        char *temp_buf[(*arg_buf).size()];
        convertStrQueueToCharArr((*arg_buf), temp_buf);

        if (!(opt_to_check->suboptionsAreValid(arg_buf->size(), temp_buf))) {
            printf("ARGUMENT CHECK FAILED AT OPTION %s\n", opt_to_check->option_name_.c_str());
            return false;
        }

        clearTempPointersArray(arg_buf->size(), temp_buf);

        return true;
    }


    inline void DefaultOptionsManager::convertStrQueueToCharArr(
            std::queue<string> queue,
            char **arr) const{

        size_t i = 0;
        while (!queue.empty()) {
            arr[i] = new char[queue.front().size() + 1];
            strcpy(arr[i], queue.front().c_str());
            queue.pop();
        }

    }


    inline bool DefaultOptionsManager::doesMapContain(const string seek_this_key) const{

        return !(opts_map_->find(seek_this_key)
                 ==
                 opts_map_->end());

    }

    inline void DefaultOptionsManager::clearFlags() {
        std::cout << "HEY, you have not overloaded clearing Flags function! I am: "
                  << option_name_
                  << std::endl;
    }






//=============================CLASSED BORDER==============================




    ExternalFunc::ExternalFunc(const string &name,
                 sh_core::callable_function funct_to_assign,
                 string &help_msg,
                 DefaultOptionsManager *options_ptr) :
            sh_core::EmbeddedFunc(name, funct_to_assign, help_msg) {
        func_opts_ = options_ptr;
    }

    ExternalFunc::~ExternalFunc() {
        delete func_opts_;
    };

    inline bool ExternalFunc::isValidDirectory(size_t nargs, char **vargs) {
        boost::filesystem::path full_path;
        for (size_t i = 0; i < nargs; ++i) {
            full_path = boost::filesystem::current_path();
            full_path /= string(vargs[i]);
            if (
                    (!boost::filesystem::is_directory(vargs[i]))
                    ||
                    (!(boost::filesystem::is_directory(full_path)))
                    ) {

                return false;
            }
        }
        return true;
    }


//Overriding
    int ExternalFunc::call(size_t nargs, char **args) {

     //   if (func_opts_ != nullptr && func_opts_->suboptionsAreValid(nargs, args))



            return sh_core::EmbeddedFunc::call(nargs, args);


            /*
        else{
            std::cout << "Opts check failed " << std::endl;
            return false;
        }
             */
    };

    void ExternalFunc::clearFlags(DefaultOptionsManager* opt_to_clear) {
        opt_to_clear->clearFlags();
    }





}






