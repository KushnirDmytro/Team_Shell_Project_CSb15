//
// Created by d1md1m on 31.03.17.
//



#ifndef LAB_2_SHELL_EXTERN_LS_H
#define LAB_2_SHELL_EXTERN_LS_H



#include "External_func.h"
#include "ctime"

//==================DECLARATIONS of defaults ==============
enum ls_sorts{NAME, UNSORT, SIZE, TIME_MODIFIED, EXTENTION };

struct ls_option_flags{
    bool detailed_listing = false;
    bool recursive = false;
    bool reverse_output = false;
    ls_sorts sort_type;
};


//==================DECLARATIONS of defaults ==============

//general options class for LS
class LS_opts : public Options{
public:
    ls_option_flags LS_flags;

public:
    LS_opts (string name,
             bool noargs_allowed = true);

    ~LS_opts();

    void clear_flags();
};





class LS_simple_opt : public Options {
public:

    bool* flag_to_write;

    LS_simple_opt(string name,
                  bool* host_flag_to_write,
                  bool noargs_allowed = true);
    bool are_suboptions_valid(size_t nargs, char **argv) override;

};



class Ls_sort_opt : public Options {
private:

    map<string, ls_sorts> *sort_opts_map;
    ls_sorts *sorts;


public:
    Ls_sort_opt( string name, ls_sorts* host_sorts);

    ~Ls_sort_opt();

    bool are_suboptions_valid(size_t nargs, char **argv) override;

};


// ==================== OPTIONS ===============




class Extern_LS : public External_func{

private:

    size_t args_start_position_shift = 1;
    vector<fs::path> *passes_to_apply;
    ls_option_flags flags;

public:
    Extern_LS(const string &name,
             callable_function funct_to_assign,
             //Options *options,
             string &help_msg);



    ~Extern_LS();



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

    void set_default_directory();

    int process_passes_from_saved(vector<fs::path> *p_form_args, int rec_depth = 0);

    void print_file_about(fs::path *path_to_print, int depth);

    void print_dir_about(fs::path *path_to_print, int depth);

    void print_dir_contain(fs::path *dir, vector<fs::path> *dir_contain, int rec_depth);

    void clear_flags();

    void clean_up_after_execution();
//show current directory
    int my_ls_inner(size_t nargs, char **argv);

/*

 //NOT FAIL
 ls /home/d1md1m/CLionProjects/Lab_2_shell/cmake-build-debug /home/d1md1m/CLionProjects/Lab_2_shell --sort -l -R


 ls /home/d1md1m/CLionProjects/Lab_2_shell/cmake-build-debug --sort -l -R


 //FAIL
ls /home/d1md1m/CLionProjects/Lab_2_shell/cmake-build-debug --sort N -l -R


  */


//Overriding
    int call(size_t nargs, char **argv) override ;
};




// ==================== OPTIONS ===============

extern Extern_LS *extern_ls_obj;


#endif //LAB_2_SHELL_EXTERN_LS_H
