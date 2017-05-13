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
                     LsFlagsStruct *ls_flags,
                     bool noargs_allowed) :
            DefaultOptionsManager(name,
                                  new bool,
                                  new std::map<string, DefaultOptionsManager *>{
                    {"-l", new DefaultOptionsManager("-l", &ls_flags->detailed_listing_)},
                    {"-r", new DefaultOptionsManager("-r", &ls_flags->reverse_output_)},
                    {"-R", new DefaultOptionsManager("-R", &ls_flags->recursive_)},
                    {"--sort", new Ls_sort_opt("--sort", &ls_flags->sort_type_)},
                    {"-F",  new DefaultOptionsManager("-F", &ls_flags->show_file_type)}
            }) {
        ls_flags_ = ls_flags;
        noargs_allowed_ = noargs_allowed;

    };


    LS_opts::~LS_opts() {
    }


    void LS_opts::clearFlags() {
        ls_flags_->recursive_ = false;
        ls_flags_->detailed_listing_ = false;
        ls_flags_->reverse_output_ = false;
        ls_flags_->show_file_type = false;
        ls_flags_->sort_type_ = DEFAULT_SORT;
    };


// option block for sorting
    Ls_sort_opt::Ls_sort_opt(string name, ls_sorts *sorts)
            : DefaultOptionsManager(name) {
        noargs_allowed_ = false;
        sort_opts_map_ = new map<string, ls_sorts>{
                {"U", UNSORT},
                {"S", SIZE},
                {"N", NAME},
                {"X", EXTENTION},
                {"t", TIME_MODIFIED}
        };
        soring_should_be_applied_ = sorts;
    };

    Ls_sort_opt::~Ls_sort_opt() {
        delete sort_opts_map_;
    }


