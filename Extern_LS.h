//
// Created by d1md1m on 31.03.17.
//



#ifndef LAB_2_SHELL_EXTERN_LS_H
#define LAB_2_SHELL_EXTERN_LS_H


#include <sys/stat.h>

#include <sys/types.h>
#include "External_func.h"
#include "ctime"
#include <boost/algorithm/string.hpp>

namespace fs = boost::filesystem;

//==================DECLARATIONS of defaults ==============
enum ls_sorts{NAME, UNSORT, SIZE, TIME_MODIFIED, EXTENTION };

struct ls_option_flags{
    bool detailed_listing = false;
    bool recursive = false;
    bool reverse_output = false;
    bool file_props = false;
    ls_sorts sort_type = NAME;
};



//==================DECLARATIONS of defaults ==============

//general options class for LS
class LS_opts : public Defaul_options_manager{
public:
    ls_option_flags LS_flags;

public:
    LS_opts (string name,
             bool noargs_allowed = true);

    ~LS_opts();

    void clear_flags();
};



class LS_no_subopt_opt : public Defaul_options_manager {

private:
    bool* flag_to_write;

public:
    LS_no_subopt_opt(string name,
                  bool* host_flag_to_write,
                  bool noargs_allowed = true);
    bool are_suboptions_valid(size_t nargs, char **argv) override;

};



class Ls_sort_opt : public Defaul_options_manager {
private:


    map<string, ls_sorts> *sort_opts_map;
    ls_sorts *sorts;

public:

    Ls_sort_opt( string name, ls_sorts* host_sorts);
    bool are_suboptions_valid(size_t nargs, char **argv) override;
    ~Ls_sort_opt();




};




// ==================== OPTIONS ===============




class Extern_LS : public External_func{

private:

    size_t args_start_position_shift = 1;
    vector<fs::path> passes_to_apply;

public:


    //I HAD TO DO IT TO AVOID BUG
    LS_opts *ls_opts;

    //ls_option_flags flags;


    Extern_LS(const string &name,
             callable_function funct_to_assign,
             //Defaul_options_manager *options,
             string &help_msg);



    ~Extern_LS();


    // 1--getting pathes from args
    // 2--verifying options and setting flags
    // 3--sorting directories according to flags
    // 3.5 -- in case of recursion expanding directories while sorting on preliminar stages
    // 3.6 -- sorted vector allready can be printed with additional info
    // 3.6 -- else just outputting

    int get_passes_from_args(size_t nargs, char **argv, vector<fs::path> *p_form_args);

    void set_default_directory_as_pass_to_apply();

    int do_LS_job_with_vector(vector<fs::path> *p_from_args, const int rec_depth = 0);

    void print_file_about(const fs::path *path_to_print,const int depth, struct stat *fileStat);

    void print_dir_contain(const fs::path *dir,const vector<fs::path> *dir_contain,const int rec_depth);

    void clear_flags();

    const stringstream * form_permission_report_for_file(const fs::path *f, struct stat *stat_struct);

    const stringstream * form_timereport_for_file(const fs::path *f);

    void apply_sorting(vector<fs::path> *vec_to_sort);

    void print_filedata(const fs::path *path_to_print, const int depth);

    void clean_up_after_execution();
//show current directory
    int my_ls_inner(size_t nargs, char **argv);

//Overriding
    int call(size_t nargs, char **argv) override ;
};




// ==================== OPTIONS ===============

extern Extern_LS *extern_ls_obj;


#endif //LAB_2_SHELL_EXTERN_LS_H
