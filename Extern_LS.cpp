//
// Created by d1md1m on 31.03.17.
//

#include "Extern_LS.h"
#include <boost/lexical_cast.hpp>


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
                             bool noargs_allowed)
        : Options(name){
    this->opts_map = nullptr;
    this->noargs_allowed = noargs_allowed;
    this->flag_to_write = host_flag;
    }

//checker for received suboptions
    bool LS_simple_opt::are_suboptions_valid(size_t nargs, char **argv) {
        if (noargs_allowed && nargs == 0) {
            (*this->flag_to_write) = true;
            return true;
        }
        else {
            printf("Unexpected argument for %s  /n", this->name.c_str());
            return false;
        }
    }






// option block for sorting
    Ls_sort_opt::Ls_sort_opt( string name, ls_sorts *sorts, map<string, Options*> *opts_map)
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
            *this->sorts = SIZE;
            return true;
        }
        else{

            cout << argv[0] << " RECEIVED AS ARG" << endl;

            if (nargs == 1) {

                string argument = string(argv[0]);

                if (this->sort_opts_map->find(argument) == this->sort_opts_map->end()) {
                    printf("ERROR argument %s is not defined for %s\n", argument.c_str(), this->name.c_str());
                    return false;
                } else {

                    printf("found option %d\n ",this->sort_opts_map->at(argument) );

                    *this->sorts = this->sort_opts_map->at(argument);
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
    delete this->func_opts;
}


Extern_LS::Extern_LS(const string &name,
                     callable_function funct_to_assign,
                     //Options *options,
                     string &help_msg):
        External_func(name,
                      funct_to_assign,
                      //options,
                      help_msg)
{
    this->passes_to_apply = new vector<fs::path>;
    //TODO GEI IT OUT WHEN PROBLEM SOLVED
    this->func_opts =  new LS_opts("LS_opts_object");


};



// 1--getting pathes from args
// 2--verifying options and setting flags
// 3--sorting directories according to flags
// 3.5 -- in case of recursion expanding directories while sorting on preliminar stages
// 3.6 -- sorted vector allready can be printed with additional info
// 3.6 -- else just outputting
int Extern_LS::get_passes_from_args(size_t nargs, char **argv, vector<fs::path> *p_form_args){

    int i = 1; //argv index

    if (nargs == 1){
        p_form_args->push_back(fs::current_path());
        return 1;
    }

    char *arg_buf_ptr = argv[i];

    fs::path p; //path to directory (buffer)args_start_position

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



int Extern_LS::process_passes_from_saved(vector<fs::path> *p_form_args, int rec_depth){

    //TODO sort vector here
    //TODO INCLUDE OPTIONS FOR REVERSING OUTPUT IN FUTURE
    sort( (*p_form_args).begin() , (*p_form_args).end() );
    // ===============INIT===========
    using vect = vector<fs::path> ;


    // ===============INIT END===========


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
                    if ( ( (LS_opts*)func_opts)->LS_flags.recursive ) {

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

    cout << "from bottom_layer MY_LS_INNER" <<endl;


    for (fs::path p : (*this->passes_to_apply)){
        //passes are there from argument line
       cout << "FOUND PATH TO APPLY" << p << endl;
    }

    this->process_passes_from_saved(this->passes_to_apply);
    clear_flags();
    return 1;
}

//Overriding
int Extern_LS::call(size_t nargs, char **argv){

    if (this->search_for_help(nargs, argv)){
        this->output_help(this->help_info);
        return 1;
    }

    get_passes_from_args(nargs, argv, this->passes_to_apply);

    args_start_position += this->passes_to_apply->size();
    if (nargs == 1){
        args_start_position -= 1; // case of default directory adding
    }

    //shifting pointer to actual arguments position start
    argv += args_start_position;
    nargs -= args_start_position;
    return External_func::call(nargs, argv);
}


void inline Extern_LS::clear_flags(){
    ((LS_opts *) this->func_opts)->clear_flags();
}


void inline Extern_LS::print_file_about(fs::path *path_to_print, int depth){
    for (int i=0; i<=depth; ++i)
        printf("    ");
    printf("%s \n", path_to_print->filename().c_str() );


/*
 *
    //TODO add falg-check and additional info listing

    if ( ( (ls_option_flags*)this->func_opts->options_flags)->detailed_listing ){



        std::time_t raw_time = fs::last_write_time(*path_to_print);

        gmtime_r(&raw_time, time_struct_ptr);
        // = boost::lexical_cast<std::string>(time_changed);

        char time_buffer[32];
        //std::strftime(time_buffer, 32, "%a, %d.%m.%Y %H:%M:%S", time_struct_ptr);

        string str(time_buffer);

        printf(" Ext: %s size%lu  time_written  %s\n",
               path_to_print->extension().c_str() ,
               fs::file_size(*path_to_print),
               time_buffer);

    }
*/
}


void inline Extern_LS::print_dir_about(fs::path *path_to_print, int depth){
    for (int i=0; i<=depth; ++i)
        printf("    ");
    printf("/%s  \n", path_to_print->filename().c_str() );
}


void inline Extern_LS::print_dir_contain(fs::path *dir, vector<fs::path> *dir_contain, int rec_depth) {

    for(int i= 0; i < rec_depth; ++i)
        printf("   ");
    cout << (*dir) << " is a directory containing:\n";

    for (fs::path subpath: (*dir_contain) ) {
        if (fs::is_directory(subpath)) {
            print_dir_about(&subpath, rec_depth + 1);
        } else if (fs::is_regular_file(subpath)) {
            print_file_about(&subpath, rec_depth + 1);
        }
    }
}
