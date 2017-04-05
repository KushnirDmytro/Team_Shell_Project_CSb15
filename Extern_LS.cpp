//
// Created by d1md1m on 31.03.17.
//

#include "Extern_LS.h"




//just activator-function
extern int my_ls(size_t nargs, char **argv)
{
    return extern_ls_obj->my_ls_inner(nargs, argv);
    //my_ls_inner(nargs, argv);
}

extern Extern_LS *extern_ls_obj;

//OBSOLETE (global help check on higher lvl


command_option ls_opt_help{
        ls_opt_help.opt_n = 0,
        ls_opt_help.name = "Help",
        ls_opt_help.opt_args = nullptr,
        .opt_inner_valid = [](size_t nargs, char** vargs = nullptr, Function_options* owner_opt_obj) {
            return nargs == 0;
        }
};


command_option ls_opt_l{
        ls_opt_help.opt_n = 0,
        ls_opt_help.name = "-l",
        ls_opt_help.opt_args = nullptr,
        .opt_inner_valid = [](size_t nargs, char** vargs = nullptr, Function_options* owner_opt_obj){
            cout << "ENTERED DETAILED_OUTPUT" <<endl;
            if (nargs == 0){
                //Mamma, I'm a crimminal
                ( (ls_option_flags*) owner_opt_obj->opts_flags)->detailed_listing = true;
                return true;
            }
            else{
                return false;
            }
        }
};
command_option ls_opt_sort = {

        ls_opt_help.opt_n = 0,
        ls_opt_help.name = "--sort",
        ls_opt_help.opt_args = nullptr,
        .opt_inner_valid = [](size_t nargs, char **vargs = nullptr, Function_options* owner_opt_obj) {
            cout << "ENTERED SORT_OPTIONS" <<endl;
            cout << nargs << " Args number" << endl;

            if (nargs == 0){
                //Mamma, I'm a crimminal
                ( (ls_option_flags*) owner_opt_obj->opts_flags)->sort_type = EXTENTION;
                return true;
            }
            else{
                //( (ls_option_flags*) owner_opt_obj->opts_flags)->sort_type = EXTENTION;

                cout << vargs[0] << " RECEIVED AS ARG" << endl;


                if (nargs == 1) {
                    if ( !strcmp(vargs[0], "U") ){
                        ( (ls_option_flags*) owner_opt_obj->opts_flags)->sort_type = UNSORT;
                        return true;
                    }
                    else if( !strcmp(vargs[0], "S") ){
                        ( (ls_option_flags*) owner_opt_obj->opts_flags)->sort_type = SIZE;
                        cout << ( (ls_option_flags*) owner_opt_obj->opts_flags)->sort_type << "<============= CHECK\n";
                        return true;
                    }
                    else if( !strcmp(vargs[0], "t") ){
                        ( (ls_option_flags*) owner_opt_obj->opts_flags)->sort_type = TIME_MODIFIED;
                        return true;
                    }
                    else if( !strcmp(vargs[0], "X") ){
                        ( (ls_option_flags*) owner_opt_obj->opts_flags)->sort_type = EXTENTION;
                        return true;
                    }
                    else if( !strcmp(vargs[0], "N") ){
                        ( (ls_option_flags*) owner_opt_obj->opts_flags)->sort_type = NAME;
                        return true;
                    }
                    else {

                        printf("UNDEFINED SPECIFIER FOR SORTING OPTION %s \n", vargs[0]);
                        return false;
                    }
                }
                else {
                    printf("Error on argumnets number (%d) for  option --sort\n", (int)nargs );
                    return false;
                }
                }
            }
};

command_option ls_opt_revers = {
        ls_opt_help.opt_n = 0,
        ls_opt_help.name = "-r",
        ls_opt_help.opt_args = nullptr,
        .opt_inner_valid = [](size_t nargs, char **vargs = nullptr,Function_options* owner_opt_obj) {
            cout << "ENTERED REVERS_OUTPUT" <<endl;
            if (nargs == 0){
                //Mamma, I'm a crimminal
                ( (ls_option_flags*) owner_opt_obj->opts_flags)->reverse_output = true;
                return true;
            }
            else{
                return false;
            }
        }
};

command_option ls_opt_recursive = {
        ls_opt_help.opt_n = 0,
        ls_opt_help.name = "-R",
        ls_opt_help.opt_args = nullptr,
        .opt_inner_valid = [](size_t nargs, char **vargs = nullptr, Function_options* owner_opt_obj) {
            cout << "ENTERED RECURSIVE_PROCEED" <<endl;
            if (nargs == 0){
                //Mamma, I'm a crimminal
                ( (ls_option_flags*) owner_opt_obj->opts_flags)->recursive = true;
                return true;
            }
            else{
                return false;
            }
        }
};



