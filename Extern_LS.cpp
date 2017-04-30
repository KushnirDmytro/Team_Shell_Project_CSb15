// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Created by d1md1m on 31.03.17.
//

#include "Extern_LS.h"
#include <boost/lexical_cast.hpp>
#include <queue>


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

/*
bool LS_opts::suboptionS_arguments_validation(Options* opt_to_check, vector<string>* arg_buf){
    char* temp_buf[(*arg_buf).size()];
    str_vec_to_char_arr((*arg_buf), temp_buf);
    if (! (opt_to_check->are_suboptions_valid((*arg_buf).size(), temp_buf) ) ){
        printf("ARGUMENT CHECK FAILED AT OPTION %s\n", opt_to_check->option_name.c_str());
        return false;
    }
    (*arg_buf).clear();
    return true;
};
*/




/*
bool LS_opts::are_suboptions_valid(size_t nargs, char **argv) {

    if (nargs == 0){
        return argumentless_option_check(nargs, argv);
    }

    vector<string> args_vec;
    args_vec.insert(args_vec.end(), argv, argv + nargs);

    queue <string> ls_argumens_queue;



    //==========CHECK of vector insertion performed =========
    for (int i = 0; i < nargs; ++i){
        printf("function get  ===>%s<===  \n", argv[i]);
    }
    for(auto i: args_vec)
        cout<< "vector inserted word ===>" << i <<"<===" << endl;
    //==========CHECK =========


    Options *prev_founded_option = nullptr;
    Options *new_founded_option = nullptr;


    vector<string> arg_buf;

    string iter_arg_name;

    for(int i = 0; i<args_vec.size(); ++i ){

        iter_arg_name = args_vec[i];

        if (new_founded_option != nullptr) { //case when it is not first iteration
            prev_founded_option = new_founded_option;
        }

        //new_founded_option =
        if (this->opts_map->find(iter_arg_name)
            ==
            this->opts_map->end())
            new_founded_option = nullptr;
        else
            new_founded_option = this->opts_map->at(iter_arg_name);
        // get_option(iter_arg_name);


        if (new_founded_option == nullptr) {
            if (prev_founded_option == nullptr) {
                printf("EROR, FIRST ARGUMENT ===>%s<=== IS NOT OPTION\n", iter_arg_name.c_str());
                return false;
            } else {
                arg_buf.push_back(iter_arg_name);

                cout << "arg added to arg buffer vector "<< iter_arg_name << endl;
                cout << "new size of arg vector " << arg_buf.size() << endl;

            }
        }


        else {
            if (prev_founded_option == nullptr) {
                prev_founded_option = new_founded_option;
            } else {

                if (!suboptionS_arguments_validation(prev_founded_option, &arg_buf))
                    return false;
            }


        }
        //LAST option check
        if ( i == nargs - 1){

            if (!suboptionS_arguments_validation(prev_founded_option, &arg_buf))
                return false;
        }

    }


    cout << "CROSS_VALIDATION" << endl;
    if (this->are_options_cross_valid()){
        printf("ARGUMENT CHECK DONE \n");
        return true;
    }

    else    return false;
};
*/

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
            *this->sorts = SIZE;
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

                    cout << "TEEEEST" << endl;
                    auto p = dynamic_cast<Ls_sort_opt*>(extern_ls_obj->ls_opts->opts_map->at("--sort"));
                    if(p!=0)
                        cout << p->option_name <<endl;


                    //* (( (Ls_sort_opt*)extern_ls_obj->ls_opts->opts_map->at("--sort") )->sorts)  =
                     //       ( (Ls_sort_opt*)extern_ls_obj->ls_opts->opts_map->at("--sort") )->sort_opts_map->at(argument) ;

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
    this->passes_to_apply->clear();
    this->args_start_position_shift = 1;
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

    for (fs::path p : (*this->passes_to_apply)){
        //passes are there from argument line
       cout << "FOUND PATH TO APPLY" << p << endl;
    }

    this->process_passes_from_saved(this->passes_to_apply);

    clean_up_after_execution();

    return 1;
}

//Overriding
int Extern_LS::call(size_t nargs, char **argv) {

    if (this->search_for_help(nargs, argv)) {
        this->output_help(this->help_info);
        return 1;
    }

    get_passes_from_args(nargs, argv, this->passes_to_apply);

    if (passes_to_apply->size() == 0) {
        set_default_directory_as_pass_to_apply();
    }

    args_start_position_shift += this->passes_to_apply->size();

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
        this->clean_up_after_execution();
        return 1;
    }
};


void inline Extern_LS::clear_flags(){
    this->ls_opts->clear_flags();
};


void Extern_LS::print_file_about(fs::path *path_to_print, int depth){
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


void Extern_LS::print_dir_about(fs::path *path_to_print, int depth){
    for (int i=0; i<=depth; ++i)
        printf("    ");
    printf("/%s  \n", path_to_print->filename().c_str() );
}


void Extern_LS::print_dir_contain(fs::path *dir, vector<fs::path> *dir_contain, int rec_depth) {

    for(int i= 0; i < rec_depth; ++i)
        printf("   ");
    cout << (*dir) << " CONTAINS:\n";

    for (fs::path subpath: (*dir_contain) ) {
        if (fs::is_directory(subpath)) {
            print_dir_about(&subpath, rec_depth + 1);
        } else if (fs::is_regular_file(subpath)) {
            print_file_about(&subpath, rec_depth + 1);
        }
    }
}
