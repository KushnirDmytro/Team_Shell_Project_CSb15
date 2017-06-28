//
// Created by d1md1m on 14.05.17.
//

#ifndef LAB_2_SHELL_EXTERNMKDIR_H
#define LAB_2_SHELL_EXTERNMKDIR_H

#include <string>
//#include <boost/filesystem.hpp>
//#include <map>
//#include <iostream>

#include "../ExternalFunc.h"


namespace extrn {


    namespace fs = boost::filesystem;



    struct GrepConfigStruct {
        fs::path *filename_to_read;
        std::string *regexp_notation;
        bool invert_filter_ = false;
        bool from_file_ = false;
        bool ignore_latter_case_ = false;
        bool regexp_ = false;
    };


    class ExternGrep : public ExternalFunc {

        GrepConfigStruct grep_flags_;

        class GREP_OptsManager : public DefaultOptionsManager {

            // ============Inner_class lvl 2
            class GREP_FILE_OptsManager : public DefaultOptionsManager {
            private:
                fs::path* filename;
            public:
                GREP_FILE_OptsManager(string name, bool* file_input_flag, fs::path* host_filename_holder);

                ~GREP_FILE_OptsManager();

                bool suboptionsAreValid(size_t nargs, char **argv) override;
            };


            class GREP_REGEXP_OptsManager : public DefaultOptionsManager {
            private:
                std::string *regexp_notation_;
            public:
                GREP_REGEXP_OptsManager(string name, bool* regexp_filter_flag, std::string *host_regexp_notation);

                ~GREP_REGEXP_OptsManager();

                bool suboptionsAreValid(size_t nargs, char **argv) override;
            };
            // ============Inner_class lvl 2 END



        private:
            GrepConfigStruct *grep_flags_;

        public:
            GREP_OptsManager(string name,
                             GrepConfigStruct *ls_flags);

            ~GREP_OptsManager();

            bool suboptionsAreValid(size_t nargs, char **argv) override;

            void clearFlags() override;
        };


    public:
        ExternGrep(const std::string &name,
                    sh_core::callable_function funct_to_assign,
                    std::string &help_msg);

        int call(size_t nargs, char **argv) override;

        ~ExternGrep();

    };
}

#endif //LAB_2_SHELL_EXTERNMKDIR_H
