//
// Created by d1md1m on 31.03.17.
//

#include "Extern_LS.h"

extern int my_ls(size_t nargs, char **argv)
{
    return extern_ls_obj->my_ls_inner(nargs, argv);
}