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
extern int my_ls(size_t nargs, char **argv)
{
    return extern_ls_obj->my_ls_inner(nargs, argv);
    //my_ls_inner(nargs, argv);
}


extern Extern_LS *extern_ls_obj;


LS_opts::LS_opts (string name,
             bool noargs_allowed ) :
        Options(name){

    this->noargs_allowed = noargs_allowed;

    this->opts_map = new map<string, Options*>{
            {"-l",
                    new LS_simple_opt( "-l", &this->LS_flags.detailed_listing) },
            {"-r",
                    new LS_simple_opt( "-r", &this->LS_flags.reverse_output)},
            {"-R",
                    new LS_simple_opt( "-R", &this->LS_flags.recursive)},
            {"--sort",
                    new Ls_sort_opt( "--sort", &this->LS_flags.sort_type)}
    };
};


LS_opts::~LS_opts(){
    delete this->opts_map->at("-l");
    delete this->opts_map->at("-r");
    delete this->opts_map->at("-R");
    delete this->opts_map->at("--sort");
}
   // bool LS_opts::are_suboptions_valid(size_t nargs, char **argv) override;


void LS_opts::clear_flags(){
    this->LS_flags.recursive = false;
    this->LS_flags.detailed_listing=false;
    this->LS_flags.reverse_output=false;
    this->LS_flags.sort_type=NAME;
};


//prototype for unspecified option
LS_simple_opt::LS_simple_opt(string name,
                             bool* host_flag,
                             bool noargs_allowed_)
        : Options(name){
    opts_map = nullptr;
    noargs_allowed = noargs_allowed_;
    flag_to_write = host_flag;
    }

//checker for received suboptions
    bool LS_simple_opt::are_suboptions_valid(size_t nargs, char **argv) {
        if (noargs_allowed && nargs == 0) {
            (*this->flag_to_write) = true;
            return true;
        }
        else {
            printf("Unexpected argument for %s  /n", this->option_name.c_str());
            return false;
        }
    }