// suboptions validator
    bool Ls_sort_opt::suboptionsAreValid(size_t nargs, char **argv) {
        cout << "ENTERED SORT_OPTIONS" << endl;
        cout << nargs << " Args number" << endl;

        if (nargs == 0) {
            // setting defaul sorting scheme
            *soring_should_be_applied_ = DEFAULT_SORT;
            return true;
        } else {

            cout << argv[0] << " RECEIVED AS ARG" << endl;

            if (nargs == 1) {

                string argument = string(argv[0]);

                if (sort_opts_map_->find(argument) == sort_opts_map_->end()) {
                    printf("ERROR argument %s is not defined for %s\n", argument.c_str(), option_name_.c_str());
                    return false;
                } else {

                    printf("found option %d\n ", sort_opts_map_->at(argument));

                    *soring_should_be_applied_ = sort_opts_map_->at(argument);
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

        //TODO GEI IT OUT WHEN PROBLEM SOLVED
        ls_opts = new LS_opts("LS_opts_object",  &this->ls_flags);

    };


    void inline ExternLS::cleanUpAllAfterExecution() {
        clearFlags(ls_opts);
        //clearFlags(ls_opts);
        passes_to_apply_.clear();
        args_start_position_offset_ = 1;
    }

    void inline ExternLS::setCurrentDirectoryAsPassToApply() {

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
    int ExternLS::extractPassesFromArgs(size_t nargs, char **argv, vector<fs::path> *p_form_args) {
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


    inline void ExternLS::performTimeCorrection() const{
        //for first file not to have empty time-data
        struct tm time_struct;
        const std::time_t raw_time = timezone;

        gmtime_r(&raw_time, &time_struct);
    }


    inline void ExternLS::applySorting(vector<fs::path> *vec_to_sort) const{
        /*
        // знаю що написана дурня і тег взагалі не про те
        bidirectional_iterator_tag<fs::path>  v_start;
        bidirectional_iterator_tag<fs::path> v_finish;
        if (this->ls_opts->ls_flags_.reverse_output_){
             v_start = vec_to_sort->rbegin();
             v_finish = vec_to_sort->rend();
        } else{
            v_start = vec_to_sort->begin();
            v_finish = vec_to_sort->end();
        }
    */
        switch (ls_opts->ls_flags_->sort_type_) {
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
                     [](fs::path first, fs::path second) -> bool {
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

        if (ls_opts->ls_flags_->reverse_output_) {
            reverse(vec_to_sort->begin(), vec_to_sort->end());
        }


    };


    int ExternLS::do_LS_job_with_vector(
            /*not const, to be sorted inside*/
            vector<fs::path> *p_from_args,
            const int rec_depth) {

        applySorting(p_from_args);
        // ===============INIT===========
        using vect = vector<fs::path>;
        // ===============INIT END===========

        if (ls_opts->ls_flags_->detailed_listing_) {
            performTimeCorrection();
        }

        //iterate list of arguments get
        for (fs::path p:*p_from_args) {

            try {
                if (fs::exists(p))    // does p actually exist?
                {
                    if (is_directory(p)) {

                        if (!ls_opts->ls_flags_->recursive_ && rec_depth != 0) {
                            printAllAboutFile(&p, rec_depth); // if we should not look into directory
                        } else {
                            vect subdir_contain;
                            copy(fs::directory_iterator(p), fs::directory_iterator(), back_inserter(subdir_contain));
                            applySorting(&subdir_contain);
                            for (int i = 0; i < rec_depth; ++i)
                                printf("   ");
                            cout << p << " is a directory containing:\n";
                            if (ls_opts->ls_flags_->recursive_)//recursive_ dive into directory
                                do_LS_job_with_vector(&subdir_contain, rec_depth + 1);
                            else
                                printDirContain(&p, &subdir_contain, rec_depth);
                        }
                    } else
                        printAllAboutFile(&p, rec_depth);
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

        do_LS_job_with_vector(&passes_to_apply_);

        cleanUpAllAfterExecution();

        return 1;
    }

//Overriding
    int ExternLS::call(size_t nargs, char **argv) {

        if (searchForHelp(nargs, argv)) {
            outputHelp(help_info_);
            return 1;
        }

        extractPassesFromArgs(nargs, argv, &passes_to_apply_);

        if (passes_to_apply_.size() == 0) {
            setCurrentDirectoryAsPassToApply();
        }

        args_start_position_offset_ += passes_to_apply_.size();

        //shifting pointer to actual arguments position start
        argv += args_start_position_offset_;
        nargs -= args_start_position_offset_;

        if (ls_opts->suboptionsAreValid(nargs, argv)) {
            cout << "problem checking" << endl;


            cout << "Detailed listing flag " << ls_opts->ls_flags_->detailed_listing_ << endl;
            cout << "Recursive output flag " << ls_opts->ls_flags_->recursive_ << endl;
            cout << "Reverted output flag " << ls_opts->ls_flags_->reverse_output_ << endl;
            cout << "Sorting type " << ls_opts->ls_flags_->sort_type_ << endl;

            return ExternalFunc::call(nargs, argv);
        } else {
            cleanUpAllAfterExecution();
            return 1;
        }
    };

/*
    void inline ExternLS::clearFlags() {
        ls_opts->clearFlags();
    };
*/

    inline const stringstream*
    ExternLS::formPermissionReportForFile (const fs::path *path_to_print,
                                           struct stat *fileStat) const{

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


    inline const stringstream *ExternLS::FormTimereportForFile(const fs::path *path_to_print) const{

        struct tm time_struct;
        const std::time_t raw_time = fs::last_write_time(*path_to_print) - timezone;
        gmtime_r(&raw_time, &time_struct);

/*
 *  IT CAN BE USED FOR FUTURE DEBUG OF "@1H issue"
        std::time_t raw_time_now;
        time(&raw_time_now);
        struct tm time_now_struct;
        gmtime_r(&raw_time_now, &time_now_struct);
        std::cout << "NOW IS DST: " << time_now_struct.tm_isdst << endl;
*/

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


    inline void ExternLS::printAllAboutFile(const fs::path *path_to_print, const int depth) const {
        for (int i = 0; i <= depth; ++i)
            printf("    ");
        char filemark = ' ';

        //didn't find the way to make it const
        struct stat fileStat;

        stat(path_to_print->c_str(), &fileStat);

        const struct stat constFileStat = fileStat;

        if (fileStat.st_mode & S_IFDIR) {// is directory
            filemark = '/';
            printf("%c%s \n", filemark, path_to_print->filename().c_str());
            return;
        }

        if (ls_opts->ls_flags_->show_file_type) {
            if (S_ISLNK(fileStat.st_mode)) filemark = '@'; //symbolic link
            if (S_ISSOCK(fileStat.st_mode)) filemark = '='; // socket
            if (S_ISFIFO(fileStat.st_mode)) filemark = '|'; // pipe (named channel)
            if ((fileStat.st_mode & S_IXUSR) || (fileStat.st_mode & S_IXGRP) || (fileStat.st_mode & S_IXOTH))
                filemark = '*'; //executable
        }

        printf("%c%s \n", filemark, path_to_print->filename().c_str());

        if (ls_opts->ls_flags_->detailed_listing_)
            printFileAbout(path_to_print, depth, &fileStat);

    }


    inline void ExternLS::printFileAbout(const fs::path *path_to_print, const int depth, struct stat *file_Stat) const{

        //just to be explicit and be able to delete it later
        const stringstream *time_stream = FormTimereportForFile(path_to_print);
        const stringstream *permissions_stream = formPermissionReportForFile(path_to_print, file_Stat);

        printf(" Perm: %s Ext: [%s] size%lu B  time_written  %s\n",
               permissions_stream->str().c_str(),
               path_to_print->extension().c_str(),
               file_Stat->st_size,
               time_stream->str().c_str());

        delete permissions_stream;
        delete time_stream;
    }


    inline void
    ExternLS::printDirContain(const fs::path *dir, const vector<fs::path> *dir_contain, const int rec_depth) {

        for (int i = 0; i < rec_depth; ++i)
            printf("   ");
        cout << (*dir) << " CONTAINS:\n";

        for (const fs::path subpath: (*dir_contain))
            printAllAboutFile(&subpath, rec_depth + 1);

    }


}