//
// Created by d1md1m on 29.03.17.
//


#include "External_func.h"
#include "Extern_LS.h"


Options::Options( string name){
    this->name=name;
}

Options::~Options() {
    //delete this->options_flags;
    delete this->opts_map;
}

inline Options* Options::get_option(string potential_arg) {
    if (this->opts_map->find(potential_arg)
        ==
        this->opts_map->end())
        return nullptr;
    else
        return this->opts_map->at(potential_arg);
}


//checks for crossvalidations of flags setting
bool Options::are_options_cross_valid(){
    printf("Purely default crosscheck, no aditional restrictions set\n");
    return true;
}

bool Options::argumentless_option_check(size_t nargs, char **argv) {
    if (nargs == 0){
        if (this->noargs_allowed)
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
bool Options::are_suboptions_valid(size_t nargs, char **argv){

    if (nargs == 0){
        return argumentless_option_check(nargs, argv);
    }


    vector<string> args_vec;
    args_vec.insert(args_vec.end(), &argv[0], &argv[nargs]);


    //==========CHECK of vector insertion performed =========
    for (int i = 0; i < nargs; ++i){
        printf("function get  ===>%s<===  \n", argv[i]);
    }
    for(auto i: args_vec)
        cout<< "vector inserted word ===>" << i <<"<===" << endl;
    //==========CHECK =========


    Options *prev_founded_option = nullptr;
    Options *new_founded_option = nullptr;


    vector<string> arg_buf;

    string iter_arg_name;

    for(int i = 0; i<args_vec.size(); ++i ){

        iter_arg_name = args_vec[i];

        if (new_founded_option != nullptr) { //case when it is not first iteration
            prev_founded_option = new_founded_option;
        }

        //new_founded_option =
        if (this->opts_map->find(iter_arg_name)
                                ==
                                this->opts_map->end())
            new_founded_option = nullptr;
        else
            new_founded_option = this->opts_map->at(iter_arg_name);
               // get_option(iter_arg_name);


        if (new_founded_option == nullptr) {
            if (prev_founded_option == nullptr) {
                printf("EROR, FIRST ARGUMENT ===>%s<=== IS NOT OPTION\n", iter_arg_name.c_str());
                return false;
            } else {
                arg_buf.push_back(iter_arg_name);

                cout << "arg added to arg buffer vector "<< iter_arg_name << endl;
                cout << "new size of arg vector " << arg_buf.size() << endl;

            }
        }


        else {
            if (prev_founded_option == nullptr) {
                prev_founded_option = new_founded_option;
            } else {

                if (!suboptionS_arguments_validation(prev_founded_option, &arg_buf))
                    return false;
            }


        }
            //LAST option check
        if ( i == nargs - 1){

            if (!suboptionS_arguments_validation(prev_founded_option, &arg_buf))
                return false;
        }

    }


    cout << "CROSS_VALIDATION" << endl;
    if (this->are_options_cross_valid()){
        printf("ARGUMENT CHECK DONE \n");
        return true;
    }

    else    return false;

}


bool Options::suboptionS_arguments_validation(Options* opt_to_check, vector<string>* arg_buf) {
    char* temp_buf[(*arg_buf).size()];
    str_vec_to_char_arr((*arg_buf), temp_buf);
    if (! (opt_to_check->are_suboptions_valid((*arg_buf).size(), temp_buf) ) ){
        printf("ARGUMENT CHECK FAILED AT OPTION %s\n", opt_to_check->name.c_str());
        return false;
    }
    (*arg_buf).clear();
    return true;
}



void Options::str_vec_to_char_arr(vector<string> vec, char**arr){
    for (int i =0; i < vec.size(); ++i){
        strcpy(arr[i], vec[i].c_str());
    }
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


    string hello =  "HelloWorld";
/*(
    if (this->func_opts->are_suboptions_valid(nargs, args)){
        cout << "problem checking" << endl;


        cout<< "Detailed listing flag "<<  ( (LS_opts*)this->func_opts)->LS_flags.detailed_listing  <<endl;
        cout<< "Recursive output flag "<<( (LS_opts*)this->func_opts)->LS_flags.recursive  <<endl;
        cout<< "Reverted output flag "<<( (LS_opts*)this->func_opts)->LS_flags.reverse_output  <<endl;
        cout<< "Sorting type "<<  ( (LS_opts*)this->func_opts)->LS_flags.sort_type <<endl;
*/
        return Embedded_func::call(nargs, args);

};










