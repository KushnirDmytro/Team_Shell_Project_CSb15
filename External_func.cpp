//
// Created by d1md1m on 29.03.17.
//


#include "External_func.h"
#include "Extern_LS.h"


Options::Options(map <string, Options*> *opts_map, string name){
*this->opts_map = *opts_map;
    this->name=name;
}


Options::~Options() {
    delete this->options_flags;
    delete this->opts_map;
}

Options* Options::get_option(string potential_arg) {
    if (this->opts_map->find(potential_arg)
        ==
        this->opts_map->end())
        return nullptr;
    else
        return this->opts_map->at(potential_arg);
}


bool Options::are_options_cross_valid(){
    printf("Purely default crosscheck, no aditional restrictions set\n");
    return true;
}

bool Options::are_suboptions_valid(size_t nargs, char **argv){
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


    vector<string> args_vec;
    args_vec.insert(args_vec.end(), &argv[0], &argv[nargs]);


    //==========CHECK =========
    for (int i = 0; i < nargs; ++i){
        printf("function get  ===>%s<===  \n", argv[i]);
    }
    for(auto i: args_vec)
        cout<< "vector inserted word ===>" << i <<"<===" << endl;
    //==========CHECK =========


    Options *prev_founded_option = nullptr;
    Options *new_founded_option = nullptr;


    vector<string> arg_buf;

    for(auto iter_arg_name: args_vec){
        //founded_option = get_option(i);

        if (new_founded_option != nullptr) {
            prev_founded_option = new_founded_option;
        }
        new_founded_option = get_option(iter_arg_name);
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
        else{
            if(prev_founded_option == nullptr){
                prev_founded_option = new_founded_option;
            }
            else{


                //TODO MAKE THIS PART INLINE FUNCTION
                //perform arguments checking
                char* temp_buf[arg_buf.size()];
                str_vec_to_char_arr(arg_buf, temp_buf);
                if (! (prev_founded_option->are_suboptions_valid(arg_buf.size(), temp_buf) ) ){
                    printf("ARGUMENT CHECK FAILED AT OPTION %s\n", prev_founded_option->name.c_str());
                    return false;
                }
                arg_buf.clear();
            }

            //block for last argument
            if (iter_arg_name == args_vec[args_vec.size()-1]){

                char* temp_buf[arg_buf.size()];
                str_vec_to_char_arr(arg_buf, temp_buf);


                if (! (new_founded_option->are_suboptions_valid(arg_buf.size(), temp_buf) ) ){
                    printf("ARGUMENT CHECK FAILED AT OPTION %s\n", new_founded_option->name.c_str());
                    return false;
                }
                arg_buf.clear();
            }
        }
    }


    cout << "CROSS_VALIDATION" << endl;
    if (this->are_options_cross_valid()){
        printf("ARGUMENT CHECK DONE \n");
        return true;

}
    else return false;
}




void Options::str_vec_to_char_arr(vector<string> vec, char**arr){
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
//    this->func_opts->are_suboptions_valid(this->nargs, this->vargs);
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

    if (this->func_opts->are_suboptions_valid(nargs, args)){
        cout << "problem checking" << endl;


        cout<< "Detailed listing flag "<<((ls_option_flags*)this->func_opts->options_flags)->detailed_listing <<endl;
        cout<< "Recursive output flag "<<((ls_option_flags*)this->func_opts->options_flags)->recursive <<endl;
        cout<< "Reverted output flag "<<((ls_option_flags*)this->func_opts->options_flags)->reverse_output <<endl;
        cout<< "Sorting type "<<  ((ls_option_flags*)this->func_opts->options_flags)->sort_type <<endl;

        return Embedded_func::call(nargs, args);
    }
    else return 0;
}










