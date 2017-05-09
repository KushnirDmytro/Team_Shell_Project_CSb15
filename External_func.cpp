// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// Created by d1md1m on 29.03.17.
//


#include "External_func.h"

Defaul_options_manager::Defaul_options_manager( string name_,
                                                map <string, Defaul_options_manager*> *opts_map_){
    opts_map = opts_map_;
    option_name=name_;
}

Defaul_options_manager::~Defaul_options_manager() {
    for (auto item: *opts_map){
        delete item.second;
    }
    delete opts_map;
}

inline Defaul_options_manager* Defaul_options_manager::get_option(string potential_arg) {
    if (opts_map->find(potential_arg)
        ==
        opts_map->end())
        return nullptr;
    else
        return opts_map->at(potential_arg);
}


//checks for crossvalidations of flags setting
bool Defaul_options_manager::are_options_cross_valid(){
    printf("Purely default crosscheck, no aditional restrictions set\n");
    return true;
}

bool Defaul_options_manager::argumentless_option_check(size_t nargs, char **argv) {
    if (nargs == 0){
        if (noargs_allowed)
            return true;
        else{
            printf("FOUNDED NO ARGUMENTS\n");
            return false;
        }
    }
        //case when operations should be performed by other function
    else return are_suboptions_valid(nargs, argv);
}

//default validator for suboptions and it's flags
bool Defaul_options_manager::are_suboptions_valid(size_t nargs, char **argv) {

    if (nargs == 0) {
        return argumentless_option_check(nargs, argv);
    }

    queue<string> ls_argumens_queue;
    for (size_t i = 0; i < nargs; ++i) {
        ls_argumens_queue.push( string (argv[i]) );
    }

    Defaul_options_manager *option_to_check = nullptr;

    queue<string> arg_buf;

    string current_arg_name;

    while(!ls_argumens_queue.empty()){

        current_arg_name = ls_argumens_queue.front().c_str();

        if ( map_contains(current_arg_name) ) {
            //very first argument found case
            if (option_to_check != nullptr){

                if ( !suboptionS_arguments_validation(option_to_check, &arg_buf) ){
                    cout << "check_failed on option " << option_to_check->option_name << endl;
                    return false;
                }
                while (!arg_buf.empty())
                    arg_buf.pop();

            }
            option_to_check = get_option( current_arg_name);

        }

        else{

            if (option_to_check== nullptr){
                cout << "ERROR:" << current_arg_name <<" is unextpected start of arguments sequence for " << option_name <<endl;
                return  false;
            }

            arg_buf.push(current_arg_name);

        }

        ls_argumens_queue.pop();
    }

    if (option_to_check!= nullptr){
        if ( !suboptionS_arguments_validation(option_to_check, &arg_buf) ){
            cout << "check_failed on option " << option_to_check->option_name << endl;
            return false;
        }
    }

    if (are_options_cross_valid()){
        printf("ARGUMENT CHECK DONE \n");
        return true;
    }

    else    return false;
};


inline void clear_temp_array_of_pointers(size_t arr_size, char** arr_ptr){
    for(size_t i = 0; i < arr_size ; ++i)
        delete  arr_ptr[i];
}

bool Defaul_options_manager::suboptionS_arguments_validation(Defaul_options_manager* opt_to_check, queue<string>* arg_buf) {
    char* temp_buf[(*arg_buf).size()];
    str_queue_to_char_arr((*arg_buf), temp_buf);

    if (! (opt_to_check->are_suboptions_valid( arg_buf->size(), temp_buf) ) ){
        printf("ARGUMENT CHECK FAILED AT OPTION %s\n", opt_to_check->option_name.c_str());
        return false;
    }

    clear_temp_array_of_pointers(arg_buf->size(), temp_buf);
    return true;
}



void Defaul_options_manager::str_queue_to_char_arr(queue<string> queue, char **arr){
    size_t i = 0;
    while (!queue.empty()){
        arr[i] = new char[queue.front().size()+1];
        strcpy(arr[i], queue.front().c_str());
        queue.pop();
    }
}


bool Defaul_options_manager::map_contains(string seek_this_key) {
    return  !( opts_map->find(seek_this_key)
            ==
            opts_map->end() ) ;
}


//=============================CLASSED BORDER==============================




External_func::External_func (const string &name,
               callable_function funct_to_assign,
               string &help_msg):
        Embedded_func(name, funct_to_assign, help_msg){};


bool External_func::validate_is_directory(size_t nargs, char** vargs){
    boost::filesystem::path full_path;
    for (size_t i = 0; i < nargs; ++i){
        full_path = boost::filesystem::current_path();
        full_path /= string(vargs[i]);
        if (
                (!boost::filesystem::is_directory(vargs[i]))
                ||
                (! (boost::filesystem::is_directory(full_path) ) )
                ) {

            return false;
        }
    }
    return true;
}


//Overriding
int External_func::call(size_t nargs, char **args){

/*
    if (are_suboptions_valid(nargs, args)){
        cout << "problem checking" << endl;
  */

        return Embedded_func::call(nargs, args);

};










