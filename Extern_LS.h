//
// Created by d1md1m on 31.03.17.
//



/*

Здається у  вигляді посилання на Гіт проекту.

        Прим. Джерело (авторство) комітів не являєтсья визначним для авторства проекту (вагома його частина була виконана до першого комміту та/або на інших репозиторіях, зокрема зовнішні ф-ї).

https://github.com/Lewbolew/Lab_2_shell

Команда виконавців (внески у різних частинах, відповідно до виконаного обсягу роботи):
Кушнір Дмитро
Ковальчук Богдан
Петришак Богдан


прим.:"
Олег 15:19

Але туди таки закиньте — я Вам зразу гарантую, що я забуду про цю розмову, якщо там не буде :=)

"
*/

#ifndef LAB_2_SHELL_EXTERN_LS_H
#define LAB_2_SHELL_EXTERN_LS_H



#include "External_func.h"


extern command_option ls_opt_help;
extern command_option ls_opt_l;
extern command_option ls_opt_sort;
extern command_option ls_opt_revers;
extern command_option ls_opt_recursive;


enum ls_sorts{NAME, UNSORT, SIZE, TIME_MODIFIED, EXTENTION };


struct ls_option_flags: func_opts_flags{
    bool detailed_listing = false;
    bool recursive = false;
    bool reverse_output = false;
    ls_sorts sort_type;
};


class Extern_LS : public External_func{

private:

    size_t args_start_position = 1;
    vector<fs::path> *passes_to_apply;


public:
    Extern_LS(const string &name,
             callable_function funct_to_assign,
             Function_options *options,
             string &help_msg);


    ~Extern_LS(){
        delete this->passes_to_apply;
    }




//TODO replace by function-comparator
    int sort_vect(vector<fs::path> *vec_of_args, bool comparator){
        return 0;
    }


//TODO create structure for args and info
// TODO specify falgs
    int collect_additional_info(vector<fs::path> *vec_of_args, vector<string> *infos, char* flags){
        return 0;
    }





    // 1--getting pathes from args
    // 2--verifying options and setting flags
    // 3--sorting directories according to flags
    // 3.5 -- in case of recursion expanding directories while sorting on preliminar stages
    // 3.6 -- sorted vector allready can be printed with additional info
    // 3.6 -- else just outputting

    int get_passes_from_args(size_t nargs, char **argv, vector<fs::path> *p_form_args);


    int process_passes_from_saved(vector<fs::path> *p_form_args, int rec_depth = 0);


    void print_file_about(fs::path *path_to_print, int depth);

    void print_dir_about(fs::path *path_to_print, int depth);


    void print_dir_contain(fs::path *dir, vector<fs::path> *dir_contain, int rec_depth);




//show current directory
    int my_ls_inner(size_t nargs, char **argv);

/*

 //NOT FAIL
 ls /home/d1md1m/CLionProjects/Lab_2_shell/cmake-build-debug /home/d1md1m/CLionProjects/Lab_2_shell --sort -l -R


 ls /home/d1md1m/CLionProjects/Lab_2_shell/cmake-build-debug --sort -l -R


 //FAIL
ls /home/d1md1m/CLionProjects/Lab_2_shell/cmake-build-debug /home/d1md1m/CLionProjects/Lab_2_shell --sort N -l -R


  */


//Overriding
    int call(size_t nargs, char **argv) override ;
};


extern Extern_LS *extern_ls_obj;


#endif //LAB_2_SHELL_EXTERN_LS_H
