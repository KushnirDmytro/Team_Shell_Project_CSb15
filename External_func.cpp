// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Created by d1md1m on 29.03.17.
//


#include "External_func.h"
#include "Extern_LS.h"


Options::Options( string name_){
    option_name=name_;
}

Options::~Options() {
    delete opts_map;
}

inline Options* Options::get_option(string potential_arg) {
    if (opts_map->find(potential_arg)
        ==
        opts_map->end())
        return nullptr;
    else
        return opts_map->at(potential_arg);
}


//checks for crossvalidations of flags setting
bool Options::are_options_cross_valid(){
    printf("Purely default crosscheck, no aditional restrictions set\n");
    return true;
}

bool Options::argumentless_option_check(size_t nargs, char **argv) {
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
bool Options::are_suboptions_valid(size_t nargs, char **argv) {

    if (nargs == 0) {
        return argumentless_option_check(nargs, argv);
    }

  //  vector<string> args_vec;
   // args_vec.insert(args_vec.end(), argv, argv + nargs);

    queue<string> ls_argumens_queue;
    for (size_t i = 0; i < nargs; ++i) {
        ls_argumens_queue.push( string (argv[i]) );
    }


/*

    //==========CHECK of vector insertion performed =========

    for (int i = 0; i < nargs; ++i) {
        printf("function get  ===>%s<===  \n", argv[i]);
    }
    for (auto i: args_vec)
        cout << "vector inserted word ===>" << i << "<===" << endl;

    while (!ls_argumens_queue.empty()) {
        //auto buf = ls_argumens_queue.front();
        cout << "queue inserted word ===>" << ls_argumens_queue.front() << "<===" << endl;
        ls_argumens_queue.pop();
    }
    for (auto i : args_vec) {
        ls_argumens_queue.push(i);
    }

    int i = 0;
    while (!ls_argumens_queue.empty()) {
        //auto buf = ls_argumens_queue.front();


        cout << "queue inserted word ===>" << ls_argumens_queue.front() <<
             "equality to " << args_vec[i] << strcmp(args_vec[i].c_str(), ls_argumens_queue.front().c_str())<< endl;
        ls_argumens_queue.pop();
        i++;
    }
    for (auto i : args_vec) {
        ls_argumens_queue.push(i);
    }


    //==========CHECK =========


    */


    Options *option_to_check = nullptr;

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

    cout << "CROSS_VALIDATION" << endl;
    if (this->are_options_cross_valid()){
        printf("ARGUMENT CHECK DONE \n");
        return true;
    }

    else    return false;
};


inline void clear_temp_array_of_pointers(size_t arr_size, char** arr_ptr){
    for(size_t i = 0; i < arr_size ; ++i)
        delete  arr_ptr[i];
}

bool Options::suboptionS_arguments_validation(Options* opt_to_check, queue<string>* arg_buf) {
    char* temp_buf[(*arg_buf).size()];
    str_queue_to_char_arr((*arg_buf), temp_buf);

    if (! (opt_to_check->are_suboptions_valid( arg_buf->size(), temp_buf) ) ){
        printf("ARGUMENT CHECK FAILED AT OPTION %s\n", opt_to_check->option_name.c_str());
        return false;
    }

    clear_temp_array_of_pointers(arg_buf->size(), temp_buf);
    //arg_buf->clear();
    return true;
}



void Options::str_queue_to_char_arr(queue<string> queue, char **arr){
    size_t i = 0;
    while (!queue.empty()){
        arr[i] = new char[queue.front().size()+1];
        strcpy(arr[i], queue.front().c_str());
        queue.pop();
    }
}


bool Options::map_contains(string seek_this_key) {
    return  !( opts_map->find(seek_this_key)
            ==
            opts_map->end() ) ;
}






//=============================CLASSED BORDER==============================





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
    if (this->func_opts->are_suboptions_valid(nargs, args)){
        cout << "problem checking" << endl;


        cout<< "Detailed listing flag "<<  ( (LS_opts*)this->func_opts)->LS_flags.detailed_listing  <<endl;
        cout<< "Recursive output flag "<<( (LS_opts*)this->func_opts)->LS_flags.recursive  <<endl;
        cout<< "Reverted output flag "<<( (LS_opts*)this->func_opts)->LS_flags.reverse_output  <<endl;
        cout<< "Sorting type "<<  ( (LS_opts*)this->func_opts)->LS_flags.sort_type <<endl;

       */
        return sh_core::Embedded_func::call(nargs, args);

};










