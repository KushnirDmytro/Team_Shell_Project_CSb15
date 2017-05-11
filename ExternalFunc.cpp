// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Created by d1md1m on 29.03.17.
//


#include "ExternalFunc.h"
#include "Extern_LS.h"

namespace ext {

    DefaultOptionsManager::DefaultOptionsManager(string name_) {
        option_name_ = name_;
    }

    DefaultOptionsManager::~DefaultOptionsManager() {
        delete opts_map;
    }

    inline DefaultOptionsManager *DefaultOptionsManager::getSuboptionFromMap(const string potential_arg) const{
        if (!map_contains(potential_arg))
            return nullptr;
        else
            return opts_map->at(potential_arg);
    }


//checks for crossvalidations of flags setting
    bool DefaultOptionsManager::areOptionsCrossValid() {
        printf("Purely default crosscheck, no aditional restrictions set\n");
        return true;
    }

    bool DefaultOptionsManager::argumentlessSuboptionCheck(size_t nargs, char **argv) {
        if (nargs == 0) {
            if (noargs_allowed_)
                return true;
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
            return argumentlessSuboptionCheck(nargs, argv);
        }

        //  vector<string> args_vec;
        // args_vec.insert(args_vec.end(), argv, argv + nargs);

        queue<string> ls_argumens_queue;
        for (size_t i = 0; i < nargs; ++i) {
            ls_argumens_queue.push(string(argv[i]));
        }


        DefaultOptionsManager *option_to_check = nullptr;

        queue<string> arg_buf;

        string current_arg_name;

        while (!ls_argumens_queue.empty()) {

            current_arg_name = ls_argumens_queue.front().c_str();

            if (map_contains(current_arg_name)) {
                //very first argument found case
                if (option_to_check != nullptr) {

                    if (!suboptionS_arguments_validation(option_to_check, &arg_buf)) {
                        cout << "check_failed on option " << option_to_check->option_name_ << endl;
                        return false;
                    }
                    while (!arg_buf.empty())
                        arg_buf.pop();

                }

                option_to_check = getSuboptionFromMap(current_arg_name);

            } else {

                if (option_to_check == nullptr) {
                    cout << "ERROR:" << current_arg_name << " is unextpected start of arguments sequence for "
                         << option_name_ << endl;
                    return false;
                }

                arg_buf.push(current_arg_name);

            }

            ls_argumens_queue.pop();
        }

        if (option_to_check != nullptr) {
            if (!suboptionS_arguments_validation(option_to_check, &arg_buf)) {
                cout << "check_failed on option " << option_to_check->option_name_ << endl;
                return false;
            }
        }

        cout << "CROSS_VALIDATION" << endl;
        if (this->areOptionsCrossValid()) {
            printf("ARGUMENT CHECK DONE \n");
            return true;
        } else return false;
    };


    inline void clearTempPointersArray(size_t arr_size, char **arr_ptr) {
        for (size_t i = 0; i < arr_size; ++i)
            delete arr_ptr[i];
    }

    bool DefaultOptionsManager::suboptionS_arguments_validation(DefaultOptionsManager *opt_to_check, queue<string> *arg_buf) {
        char *temp_buf[(*arg_buf).size()];
        str_queue_to_char_arr((*arg_buf), temp_buf);

        if (!(opt_to_check->suboptionsAreValid(arg_buf->size(), temp_buf))) {
            printf("ARGUMENT CHECK FAILED AT OPTION %s\n", opt_to_check->option_name_.c_str());
            return false;
        }

        clearTempPointersArray(arg_buf->size(), temp_buf);
        //arg_buf->clear();
        return true;
    }


    void DefaultOptionsManager::str_queue_to_char_arr(queue<string> queue, char **arr) {
        size_t i = 0;
        while (!queue.empty()) {
            arr[i] = new char[queue.front().size() + 1];
            strcpy(arr[i], queue.front().c_str());
            queue.pop();
        }
    }


    bool DefaultOptionsManager::map_contains(const string seek_this_key) const{
        return !(opts_map->find(seek_this_key)
                 ==
                 opts_map->end());
    }






//=============================CLASSED BORDER==============================





    bool ExternalFunc::isValidDirectory(size_t nargs, char **vargs) {
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

        //if (this->func_opts->suboptionsAreValid(nargs, args)){

        return sh_core::EmbeddedFunc::call(nargs, args);

    };


}






