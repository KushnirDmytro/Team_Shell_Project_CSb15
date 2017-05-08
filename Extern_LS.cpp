// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Created by d1md1m on 31.03.17.
//

#include "Extern_LS.h"
#include <boost/lexical_cast.hpp>
#include <sys/types.h>
#include <sys/xattr.h>


#include <sys/stat.h>
#include <sys/types.h>

//just activator-function



extern Extern_LS *extern_ls_obj;


LS_opts::LS_opts (string name,
             bool noargs_allowed ) :
        Options(name){

    noargs_allowed = noargs_allowed;

    opts_map = new map<string, Options*>{
            {"-l",
                    new LS_no_subopt_opt( "-l", &LS_flags.detailed_listing) },
            {"-r",
                    new LS_no_subopt_opt( "-r", &LS_flags.reverse_output)},
            {"-R",
                    new LS_no_subopt_opt( "-R", &LS_flags.recursive)},
            {"--sort",
                    new Ls_sort_opt( "--sort", &LS_flags.sort_type)},
            {"-F",
                    new LS_no_subopt_opt( "-F", &LS_flags.file_props)}
    };
};


LS_opts::~LS_opts(){
    for(auto item: *opts_map)
        delete item.second;
    delete opts_map;
   }
   // bool LS_opts::are_suboptions_valid(size_t nargs, char **argv) override;


void LS_opts::clear_flags(){
    LS_flags.recursive = false;
    LS_flags.detailed_listing=false;
    LS_flags.reverse_output=false;
    LS_flags.sort_type=NAME;
};


//prototype for unspecified option
LS_no_subopt_opt::LS_no_subopt_opt(string name,
                             bool* host_flag,
                             bool noargs_allowed_)
        : Options(name){
    opts_map = nullptr;
    noargs_allowed = noargs_allowed_;
    flag_to_write = host_flag;
    }

//checker for received suboptions
    bool LS_no_subopt_opt::are_suboptions_valid(size_t nargs, char **argv) {
        if (noargs_allowed && nargs == 0) {
            (*flag_to_write) = true;
            return true;
        }
        else {
            printf("Unexpected argument for %s  /n", option_name.c_str());
            return false;
        }
    }






// option block for sorting
    Ls_sort_opt::Ls_sort_opt( string name, ls_sorts *sorts)
            : Options( name){
        noargs_allowed = false;
        sort_opts_map = new map<string, ls_sorts>{
                {"U", UNSORT},
                {"S", SIZE},
                {"N", NAME},
                {"X", EXTENTION},
                {"t", TIME_MODIFIED}
        };
        sorts = sorts;

    };

    Ls_sort_opt::~Ls_sort_opt() {
        delete sort_opts_map;
    }





// suboptions validator
    bool Ls_sort_opt::are_suboptions_valid(size_t nargs, char **argv) {
        cout << "ENTERED SORT_OPTIONS" <<endl;
        cout << nargs << " Args number" << endl;

        if (nargs == 0){
            // setting defaul sorting scheme
            *sorts = NAME;
            return true;
        }
        else{

            cout << argv[0] << " RECEIVED AS ARG" << endl;

            if (nargs == 1) {

                string argument = string(argv[0]);

                if (sort_opts_map->find(argument) == sort_opts_map->end()) {
                    printf("ERROR argument %s is not defined for %s\n", argument.c_str(), option_name.c_str());
                    return false;
                } else {

                    printf("found option %d\n ",sort_opts_map->at(argument) );

                    *sorts = sort_opts_map->at(argument);
                    return true;
                }
            }
            else {
                printf("Error on argumnets number (%d) for  option --sort\n", (int)nargs );
                return false;
            }
        }
    }








Extern_LS::~Extern_LS(){
    delete passes_to_apply;
    delete ls_opts;
}


Extern_LS::Extern_LS(const string &name,
                     callable_function funct_to_assign,
                     string &help_msg):
        External_func(name,
                      funct_to_assign,
                      help_msg)
{

    //TODO make this vector static ptr
    passes_to_apply = new vector<fs::path>;
    //TODO GEI IT OUT WHEN PROBLEM SOLVED
    ls_opts =  new LS_opts("LS_opts_object");


};




void inline Extern_LS::clean_up_after_execution() {
    clear_flags();
    passes_to_apply->clear();
    args_start_position_shift = 1;
}

void inline Extern_LS::set_default_directory_as_pass_to_apply() {
    current_directory->refresh_path();
    passes_to_apply->push_back(current_directory->getActual_path());
    cout << "set path to apply as  ==>" << current_directory->getActual_path()<< endl;
    args_start_position_shift-=1; //counting this data modification
}