// option block for sorting
    Ls_sort_opt::Ls_sort_opt( string name, ls_sorts *sorts)
            : Options( name){
        this->noargs_allowed = false;
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
    bool Ls_sort_opt::are_suboptions_valid(size_t nargs, char **argv) {
        cout << "ENTERED SORT_OPTIONS" <<endl;
        cout << nargs << " Args number" << endl;

        if (nargs == 0){
            // setting defaul sorting scheme
            *this->sorts = NAME;
            return true;
        }
        else{

            cout << argv[0] << " RECEIVED AS ARG" << endl;

            if (nargs == 1) {

                string argument = string(argv[0]);

                if (this->sort_opts_map->find(argument) == this->sort_opts_map->end()) {
                    printf("ERROR argument %s is not defined for %s\n", argument.c_str(), this->option_name.c_str());
                    return false;
                } else {

                    printf("found option %d\n ",this->sort_opts_map->at(argument) );

                    /*
                    cout << "TEEEEST" << endl;
                    auto p = dynamic_cast<Ls_sort_opt*>(extern_ls_obj->ls_opts->opts_map->at("--sort"));
                    if(p!=0)
                        cout << p->option_name <<endl;
                    */


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
    delete this->passes_to_apply;
    delete this->ls_opts;
}


Extern_LS::Extern_LS(const string &name,
                     callable_function funct_to_assign,
                     string &help_msg):
        External_func(name,
                      funct_to_assign,
                      help_msg)
{
    //cout << "S: " << option_name << endl;

    //TODO make this vector static ptr
    this->passes_to_apply = new vector<fs::path>;
    //TODO GEI IT OUT WHEN PROBLEM SOLVED
    this->ls_opts =  new LS_opts("LS_opts_object");


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

                if (fs::is_directory(p) || fs::is_regular_file(p)) {
                    p_form_args->push_back(p);
                }
                else
                    cout << p << " exists, but is neither a regular file nor a directory\n";
            }
            else
                cout << p << " does not exist\n";
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

int Extern_LS::process_passes_from_saved( /*not const, to be sotred inside*/vector<fs::path> *p_form_args,const int rec_depth){

    //TODO sort vector here
    //TODO INCLUDE OPTIONS FOR REVERSING OUTPUT IN FUTURE
    sort( (*p_form_args).begin() , (*p_form_args).end() );
    // ===============INIT===========
    using vect = vector<fs::path> ;
    // ===============INIT END===========


    if (ls_opts->LS_flags.detailed_listing){
        time_correction();
    }


    //iterate list of arguments get
    for(fs::path p:*p_form_args){

        try
        {
            if (fs::exists(p))    // does p actually exist?
            {


                if (fs::is_regular_file(p)) {
                    print_file_about(&p, rec_depth);
                }


                    //need to fill buffer before processing
                else if (is_directory(p))
                {
                    vect subdir_contain;

                    copy(fs::directory_iterator(p), fs::directory_iterator(), back_inserter(subdir_contain));

                    //RECURSIVE EXEC BRANCH
                    if ( ls_opts->LS_flags.recursive ) {

                        for(int i= 0; i < rec_depth; ++i)
                            printf("   ");
                        cout << p << " is a directory containing:\n";
                        process_passes_from_saved(&subdir_contain, rec_depth + 1);
                    }

                        //linear exec branch
                    else
                    {
                        //TODO special sorting
                        sort(subdir_contain.begin(), subdir_contain.end());
                        print_dir_contain(&p, &subdir_contain, rec_depth);
                    }

                }

                else
                    cout << p << " exists, but is neither a regular file nor a directory\n";
            }
            else
                cout << p << " does not exist\n";
        }

        catch (const fs::filesystem_error& ex)
        {
            cout << ex.what() << '\n';
        }

    }

    return 0; //number of OK pathes
}




//show current directory
int Extern_LS::my_ls_inner(size_t nargs, char **argv){

    //cout << "from bottom_layer MY_LS_INNER" <<endl;

    for (fs::path p : (*passes_to_apply)){
        //passes are there from argument line
       cout << "FOUND PATH TO APPLY" << p << endl;
    }

    this->process_passes_from_saved(passes_to_apply);

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






inline void Extern_LS::print_file_about(const fs::path *path_to_print,const int depth){
    for (int i=0; i<=depth; ++i)
       printf("    ");
    printf("%s \n", path_to_print->filename().c_str() );

    //TODO add falg-check and additional info listing

    if ( ls_opts->LS_flags.detailed_listing){

        struct stat fileStat;

        const stringstream*  time_stream = form_timereport_for_file(path_to_print);
        const stringstream*  permissions_stream = form_permission_report_for_file(path_to_print, &fileStat);

        printf(" Perm: %s Ext: [%s] size%lu B  time_written  %s\n",
               permissions_stream->str().c_str(),
               path_to_print->extension().c_str() ,
               fileStat.st_size,
        time_stream->str().c_str() );
        delete permissions_stream;
        delete time_stream;
    }


}


inline void Extern_LS::print_dir_about(const fs::path *path_to_print,const int depth){
    for (int i=0; i<=depth; ++i)
        printf("    ");
    printf("/%s  \n", path_to_print->filename().c_str() );
}


inline void Extern_LS::print_dir_contain(const fs::path *dir,const vector<fs::path> *dir_contain,const int rec_depth) {

    for(int i= 0; i < rec_depth; ++i)
        printf("   ");
    cout << (*dir) << " CONTAINS:\n";

    for (const fs::path subpath: (*dir_contain) ) {
        if (fs::is_directory(subpath)) {
            print_dir_about(&subpath, rec_depth + 1);
        } else if (fs::is_regular_file(subpath)) {
            print_file_about(&subpath, rec_depth + 1);
        }
    }
}
