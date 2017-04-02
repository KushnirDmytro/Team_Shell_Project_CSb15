//
// Created by d1md1m on 31.03.17.
//

#include "Extern_LS.h"

extern int my_ls(size_t nargs, char **argv)
{
    return extern_ls_obj->my_ls_inner(nargs, argv);
    //my_ls_inner(nargs, argv);
}


//OBSOLETE (global help check on higher lvl
command_option ls_opt_help{
        ls_opt_help.opt_n = 0,
        ls_opt_help.name = "Help",
        ls_opt_help.opt_args = nullptr,
        .opt_inner_valid = [](size_t nargs, char** vargs = nullptr){
            return nargs == 0;
        }
};
command_option ls_opt_l{
        ls_opt_help.opt_n = 0,
        ls_opt_help.name = "-l",
        ls_opt_help.opt_args = nullptr,
        .opt_inner_valid = [](size_t nargs, char** vargs = nullptr){
            return nargs == 0;
        }
};
command_option ls_opt_sort{

        ls_opt_help.opt_n = 0,
        ls_opt_help.name = "--sort",
        ls_opt_help.opt_args = nullptr,
        .opt_inner_valid = [](size_t nargs, char **vargs = nullptr) {
            return nargs == 0;}
};

command_option ls_opt_revers{
        ls_opt_help.opt_n = 0,
        ls_opt_help.name = "-r",
        ls_opt_help.opt_args = nullptr,
        .opt_inner_valid = [](size_t nargs, char **vargs = nullptr) {
            return nargs == 0;
        }
};

command_option ls_opt_recursive{

        ls_opt_help.opt_n = 0,
        ls_opt_help.name = "-R",
        ls_opt_help.opt_args = nullptr,
        .opt_inner_valid = [](size_t nargs, char **vargs = nullptr) {
            return nargs == 0;
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