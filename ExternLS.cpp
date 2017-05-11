// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Created by d1md1m on 31.03.17.
//

#include "ExternLS.h"
#include <boost/lexical_cast.hpp>
#include <sys/types.h>
#include <sys/xattr.h>


#include <sys/stat.h>
#include <sys/types.h>

//TODO get it out
using namespace std;

namespace ext {





    LS_opts::LS_opts(string name,
                     bool noargs_allowed) :
            DefaultOptionsManager(name) {

        this->noargs_allowed_ = noargs_allowed;

        this->opts_map_ = new std::map<string, DefaultOptionsManager *>{
                {"-l",
                        new LS_no_subopt_opt("-l", &this->LS_flags.detailed_listing)},
                {"-r",
                        new LS_no_subopt_opt("-r", &this->LS_flags.reverse_output)},
                {"-R",
                        new LS_no_subopt_opt("-R", &this->LS_flags.recursive)},
                {"--sort",
                        new Ls_sort_opt("--sort", &this->LS_flags.sort_type)},
                {"-F",
                        new LS_no_subopt_opt("-F", &this->LS_flags.show_file_type)}
        };
    };


    LS_opts::~LS_opts() {
    }
    // bool LS_opts::suboptionsAreValid(size_t nargs_, char **argv) override;


    void LS_opts::clear_flags() {
        LS_flags.recursive = false;
        LS_flags.detailed_listing = false;
        LS_flags.reverse_output = false;
        LS_flags.show_file_type = false;
        LS_flags.sort_type = NAME;
    };


//prototype for unspecified option
    LS_no_subopt_opt::LS_no_subopt_opt(string name,
                                       bool *host_flag,
                                       bool noargs_allowed_)
            : DefaultOptionsManager(name) {
        opts_map_ = nullptr;
        noargs_allowed_ = noargs_allowed_;
        flag_to_write = host_flag;
    }

//checker for received suboptions
    bool LS_no_subopt_opt::suboptionsAreValid(size_t nargs, char **argv) {
        if (nargs == 0) {
            if (noargs_allowed_) {
                (*flag_to_write) = true;
                return true;
            } else {
                printf("Unexpected argument for %s  /n", option_name_.c_str());
                return false;
            }
        }
        else return suboptionsAreValid(nargs, argv);
    }


// option block for sorting
    Ls_sort_opt::Ls_sort_opt(string name, ls_sorts *sorts)
            : DefaultOptionsManager(name) {
        this->noargs_allowed_ = false;
        this->sort_opts_map = new map<string, ls_sorts>{
                {"U", UNSORT},
                {"S", SIZE},
                {"N", NAME},
                {"X", EXTENTION},
                {"t", TIME_MODIFIED}
        };
        this->sorts = sorts;

    };

    Ls_sort_opt::~Ls_sort_opt() {
        delete this->sort_opts_map;
    }


// suboptions validator
    bool Ls_sort_opt::suboptionsAreValid(size_t nargs, char **argv) {
        cout << "ENTERED SORT_OPTIONS" << endl;
        cout << nargs << " Args number" << endl;

        if (nargs == 0) {
            // setting defaul sorting scheme
            *sorts = NAME;
            return true;
        } else {

            cout << argv[0] << " RECEIVED AS ARG" << endl;

            if (nargs == 1) {

                string argument = string(argv[0]);

                if (sort_opts_map->find(argument) == sort_opts_map->end()) {
                    printf("ERROR argument %s is not defined for %s\n", argument.c_str(), option_name_.c_str());
                    return false;
                } else {

                    printf("found option %d\n ", sort_opts_map->at(argument));

                    *sorts = sort_opts_map->at(argument);
                    return true;
                }
            } else {
                printf("Error on argumnets number (%d) for  option --sort\n", (int) nargs);
                return false;
            }
        }
    }


    ExternLS::~ExternLS() {
        delete ls_opts;
    }