// 1--getting pathes from args
// 2--verifying options and setting flags
// 3--sorting directories according to flags
// 3.5 -- in case of recursion expanding directories while sorting on preliminar stages
// 3.6 -- sorted vector allready can be printed with additional info
// 3.6 -- else just outputting
int Extern_LS::get_passes_from_args(size_t nargs, char **argv, vector<fs::path> *p_form_args){
    int i = 1; //argv index
    char *arg_buf_ptr = argv[i];

    fs::path p; //path to directory (buffer)args_start_position_shift

    //while first command option marker is not met in line of arguments
    while ( i<nargs && arg_buf_ptr[0] != '-'){

        p = fs::path(arg_buf_ptr);

        try
        {
            if (fs::exists(p))    // does p actually exist?
            {
                p_form_args->push_back(p);
            }
            else
                cout <<  p << endl <<" does not exist\n";
        }

        catch (const fs::filesystem_error& ex)
        {
            cout << ex.what() << '\n';
        }
        ++i;
        arg_buf_ptr = argv[i];
    }

    return 0; //number of OK pathes
}


inline void time_correction(){
    struct tm time_struct;
    const std::time_t raw_time =  timezone;
    gmtime_r(&raw_time, &time_struct);
}


inline void Extern_LS::apply_sorting(vector<fs::path> *vec_to_sort){
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
    switch (this->ls_opts->LS_flags.sort_type){
        case NAME: {
            sort(vec_to_sort->begin(),
                 vec_to_sort->end(),
                 [](fs::path first, fs::path second)->
                               bool {
                     string st1 = first.filename().string();
                     string st2 = second.filename().string();
                     std::transform(st1.begin(), st1.end(), st1.begin(),
                                    [](unsigned char c) { return std::tolower(c); });
                     std::transform(st2.begin(), st2.end(), st2.begin(),
                                    [](unsigned char c) { return std::tolower(c); });
                     return  st1 < st2; }
            );
            break;}

        case TIME_MODIFIED:{
            sort(vec_to_sort->begin(),
                 vec_to_sort->end(),
                 [](fs::path first, fs::path second)->
                                         bool {
                                     return fs::last_write_time(first) < fs::last_write_time(second);}
            );
            break;}
        case UNSORT:{
            sort(vec_to_sort->begin(),
                 vec_to_sort->end(),
                 [](fs::path first, fs::path second)->
                         bool {
                     return false;}
            );
            break;}
        case EXTENTION:{
            sort(vec_to_sort->begin(),
                 vec_to_sort->end(),
                 [](fs::path first, fs::path second)->
                         bool {
                     return first.extension() < second.extension();}
            );
            break;}

        case SIZE: //FUNPART....
        {
            //splitting non-files (directories)
            vector<fs::path>::iterator seek_first_file =  partition(vec_to_sort->begin(),
                                                                    vec_to_sort->end(),
                 [](fs::path elem)-> bool { return ! fs::is_regular_file(elem);}
            );

            //sorting files only
            sort(seek_first_file,
                 vec_to_sort->end(),
                 [](fs::path first, fs::path second)->
                         bool {
                     return fs::file_size(first) < fs::file_size(second);}
            );

            break;}

            //default sorting by name for safety
        default: {sort(vec_to_sort->begin(), vec_to_sort->end(),
                      [](fs::path first, fs::path secod)->
                              bool { return first.filename() < secod.filename();});
            break;}
    }

    if (this->ls_opts->LS_flags.reverse_output){
        reverse(vec_to_sort->begin(), vec_to_sort->end() );
    }


};


