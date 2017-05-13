//
// Created by d1md1m on 11.05.17.
//

#ifndef LAB_2_SHELL_CORE_LIB_DECL_H
#define LAB_2_SHELL_CORE_LIB_DECL_H

namespace ext {
    int my_ls(size_t nargs, char **args);
}

namespace sh_core {


//=============FUNCTIONS AND STRUCTURES DECLARATIONS=============


    int my_cd(size_t nargs, char **args);

    int my_pwd(size_t nargs, char **args);

    int my_help(size_t nargs, char **args);

    int my_exit(size_t nargs, char **args);

    int my_sh(size_t nargs, char **args);

/*
    Embedded_func *my_shell_fileinterpreter;
    Embedded_func *my_pwd_obj;
    Embedded_func *my_cd_obj;
    Embedded_func *my_help_obj;
    Embedded_func *my_exit_obj;

    */
}

#endif //LAB_2_SHELL_CORE_LIB_DECL_H