// 1--getting pathes from args
// 2--verifying options and setting flags
// 3--sorting directories according to flags
// 3.5 -- in case of recursion expanding directories while sorting on preliminar stages
// 3.6 -- sorted vector allready can be printed with additional info
// 3.6 -- else just outputting
int Extern_LS::get_passes_from_args(size_t nargs, char **argv, vector<fs::path> *p_form_args){

    int i = 1; //argv index

    if (nargs == 1){
        p_form_args->push_back(fs::current_path());
        return 1;
    }

    char *arg_buf_ptr = argv[i];


    fs::path p; //path to directory (buffer)args_start_position


    //while first command option marker is not met in line of arguments
    while ( i<nargs && arg_buf_ptr[0] != '-'){


        p = fs::path(arg_buf_ptr);

        try
        {
            if (fs::exists(p))    // does p actually exist?
            {

                if (fs::is_directory(p) || fs::is_regular_file(p)) {
                    p_form_args->push_back(p);
                }
                else
                    cout << p << " exists, but is neither a regular file nor a directory\n";
            }
            else
                cout << p << " does not exist\n";
        }

        catch (const fs::filesystem_error& ex)
        {
            cout << ex.what() << '\n';
        }

        ++i;
        arg_buf_ptr = argv[i];
    }

    return 0; //number of OK pathes
}



int Extern_LS::process_passes_from_saved(vector<fs::path> *p_form_args, int rec_depth){

    //TODO sort vector here
    //TODO INCLUDE OPTIONS FOR REVERSING OUTPUT IN FUTURE
    sort( (*p_form_args).begin() , (*p_form_args).end() );
    // ===============INIT===========
    using vect = vector<fs::path> ;


    // ===============INIT END===========


    //iterate list of arguments get
    for(fs::path p:*p_form_args){

        try
        {
            if (fs::exists(p))    // does p actually exist?
            {


                if (fs::is_regular_file(p)) {
                    print_file_about(&p, rec_depth);
                }


                    //need to fill buffer before processing
                else if (is_directory(p))
                {
                    vect subdir_contain;

                    copy(fs::directory_iterator(p), fs::directory_iterator(), back_inserter(subdir_contain));

                    //RECURSIVE EXEC BRANCH
                    if ( ((ls_option_flags*)this->func_opts->opts_flags)->recursive ) {

                        for(int i= 0; i < rec_depth; ++i)
                            printf("   ");
                        cout << p << " is a directory containing:\n";
                        process_passes_from_saved(&subdir_contain, rec_depth + 1);
                    }

                        //linear exec branch
                    else
                    {
                        //TODO special sorting
                        sort(subdir_contain.begin(), subdir_contain.end());
                        print_dir_contain(&p, &subdir_contain, rec_depth);
                    }

                }

                else
                    cout << p << " exists, but is neither a regular file nor a directory\n";
            }
            else
                cout << p << " does not exist\n";
        }

        catch (const fs::filesystem_error& ex)
        {
            cout << ex.what() << '\n';
        }

    }

    return 0; //number of OK pathes
}


//show current directory
int Extern_LS::my_ls_inner(size_t nargs, char **argv){

    cout << "from bottom_layer MY_LS_INNER" <<endl;

    /*
    for (fs::path p : (*this->passes_to_apply)){
        //passes are there from argument line
       cout << p << endl;
    }
     */

    this->process_passes_from_saved(this->passes_to_apply);

    return 1;
}

//Overriding
int Extern_LS::call(size_t nargs, char **argv){

    if (this->search_for_help(nargs, argv)){
        this->output_help(this->help_info);
        return 1;
    }

    get_passes_from_args(nargs, argv, this->passes_to_apply);

    args_start_position += this->passes_to_apply->size();
    if (nargs == 1){
        args_start_position -= 1; // case of default directory adding
    }

    //shifting pointer to actual arguments position start
    argv += args_start_position;
    nargs -= args_start_position;
    return External_func::call(nargs, argv);
}

void inline Extern_LS::print_file_about(fs::path *path_to_print, int depth){
    for (int i=0; i<=depth; ++i)
        printf("    ");
    printf("%s  Siz=%d\n", path_to_print->filename().c_str(), (int)file_size(*path_to_print) );
    //TODO add falg-check and additional info listing
}


void inline Extern_LS::print_dir_about(fs::path *path_to_print, int depth){
    for (int i=0; i<=depth; ++i)
        printf("    ");
    printf("/%s  \n", path_to_print->filename().c_str() );
}


void inline Extern_LS::print_dir_contain(fs::path *dir, vector<fs::path> *dir_contain, int rec_depth) {

    for(int i= 0; i < rec_depth; ++i)
        printf("   ");
    cout << (*dir) << " is a directory containing:\n";

    for (fs::path subpath: (*dir_contain) ) {
        if (fs::is_directory(subpath)) {
            print_dir_about(&subpath, rec_depth + 1);
        } else if (fs::is_regular_file(subpath)) {
            print_file_about(&subpath, rec_depth + 1);
        }
    }
}

Extern_LS::Extern_LS(const string &name,
          callable_function funct_to_assign,
          Function_options *options,
          string &help_msg):
        External_func(name,
                      funct_to_assign,
                      options,
                      help_msg)
{

    this->passes_to_apply = new vector<fs::path>;
    this->func_opts->opts_flags= new ls_option_flags ;

/*
        ls_func_opts_map_ptr = new map<string, command_option*>;
        (*ls_func_opts_map_ptr)["--help"] = &ls_opt_help;
*/
    //TODO GEI IT OUT WHEN PROBLEM SOLVED

    this->func_opts->func_opts_map = new map<string, command_option*>;//ls_func_opts_map_ptr;
    (*this->func_opts->func_opts_map) ["-l"] = &ls_opt_l;
    (*this->func_opts->func_opts_map) ["--sort"] = &ls_opt_sort;
    (*this->func_opts->func_opts_map) ["-r"] = &ls_opt_revers;
    (*this->func_opts->func_opts_map) ["-R"] = &ls_opt_recursive;


}