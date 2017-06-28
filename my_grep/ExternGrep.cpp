// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Created by d1md1m on 14.05.17.
//


#include <string>
#include <iostream>

#include "../Shell_core/EmbeddedFunc.h"
#include "../ExternalFunc.h"
#include "ExternGrep.h"


#include <boost/algorithm/string.hpp>


namespace extrn{


    ExternGrep::ExternGrep(const string &name,
                sh_core::callable_function funct_to_assign,
                string &help_msg):ExternalFunc(name,
            funct_to_assign,
            help_msg,
    new GREP_OptsManager("GrepOptsObject", &grep_status_)){

        noargs_allowed_ = false;
    }


    void ExternGrep::GREP_OptsManager::clearFlags() {
        grep_flags_->regexp_ = false;
        grep_flags_->from_file_ = false;
        grep_flags_->ignore_latter_case_ = false;
        grep_flags_->invert_filter_ = false;
        grep_flags_->filenames_to_read = new std::vector<fs::path>;
        grep_flags_->filter_pattern_ = new string("");
    }

    bool ExternGrep::GREP_OptsManager::GREP_FILE_OptsManager::suboptionsAreValid(size_t nargs, char **argv) {
        printf("GREP_FILE_ARGUMENTS_PLACEHOLDER\n");
        return true;
    };


    bool ExternGrep::GREP_OptsManager::GREP_REGEXP_OptsManager::suboptionsAreValid(size_t nargs, char **argv){
        printf("GREP_REGEXP_ARGUMENTS_PLACEHOLDER\n");
        return true;
    };

    bool ExternGrep::GREP_OptsManager::suboptionsAreValid(size_t nargs, char **argv){
        printf("GREP_OPTS_ARGUMENTS_PLACEHOLDER\n"); // TODO consider if it is needed at all
        return true;
    };

    ExternGrep::GREP_OptsManager::GREP_FILE_OptsManager::
    GREP_FILE_OptsManager(string name, bool* file_input_flag, std::vector<fs::path>* host_filename_holder)
            : DefaultOptionsManager(name, file_input_flag) {
        noargs_allowed_ = false;
        filenames = host_filename_holder;
        // just duplication to ease access and enforce data integrity
    };
    ExternGrep::GREP_OptsManager::GREP_FILE_OptsManager::
    ~GREP_FILE_OptsManager(){    };


    ExternGrep::GREP_OptsManager::GREP_REGEXP_OptsManager::
    GREP_REGEXP_OptsManager(string name,  bool* file_input_flag, std::string* host_regexp_notation)
            : DefaultOptionsManager(name, file_input_flag) {
        noargs_allowed_ = false;
        regexp_notation_ = host_regexp_notation;
    };
    ExternGrep::GREP_OptsManager::GREP_REGEXP_OptsManager::
    ~GREP_REGEXP_OptsManager(){    };




    ExternGrep::GREP_OptsManager::GREP_OptsManager(std::string name,
                   GrepConfigStruct *grep_flags) :
            DefaultOptionsManager(name,
                                  new bool,
                                  new std::map<std::string, DefaultOptionsManager *>{
                                          {"-v", new DefaultOptionsManager("--invert-match",
                                                                           &grep_flags->invert_filter_)},

                                          {"--invert-match", new DefaultOptionsManager("--invert-match",
                                                                                       &grep_flags->invert_filter_)},

                                          {"-i", new DefaultOptionsManager("--ignore-case",
                                                                           &grep_flags->ignore_latter_case_)},

                                          {"--ignore-case", new DefaultOptionsManager("--ignore-case",
                                                                                      &grep_flags->ignore_latter_case_)},

                                          {"--regexp=", new GREP_REGEXP_OptsManager("--regexp=",
                                                                                    &grep_flags->regexp_,
                                                                                    grep_flags->filter_pattern_)},

                                          {"--file=",  new GREP_FILE_OptsManager("--file=",
                                                                                 &grep_flags->from_file_,
                                                                                 grep_flags->filenames_to_read )}
                                  }) {
        grep_flags_ = grep_flags;
    };

    ExternGrep::GREP_OptsManager::~GREP_OptsManager(){    };



    int  ExternGrep::call(size_t nargs, char **argv){
        initialNargs_ = nargs;
        initialVargs_ = argv;
        printf("PLACEHOLDER 4 GREP\n");
        if (needToPrintHelp(nargs, argv)) {
            outputHelp(help_info_);
            return EXIT_SUCCESS;
        }


        int result_code = extrn::ExternalFunc::call(nargs, argv);

//        int result_code = sh_core::EmbeddedFunc::call(nargs, argv);
//        return result_code;
        return EXIT_SUCCESS;
    };

    ExternGrep::~ExternGrep(){
    };






    bool ExternGrep::regexpFilter(std::string *arg){
        std::string candidate;// = new std::string;
        if (grep_status_.ignore_latter_case_){
            candidate = boost::algorithm::to_lower_copy(*arg);
        } else{
            candidate = *arg;
        }
        // inversion counted here
        bool check_result = (
                grep_status_.invert_filter_ ^
                (candidate.find(grep_status_.filter_pattern_->c_str()) != std::string::npos)
        );

        if (check_result){
            printf("%s\n", (*arg).c_str());
        }
        return check_result;
    };

    // TODO reading from files
    // TODO prepearing then filtering
    // TODO REGEXP
    // TODO ANALYSE INPUT
    int ExternGrep::myGrepStaticLauncher(size_t nargs, char **argv) {

        vector<std::string> resultVector;

        std::string filteringBuffer;

        if (this->grep_status_.filenames_to_read){

        }
        else {
            while (std::cin.good()){
                getline(std::cin, filteringBuffer);
               // if (filteringBuffer.find(this))
            }
        }
//
//
//
//        std::ifstream fileStream();
//        if (!fileStream.good()){
//            perror("failed to create stream from file\n");
//            return EXIT_FAILURE;
//        }


        boost::system::error_code error_code;
        fs::path pathBuf;
        for(int i = 1; i < nargs; ++i) {
            pathBuf = fs::current_path() ;
            pathBuf /= fs::path(argv[i]);
            if (fs::exists(pathBuf)){
                printf("[%s] already exists, skipping it\n", pathBuf.c_str());
                continue;
            }
            if ( ! fs::create_directory(pathBuf, error_code)) {
                perror(error_code.message().c_str());
            }else{
                printf("directory [%s] successfully created \n", pathBuf.c_str());
            }
        }

        return 0;
    }



}