int Extern_LS::do_LS_job_with_vector( /*not const, to be sorted inside*/vector<fs::path> *p_from_args,
                                                                        const int rec_depth){
    apply_sorting(p_from_args);
    // ===============INIT===========
    using vect = vector<fs::path> ;
    // ===============INIT END===========

    if (ls_opts->LS_flags.detailed_listing){
        time_correction();
    }

    //iterate list of arguments get
    for(fs::path p:*p_from_args){

        try
        {
            if (fs::exists(p))    // does p actually exist?
            {
                if (is_directory(p)) {

                    if (!ls_opts->LS_flags.recursive && rec_depth != 0) {
                        print_filedata(&p, rec_depth); // if we should not look into directory
                    }
                    else{
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
                }
                else
                    print_filedata(&p, rec_depth);
            }
            else
                cout << p << endl << " does not exist\n"; //yes doublecheck
        }

        catch (fs::filesystem_error &ex)
        {
            cout << ex.what() << '\n';
        }
    }
    return 0;
}




//show current directory
int Extern_LS::my_ls_inner(size_t nargs, char **argv){
    for (fs::path p : (*passes_to_apply)){
        //passes are there from argument line
       cout << "FOUND PATH TO APPLY" << p << endl;
    }

    this->do_LS_job_with_vector(passes_to_apply);

    clean_up_after_execution();

    return 1;
}

//Overriding
int Extern_LS::call(size_t nargs, char **argv) {

    if (search_for_help(nargs, argv)) {
        output_help(help_info);
        return 1;
    }

    get_passes_from_args(nargs, argv, passes_to_apply);

    if (passes_to_apply->size() == 0) {
        set_default_directory_as_pass_to_apply();
    }

    args_start_position_shift += passes_to_apply->size();

    //shifting pointer to actual arguments position start
    argv += args_start_position_shift;
    nargs -= args_start_position_shift;

    if (ls_opts->are_suboptions_valid(nargs, argv)) {
        cout << "problem checking" << endl;


        cout << "Detailed listing flag " << ls_opts->LS_flags.detailed_listing << endl;
        cout << "Recursive output flag " << ls_opts->LS_flags.recursive << endl;
        cout << "Reverted output flag " << ls_opts->LS_flags.reverse_output << endl;
        cout << "Sorting type " << ls_opts->LS_flags.sort_type << endl;

        return External_func::call(nargs, argv);
    }


    else {
        clean_up_after_execution();
        return 1;
    }
};


void inline Extern_LS::clear_flags(){
    this->ls_opts->clear_flags();
};


inline const stringstream * Extern_LS::form_permission_report_for_file(const fs::path *path_to_print, struct stat *fileStat) {

    stat(path_to_print->c_str(), fileStat );

    stringstream* result = new stringstream;

    *result << " Usr:"
            << ( getpwuid(fileStat->st_uid)->pw_name)
            << " Gr:"
            << ( getpwuid(fileStat->st_gid)->pw_name)
            << " "
            << ( (fileStat->st_mode & S_IRUSR) ? "r" : "-")
            << ( (fileStat->st_mode & S_IWUSR) ? "w" : "-")
            << ( (fileStat->st_mode & S_IXUSR) ? "x" : "-")
            << ( (fileStat->st_mode & S_IRGRP) ? "r" : "-")
            << ( (fileStat->st_mode & S_IWGRP) ? "w" : "-")
            << ( (fileStat->st_mode & S_IXGRP) ? "x" : "-")
            << ( (fileStat->st_mode & S_IROTH) ? "r" : "-")
            << ( (fileStat->st_mode & S_IWOTH) ? "w" : "-")
            << ( (fileStat->st_mode & S_IXOTH) ? "x" : "-")
            << " ";

    return result;
}

inline const stringstream * Extern_LS::form_timereport_for_file(const fs::path *path_to_print){

    struct tm time_struct;

    const std::time_t raw_time = fs::last_write_time(*path_to_print) - timezone;

    /*
    cout << "TIME ZONE "<<timezone << endl;

    cout << "RAW TIME "<<raw_time << endl;

    cout << "TIME ZONE "  <<__timezone<< endl;
*/
    gmtime_r(&raw_time, &time_struct);


    stringstream *ss = new stringstream;

    *ss <<"" << 1900 + time_struct.tm_year
       << "-" <<1 + time_struct.tm_mon
       << "-" << time_struct.tm_mday
       <<"  " << time_struct.tm_hour + time_struct.tm_isdst + 1
       << ":" << time_struct.tm_min
       << ":" << time_struct.tm_sec
       << time_struct.tm_zone
       << " (dst " << time_struct.tm_isdst << ')'
       << " correction " << 1 << "H"
       << endl;

    return ss;
}


inline void Extern_LS::print_filedata(const fs::path *path_to_print, const int depth){
    for (int i=0; i<=depth; ++i)
        printf("    ");
    char filemark = ' ';

    struct stat fileStat;

    stat(path_to_print->c_str(), &fileStat);

    if (fileStat.st_mode & S_IFDIR){// is directory
        filemark = '\\';
        printf("%c%s \n", filemark,  path_to_print->filename().c_str() );
        return;
    }

    if (ls_opts->LS_flags.file_props){
        if (S_ISLNK(fileStat.st_mode)) filemark = '@'; //symbolic link
        if (S_ISSOCK(fileStat.st_mode)) filemark = '='; // socket
        if (S_ISFIFO(fileStat.st_mode)) filemark = '|'; // pipe (named channel)
        if ( (fileStat.st_mode & S_IXUSR) || (fileStat.st_mode & S_IXGRP) || (fileStat.st_mode & S_IXOTH) ) filemark = '*'; //executable
    }

    printf("%c%s \n", filemark,  path_to_print->filename().c_str() );

    if (ls_opts->LS_flags.detailed_listing)
        print_file_about(path_to_print, depth, &fileStat);

}


inline void Extern_LS::print_file_about(const fs::path *path_to_print,const int depth, struct stat *fileStat){

        const stringstream *time_stream = form_timereport_for_file(path_to_print);
        const stringstream *permissions_stream = form_permission_report_for_file(path_to_print, fileStat);

        printf(" Perm: %s Ext: [%s] size%lu B  time_written  %s\n",
               permissions_stream->str().c_str(),
               path_to_print->extension().c_str() ,
               fileStat->st_size,
        time_stream->str().c_str() );

        delete permissions_stream;
        delete time_stream;
}




inline void Extern_LS::print_dir_contain(const fs::path *dir,const vector<fs::path> *dir_contain,const int rec_depth) {

    for(int i= 0; i < rec_depth; ++i)
        printf("   ");
    cout << (*dir) << " CONTAINS:\n";

    for (const fs::path subpath: (*dir_contain) )
        print_filedata(&subpath, rec_depth + 1);

}