    ExternLS::ExternLS(const string &name,
                         sh_core::callable_function funct_to_assign,
                         string &help_msg) :
            ExternalFunc(name,
                          funct_to_assign,
                          help_msg) {

        //TODO make this vector static ptr
        //passes_to_apply_ = new vector<fs::path>;
        //TODO GEI IT OUT WHEN PROBLEM SOLVED
        ls_opts = new LS_opts("LS_opts_object");


    };


    void inline ExternLS::clean_up_after_execution() {
        clearFlags();
        passes_to_apply_.clear();
        args_start_position_offset_ = 1;
    }

    void inline ExternLS::set_default_directory_as_pass_to_apply() {
        sh_core::environment->dir_->refreshPath();
        passes_to_apply_.push_back(sh_core::environment->dir_->getActualPath());
        cout << "set path to apply as  ==>" << sh_core::environment->dir_->getActualPath() << endl;
        args_start_position_offset_ -= 1; //counting this data modification
    }


// 1--getting pathes from args
// 2--verifying options and setting flags
// 3--sorting directories according to flags
// 3.5 -- in case of recursion expanding directories while sorting on preliminar stages
// 3.6 -- sorted vector allready can be printed with additional info
// 3.6 -- else just outputting
    int ExternLS::get_passes_from_args(size_t nargs, char **argv, vector<fs::path> *p_form_args) {
        int i = 1; //argv index
        char *arg_buf_ptr = argv[i];

        fs::path p; //path to directory (buffer)args_start_position_offset_

        //while first command option marker is not met in line of arguments
        while (i < nargs && arg_buf_ptr[0] != '-') {

            p = fs::path(arg_buf_ptr);

            try {
                if (fs::exists(p))    // does p actually exist?
                {
                    p_form_args->push_back(p);
                } else
                    cout << p << endl << " does not exist\n";
            }

            catch (const fs::filesystem_error &ex) {
                cout << ex.what() << '\n';
            }
            ++i;
            arg_buf_ptr = argv[i];
        }

        return 0; //number of OK pathes
    }


    inline void time_correction() {
        struct tm time_struct;
        const std::time_t raw_time = timezone;
        gmtime_r(&raw_time, &time_struct);
    }


    inline void ExternLS::apply_sorting(vector<fs::path> *vec_to_sort) {
        /*
        // знаю що написана дурня і тег взагалі не про те
        bidirectional_iterator_tag<fs::path>  v_start;
        bidirectional_iterator_tag<fs::path> v_finish;
        if (this->ls_opts->LS_flags.reverse_output){
             v_start = vec_to_sort->rbegin();
             v_finish = vec_to_sort->rend();
        } else{
            v_start = vec_to_sort->begin();
            v_finish = vec_to_sort->end();
        }
    */
        switch (this->ls_opts->LS_flags.sort_type) {
            case NAME: {
                sort(vec_to_sort->begin(),
                     vec_to_sort->end(),
                     [](fs::path first, fs::path second) ->
                             bool {
                         string st1 = first.filename().string();
                         string st2 = second.filename().string();
                         std::transform(st1.begin(), st1.end(), st1.begin(),
                                        [](unsigned char c) { return std::tolower(c); });
                         std::transform(st2.begin(), st2.end(), st2.begin(),
                                        [](unsigned char c) { return std::tolower(c); });
                         return st1 < st2;
                     }
                );
                break;
            }

            case TIME_MODIFIED: {
                sort(vec_to_sort->begin(),
                     vec_to_sort->end(),
                     [](fs::path first, fs::path second) ->
                             bool {
                         return fs::last_write_time(first) < fs::last_write_time(second);
                     }
                );
                break;
            }
            case UNSORT: {
                sort(vec_to_sort->begin(),
                     vec_to_sort->end(),
                     [](fs::path first, fs::path second) ->
                             bool {
                         return false;
                     }
                );
                break;
            }
            case EXTENTION: {
                sort(vec_to_sort->begin(),
                     vec_to_sort->end(),
                     [](fs::path first, fs::path second) ->
                             bool {
                         return first.extension() < second.extension();
                     }
                );
                break;
            }

            case SIZE: //FUNPART....
            {
                //splitting non-files (directories)
                vector<fs::path>::iterator seek_first_file = partition(vec_to_sort->begin(),
                                                                       vec_to_sort->end(),
                                                                       [](fs::path elem) -> bool {
                                                                           return !fs::is_regular_file(elem);
                                                                       }
                );

                //sorting files only
                sort(seek_first_file,
                     vec_to_sort->end(),
                     [](fs::path first, fs::path second) ->
                             bool {
                         return fs::file_size(first) < fs::file_size(second);
                     }
                );

                break;
            }

                //default sorting by name_ for safety
            default: {
                sort(vec_to_sort->begin(), vec_to_sort->end(),
                     [](fs::path first, fs::path secod) ->
                             bool { return first.filename() < secod.filename(); });
                break;
            }
        }

        if (this->ls_opts->LS_flags.reverse_output) {
            reverse(vec_to_sort->begin(), vec_to_sort->end());
        }


    };


