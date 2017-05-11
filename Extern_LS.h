//
// Created by d1md1m on 31.03.17.
//



#ifndef LAB_2_SHELL_EXTERN_LS_H
#define LAB_2_SHELL_EXTERN_LS_H


#include <sys/stat.h>

#include <sys/types.h>
#include "ExternalFunc.h"
#include "ctime"

//#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string.hpp>

namespace ext {

    namespace fs = boost::filesystem;

//==================DECLARATIONS of defaults ==============
    enum ls_sorts {
        NAME, UNSORT, SIZE, TIME_MODIFIED, EXTENTION
    };

    struct ls_option_flags {
        bool detailed_listing = false;
        bool recursive = false;
        bool reverse_output = false;
        bool file_props = false;
        ls_sorts sort_type = NAME;
    };



//==================DECLARATIONS of defaults ==============

//general options class for LS
    class LS_opts : public DefaultOptionsManager {
    public:
        ls_option_flags LS_flags;

    public:
        LS_opts(string name,
                bool noargs_allowed = true);

        ~LS_opts();

        void clear_flags();

        //bool are_suboptions_valid(size_t nargs_, char **argv) override;

//    bool suboptionS_arguments_validation(DefaultOptionsManager* opt_to_check, vector<string>* arg_buf) override;

    };


    class LS_no_subopt_opt : public DefaultOptionsManager {
    public:

        bool *flag_to_write;

        LS_no_subopt_opt(string name,
                         bool *host_flag_to_write,
                         bool noargs_allowed = true);

        bool are_suboptions_valid(size_t nargs, char **argv) override;

    };


    class Ls_sort_opt : public DefaultOptionsManager {
    private:


    public:


        map<string, ls_sorts> *sort_opts_map;
        ls_sorts *sorts;

        Ls_sort_opt(string name, ls_sorts *host_sorts);

        ~Ls_sort_opt();

        bool are_suboptions_valid(size_t nargs, char **argv) override;


    };




// ==================== OPTIONS ===============




    class Extern_LS : public ExternalFunc {

    private:

        size_t args_start_position_shift = 1;
        vector<fs::path> *passes_to_apply;

    public:


        //I HAD TO DO IT TO AVOID BUG
        LS_opts *ls_opts;

        //ls_option_flags flags;


        Extern_LS(const string &name,
                  sh_core::callable_function funct_to_assign,
                //DefaultOptionsManager *options,
                  string &help_msg);


        ~Extern_LS();


//TODO replace by function-comparator
        int sort_vect(vector<fs::path> *vec_of_args, bool comparator) {
            return 0;
        }


//TODO create structure for args and info
// TODO specify falgs
        int collect_additional_info(vector<fs::path> *vec_of_args, vector<string> *infos, char *flags) {
            return 0;
        }


        // 1--getting pathes from args
        // 2--verifying options and setting flags
        // 3--sorting directories according to flags
        // 3.5 -- in case of recursion expanding directories while sorting on preliminar stages
        // 3.6 -- sorted vector allready can be printed with additional info
        // 3.6 -- else just outputting

        int get_passes_from_args(size_t nargs, char **argv, vector<fs::path> *p_form_args);

        void set_default_directory_as_pass_to_apply();

        int do_LS_job_with_vector(vector<fs::path> *p_from_args, const int rec_depth = 0);

        void print_file_about(const fs::path *path_to_print, const int depth, struct stat *fileStat);

        void print_dir_contain(const fs::path *dir, const vector<fs::path> *dir_contain, const int rec_depth);

        void clear_flags();

        const stringstream *form_permission_report_for_file(const fs::path *f, struct stat *stat_struct);

        const stringstream *form_timereport_for_file(const fs::path *f);

        void apply_sorting(vector<fs::path> *vec_to_sort);

        void print_filedata(const fs::path *path_to_print, const int depth);

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
        int call(size_t nargs, char **argv) override;
    };




// ==================== OPTIONS ===============

   // extern Extern_LS *extern_ls_obj;

}
#endif //LAB_2_SHELL_EXTERN_LS_H
