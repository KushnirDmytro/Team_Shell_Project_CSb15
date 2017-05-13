//
// Created by d1md1m on 31.03.17.
//
#ifndef LAB_2_SHELL_EXTERN_LS_H
#define LAB_2_SHELL_EXTERN_LS_H

#include <sys/stat.h>
#include <sys/types.h>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <map>

#include "ExternalFunc.h"
#include "ctime"

#include "Shell_core/EmbeddedFunc.h"


namespace ext {

    namespace fs = boost::filesystem;

//==================DECLARATIONS of defaults ==============
    enum ls_sorts {
        NAME, UNSORT, SIZE, TIME_MODIFIED, EXTENTION
    };

    const ls_sorts DEFAULT_SORT = NAME;

    struct LsFlagsStruct {
        bool detailed_listing_ = false;
        bool recursive_ = false;
        bool reverse_output_ = false;
        bool show_file_type = false;
        ls_sorts sort_type_ = DEFAULT_SORT;
    };

//==================DECLARATIONS of defaults ==============


    class ExternLS;
    class LS_OptsManager;
    class LsSortOptsManager;


    class ExternLS : public ExternalFunc {

// ==================== OPTIONS ===============
        //general opt manager class
        class LS_OptsManager : public DefaultOptionsManager {

            // ============Inner_class lvl 2
            class LsSortOptsManager : public DefaultOptionsManager {

            private:
                map<string, ls_sorts> *sort_opts_map_;
                //this is a link to real sorting flag, available only via map using
                ls_sorts *soring_should_be_applied_;
            public:
                LsSortOptsManager(string name, ls_sorts *host_sorts);

                ~LsSortOptsManager();

                bool suboptionsAreValid(size_t nargs, char **argv) override;
            };
            // ============Inner_class lvl 2


        private:
            LsFlagsStruct *ls_flags_; // It is not a place to store it, just a link to handle
            // actually this pretty obvious solution made my day...

        public:
            LS_OptsManager(string name,
                           LsFlagsStruct *ls_flags,
                           bool noargs_allowed = true);

            ~LS_OptsManager();

            void clearFlags() override;
        };
// ==================== OPTIONS ===============



    private:
        size_t args_start_position_offset_ = 1;
        std::vector<fs::path> passes_to_apply_;
        LsFlagsStruct ls_flags;

    public:

        ExternLS(const string &name,
                  sh_core::callable_function funct_to_assign,
                  string &help_msg);


        virtual ~ExternLS();

        void setCurrentDirectoryAsPassToApply();

//show current directory
        int my_ls_inner(size_t nargs, char **argv);


//Overriding
        int call(size_t nargs, char **argv) override;

    private:
        int doLsJobWithVector(std::vector<fs::path> *p_from_args, const int rec_depth = 0);

        int extractPassesFromArgs(size_t nargs, char **argv, std::vector<fs::path> *p_form_args);

        void printFileAbout(const fs::path *path_to_print, const int depth, const struct stat *file_Stat) const;

        void printDirContain(const fs::path *dir, const std::vector<fs::path> *dir_contain, const int rec_depth);

        void printAllAboutFile(const fs::path *path_to_print, const int depth) const;

        void cleanUpAllAfterExecution();

        void performTimeCorrection() const;

        const std::stringstream *formPermissionReportForFile(const struct stat *stat_struct) const;

        const std::stringstream *formTimereportForFile(const fs::path *f) const;

        void applySorting(std::vector<fs::path> *vec_to_sort) const;

    };

}
#endif //LAB_2_SHELL_EXTERN_LS_H
