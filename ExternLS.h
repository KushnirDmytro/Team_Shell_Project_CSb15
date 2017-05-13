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

    const ls_sorts DEFAULT_SORT = NAME;

    struct ls_option_flags {
        bool detailed_listing_ = false;
        bool recursive_ = false;
        bool reverse_output_ = false;
        bool show_file_type = false;
        ls_sorts sort_type_ = DEFAULT_SORT;
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

        void clearFlags() override;

        //bool suboptionsAreValid(size_t nargs_, char **argv) override;

//    bool doesSuboptionSArgumentsAreValid(DefaultOptionsManager* opt_to_check, vector<string>* arg_buf) override;

    };


    class Ls_sort_opt : public DefaultOptionsManager {
    private:

    public:

        std::map<string, ls_sorts> *sort_opts_map_;
        //this is a link to real sorting flag, available only via map using
        ls_sorts *soring_should_be_applied_;

        Ls_sort_opt(string name, ls_sorts *host_sorts);

        ~Ls_sort_opt();

        bool suboptionsAreValid(size_t nargs, char **argv) override;


    };




// ==================== OPTIONS ===============




    class ExternLS : public ExternalFunc {

    private:

        size_t args_start_position_offset_ = 1;
        std::vector<fs::path> passes_to_apply_;

    public:

        //I HAD TO DO IT TO AVOID BUG
        LS_opts *ls_opts;


        ExternLS(const string &name,
                  sh_core::callable_function funct_to_assign,
                //DefaultOptionsManager *options,
                  string &help_msg);


        ~ExternLS();


        // 1--getting pathes from args
        // 2--verifying options and setting flags
        // 3--sorting directories according to flags
        // 3.5 -- in case of recursion expanding directories while sorting on preliminar stages
        // 3.6 -- sorted vector allready can be printed with additional info
        // 3.6 -- else just outputting

        int extractPassesFromArgs(size_t nargs, char **argv, std::vector<fs::path> *p_form_args);

        void setCurrentDirectoryAsPassToApply();

        int do_LS_job_with_vector(std::vector<fs::path> *p_from_args, const int rec_depth = 0);


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
    private:

        void printFileAbout(const fs::path *path_to_print, const int depth, struct stat *file_Stat) const;

        void printDirContain(const fs::path *dir, const std::vector<fs::path> *dir_contain, const int rec_depth);

        void printAllAboutFile(const fs::path *path_to_print, const int depth) const;

        //TODO get it AWAY!
        //void clearFlags() ;

        void cleanUpAllAfterExecution();

        void performTimeCorrection() const;

        const std::stringstream *formPermissionReportForFile(const fs::path *f, struct stat *stat_struct) const;

        const std::stringstream *FormTimereportForFile(const fs::path *f) const;

        void applySorting(std::vector<fs::path> *vec_to_sort) const;

    };




// ==================== OPTIONS ===============

   // extern ExternLS *extern_ls_obj;

}
#endif //LAB_2_SHELL_EXTERN_LS_H
