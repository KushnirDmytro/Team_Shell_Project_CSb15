//
// Created by d1md1m on 29.03.17.
//


#include "External_func.h"
#include "Extern_LS.h"


Function_options::Function_options(map <string,  command_option*> *func_opts_map){
*this->func_opts_map = *func_opts_map;
}

command_option* Function_options::get_option(string potential_arg) {
    if (this->func_opts_map->find(potential_arg)
        ==
        this->func_opts_map->end())
        return nullptr;
    else
        return this->func_opts_map->at(potential_arg);
}


bool Function_options::are_options_cross_valid(size_t nargs, char *argv[], External_func* ref_to_owner){
    if (nargs == 0){
        if (this->noargs_allowed)
            return true;
        else{
            printf("FOUNDED NO ARGUMENTS\n");
            return false;
        }
    }

    //!IMPORTANT!!!
    //--nargs;


    if (opt_cross_valid == nullptr){
        cout << "cross_validator is not initialised" << endl;
        //return true;
    }
    vector<string> args_vec;
    args_vec.insert(args_vec.end(), &argv[0], &argv[nargs]);


    //==========CHECK =========
    for (int i = 0; i < nargs; ++i){
        printf("function get  ===>%s<===  \n", argv[i]);
    }
    for(auto i: args_vec)
        cout<< "vector inserted word ===>" << i <<"<===" << endl;
    //==========CHECK =========


    command_option *prev_founded_option = nullptr;
    command_option *new_founded_option = nullptr;


    vector<string> arg_buf;

    for(auto iter_arg_name: args_vec){
        //founded_option = get_option(i);
        new_founded_option = get_option(iter_arg_name);
        if (new_founded_option == nullptr) {
            if (prev_founded_option == nullptr) {
                printf("EROOR, FIRST ARGUMENT ===>%s<=== IS NOT OPTION\n", iter_arg_name.c_str());
                return false;
            } else {
                arg_buf.push_back(iter_arg_name);
            }
        }
        else{
            if(prev_founded_option == nullptr){
                prev_founded_option = new_founded_option;
            }
            else{
                //perform arguments checking
                char* temp_buf[arg_buf.size()];
                str_vec_to_char_arr(arg_buf, temp_buf);
                if (! ((options_validator) prev_founded_option->opt_inner_valid)(arg_buf.size(), temp_buf, ref_to_owner )){
                    printf("ARGUMENT CHECK FAILED AT OPTION %s\n", prev_founded_option->name.c_str());
                    return false;
                }
            }
            if (iter_arg_name == args_vec[args_vec.size()-1]){
                char* temp_buf[arg_buf.size()];
                str_vec_to_char_arr(arg_buf, temp_buf);
                if (!prev_founded_option->opt_inner_valid(arg_buf.size(), temp_buf, ref_to_owner)){
                    printf("ARGUMENT CHECK FAILED AT OPTION %s\n", prev_founded_option->name.c_str());
                    return false;
                }
            }
        }
    }
    //block for last argument
    printf("ARGUMENT CHECK DONE \n");
    return true;
}




void Function_options::str_vec_to_char_arr(vector<string> vec, char**arr){
    for (int i =0; i < vec.size(); ++i){
        strcpy(arr[i], vec[i].c_str());
    }
}






//=============================CLASSED BORDER==============================





//options_validator opt_cross_valid;



/*
 * 1)find options in function's options list
 * 2)assume other words are referring to options arguments
 * 3)validate if option arguments are good
 */
// bool are_options_valid(){
//    this->func_opts->are_options_cross_valid(this->nargs, this->vargs);
/*
int options_index = 0;
size_t args_index = 0;
bool is_arg = false;
int last_founded_option = -1;

vector<char*> founded_args;

while (args_index < this->nargs) {
    options_index = this->func_opts->is_option(this->vargs[args_index]);
    if ( options_index != -1) {
        is_arg = true;
        last_founded_option = options_index;
        if (last_founded_option != -1) {
            if( ! this->func_opts.options[last_founded_option].opt_inner_valid(founded_args.size(), &founded_args[0])) {
                printf(" ARGUMENT %s FOR OPTION %s IS INVALID \n", this->vargs[nargs], this->func_opts.options->name.c_str() );
                return false;
            }
        }
        else{
            printf("UNKNONW ARGUMENT %s FOUND BEFORE ANY OPTIONS ", this->vargs[args_index]);
            return false;
        }
    }else {
        if (last_founded_option != -1) {
            is_arg = false;
            founded_args.push_back(this->vargs[args_index]);
        }
    }
    ++args_index;
}
return true;
 */
// }



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
    // this->nargs = nargs;
    // this->vargs = args;
    if (this->func_opts->are_options_cross_valid(nargs, args, this)){

        cout<< ((ls_option_flags*)this->opts_flags)->detailed_listing <<endl;

        return Embedded_func::call(nargs, args);
    }
    else return 0;
}










