//
// Created by d1md1m on 31.03.17.
//

#include "Extern_LS.h"

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
        .opt_inner_valid = [](size_t nargs, char** vargs = nullptr, External_func *extern_ls_obj){
            return nargs == 0;
        }
};


command_option ls_opt_l{
        ls_opt_help.opt_n = 0,
        ls_opt_help.name = "-l",
        ls_opt_help.opt_args = nullptr,
        .opt_inner_valid = [](size_t nargs, char** vargs = nullptr, External_func *external_func){
            if (nargs == 0){
                //Mamma, I'm a crimminal
                ( (ls_option_flags*) external_func->opts_flags)->detailed_listing = true;
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
        .opt_inner_valid = [](size_t nargs, char **vargs = nullptr, External_func *external_func) {
            if (nargs == 0){
                //Mamma, I'm a crimminal
                ( (ls_option_flags*) external_func->opts_flags)->sort_type = NAME;
                return true;
            }
            else{
                if (nargs == 1) {
                    if ( !strcmp(vargs[0], "U") ){
                        ( (ls_option_flags*) external_func->opts_flags)->sort_type = UNSORT;
                        return true;
                    }
                    else if( !strcmp(vargs[0], "S") ){
                        ( (ls_option_flags*) external_func->opts_flags)->sort_type = SIZE;
                        return true;
                    }
                    else if( !strcmp(vargs[0], "t") ){
                        ( (ls_option_flags*) external_func->opts_flags)->sort_type = TIME_MODIFIED;
                        return true;
                    }
                    else if( !strcmp(vargs[0], "X") ){
                        ( (ls_option_flags*) external_func->opts_flags)->sort_type = EXTENTION;
                        return true;
                    }
                    else if( !strcmp(vargs[0], "N") ){
                        ( (ls_option_flags*) external_func->opts_flags)->sort_type = NAME;
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
        .opt_inner_valid = [](size_t nargs, char **vargs = nullptr,External_func *extern_ls_obj) {
            if (nargs == 0){
                //Mamma, I'm a crimminal
                ( (ls_option_flags*) extern_ls_obj->opts_flags)->reverse_output = true;
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
        .opt_inner_valid = [](size_t nargs, char **vargs = nullptr, External_func *extern_ls_obj) {
            if (nargs == 0){
                //Mamma, I'm a crimminal
                ( (ls_option_flags*) extern_ls_obj->opts_flags)->recursive = true;
                return true;
            }
            else{
                return false;
            }
        }
};



/*

options_struct ls_opts = {
        ls_opts.func_opts_map = {
                {"--help", ls_opt_help},
                {"-h", ls_opt_help},
                {"-l", ls_opt_l},
                {"--sort", ls_opt_sort},
                {"-r", ls_opt_revers},
                {"-R", ls_opt_recursive}
                // ,{"", ls_opt_},
        }
};
 */