    int ExternLS::do_LS_job_with_vector( /*not const, to be sorted inside*/vector<fs::path> *p_from_args,
                                                                            const int rec_depth) {
        apply_sorting(p_from_args);
        // ===============INIT===========
        using vect = vector<fs::path>;
        // ===============INIT END===========

        if (ls_opts->LS_flags.detailed_listing) {
            time_correction();
        }

        //iterate list of arguments get
        for (fs::path p:*p_from_args) {

            try {
                if (fs::exists(p))    // does p actually exist?
                {
                    if (is_directory(p)) {

                        if (!ls_opts->LS_flags.recursive && rec_depth != 0) {
                            print_filedata(&p, rec_depth); // if we should not look into directory
                        } else {
                            vect subdir_contain;
                            copy(fs::directory_iterator(p), fs::directory_iterator(), back_inserter(subdir_contain));
                            apply_sorting(&subdir_contain);
                            for (int i = 0; i < rec_depth; ++i)
                                printf("   ");
                            cout << p << " is a directory containing:\n";
                            if (ls_opts->LS_flags.recursive)//recursive dive into directory
                                do_LS_job_with_vector(&subdir_contain, rec_depth + 1);
                            else
                                print_dir_contain(&p, &subdir_contain, rec_depth);
                        }
                    } else
                        print_filedata(&p, rec_depth);
                } else
                    cout << p << endl << " does not exist\n"; //yes doublecheck
            }

            catch (fs::filesystem_error &ex) {
                cout << ex.what() << '\n';
            }
        }
        return 0;
    }


//show current directory
    int ExternLS::my_ls_inner(size_t nargs, char **argv) {
        for (fs::path p : (passes_to_apply_)) {
            //passes are there from argument line
            cout << "FOUND PATH TO APPLY" << p << endl;
        }

        this->do_LS_job_with_vector(&passes_to_apply_);

        clean_up_after_execution();

        return 1;
    }

//Overriding
    int ExternLS::call(size_t nargs, char **argv) {

        if (searchForHelp(nargs, argv)) {
            outputHelp(help_info_);
            return 1;
        }

        get_passes_from_args(nargs, argv, &passes_to_apply_);

        if (passes_to_apply_.size() == 0) {
            set_default_directory_as_pass_to_apply();
        }

        args_start_position_offset_ += passes_to_apply_.size();

        //shifting pointer to actual arguments position start
        argv += args_start_position_offset_;
        nargs -= args_start_position_offset_;

        if (ls_opts->suboptionsAreValid(nargs, argv)) {
            cout << "problem checking" << endl;


            cout << "Detailed listing flag " << ls_opts->LS_flags.detailed_listing << endl;
            cout << "Recursive output flag " << ls_opts->LS_flags.recursive << endl;
            cout << "Reverted output flag " << ls_opts->LS_flags.reverse_output << endl;
            cout << "Sorting type " << ls_opts->LS_flags.sort_type << endl;

            return ExternalFunc::call(nargs, argv);
        } else {
            clean_up_after_execution();
            return 1;
        }
    };


    void inline ExternLS::clearFlags() {
        ls_opts->clear_flags();
    };


    inline const stringstream*
    ExternLS::formPermissionReportForFile (const fs::path *path_to_print, struct stat *fileStat) const{

        stat(path_to_print->c_str(), fileStat);

        stringstream *result = new stringstream;

        *result << " Usr:"
                << (getpwuid(fileStat->st_uid)->pw_name)
                << " Gr:"
                << (getpwuid(fileStat->st_gid)->pw_name)
                << " "
                << ((fileStat->st_mode & S_IRUSR) ? "r" : "-")
                << ((fileStat->st_mode & S_IWUSR) ? "w" : "-")
                << ((fileStat->st_mode & S_IXUSR) ? "x" : "-")
                << ((fileStat->st_mode & S_IRGRP) ? "r" : "-")
                << ((fileStat->st_mode & S_IWGRP) ? "w" : "-")
                << ((fileStat->st_mode & S_IXGRP) ? "x" : "-")
                << ((fileStat->st_mode & S_IROTH) ? "r" : "-")
                << ((fileStat->st_mode & S_IWOTH) ? "w" : "-")
                << ((fileStat->st_mode & S_IXOTH) ? "x" : "-")
                << " ";

        return result;
    }

    inline const stringstream *ExternLS::form_timereport_for_file(const fs::path *path_to_print) const{

        struct tm time_struct;

        const std::time_t raw_time = fs::last_write_time(*path_to_print) - timezone;

        gmtime_r(&raw_time, &time_struct);


        stringstream *ss = new stringstream;

        *ss << "" << 1900 + time_struct.tm_year
            << "-" << 1 + time_struct.tm_mon
            << "-" << time_struct.tm_mday
            << "  " << time_struct.tm_hour + time_struct.tm_isdst + 1
            << ":" << time_struct.tm_min
            << ":" << time_struct.tm_sec
            << time_struct.tm_zone
            << " (dst " << time_struct.tm_isdst << ')'
            << " correction " << 1 << "H"
            << endl;

        return ss;
    }


    inline void ExternLS::print_filedata(const fs::path *path_to_print, const int depth) {
        for (int i = 0; i <= depth; ++i)
            printf("    ");
        char filemark = ' ';

        struct stat fileStat;

        stat(path_to_print->c_str(), &fileStat);

        if (fileStat.st_mode & S_IFDIR) {// is directory
            filemark = '/';
            printf("%c%s \n", filemark, path_to_print->filename().c_str());
            return;
        }

        if (ls_opts->LS_flags.show_file_type) {
            if (S_ISLNK(fileStat.st_mode)) filemark = '@'; //symbolic link
            if (S_ISSOCK(fileStat.st_mode)) filemark = '='; // socket
            if (S_ISFIFO(fileStat.st_mode)) filemark = '|'; // pipe (named channel)
            if ((fileStat.st_mode & S_IXUSR) || (fileStat.st_mode & S_IXGRP) || (fileStat.st_mode & S_IXOTH))
                filemark = '*'; //executable
        }

        printf("%c%s \n", filemark, path_to_print->filename().c_str());

        if (ls_opts->LS_flags.detailed_listing)
            print_file_about(path_to_print, depth, &fileStat);

    }


    inline void ExternLS::print_file_about(const fs::path *path_to_print, const int depth, struct stat *fileStat) {

        const stringstream *time_stream = form_timereport_for_file(path_to_print);
        const stringstream *permissions_stream = formPermissionReportForFile(path_to_print, fileStat);

        printf(" Perm: %s Ext: [%s] size%lu B  time_written  %s\n",
               permissions_stream->str().c_str(),
               path_to_print->extension().c_str(),
               fileStat->st_size,
               time_stream->str().c_str());

        delete permissions_stream;
        delete time_stream;
    }


    inline void
    ExternLS::print_dir_contain(const fs::path *dir, const vector<fs::path> *dir_contain, const int rec_depth) {

        for (int i = 0; i < rec_depth; ++i)
            printf("   ");
        cout << (*dir) << " CONTAINS:\n";

        for (const fs::path subpath: (*dir_contain))
            print_filedata(&subpath, rec_depth + 1);

    }


}