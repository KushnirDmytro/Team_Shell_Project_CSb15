//
// Created by d1md1m on 31.03.17.
//

#ifndef LAB_2_SHELL_EXTERN_LS_H
#define LAB_2_SHELL_EXTERN_LS_H



/*

Здається у  вигляді посилання на Гіт проекту.

        Прим. Джерело (авторство) комітів не являєтсья визначним для авторства проекту (вагома його частина була виконана до першого комміту та/або на інших репозиторіях, зокрема зовнішні ф-ї).

https://github.com/Lewbolew/Lab_2_shell

Команда виконавців (внески у різних частинах, відповідно до виконаного обсягу роботи):
Кушнір Дмитро
Ковальчук Богдан
Петришак Богдан


прим.:"
Олег 15:19

Але туди таки закиньте — я Вам зразу гарантую, що я забуду про цю розмову, якщо там не буде :=)

"
*/



#include "External_func.h"


extern command_option ls_opt_help;
extern command_option ls_opt_l;
extern command_option ls_opt_sort;
extern command_option ls_opt_revers;
extern command_option ls_opt_recursive;




enum ls_sorts{NAME, UNSORT, SIZE, TIME_MODIFIED, EXTENTION };


struct ls_option_flags: func_opts_flags{
    bool detailed_listing = false;
    bool recursive = false;
    bool reverse_output = false;
    ls_sorts sort_type;
};

//ls_option_flags ls_flags;

class Extern_LS : public External_func{

private:

    size_t args_start_position = 1;
    vector<fs::path> *passes_to_apply;


public:
    Extern_LS(const string &name,
             callable_function funct_to_assign,
             Function_options *options,
             string &help_msg):
            External_func(name,
                          funct_to_assign,
                          options,
                          help_msg)
    {

        this->passes_to_apply = new vector<fs::path>;
        this->func_opts->opts_flags= new ls_option_flags ;

/*
        ls_func_opts_map_ptr = new map<string, command_option*>;
        (*ls_func_opts_map_ptr)["--help"] = &ls_opt_help;
*/
        //TODO GEI IT OUT WHEN PROBLEM SOLVED

        this->func_opts->func_opts_map = new map<string, command_option*>;//ls_func_opts_map_ptr;
        (*this->func_opts->func_opts_map) ["-l"] = &ls_opt_l;
        (*this->func_opts->func_opts_map) ["--sort"] = &ls_opt_sort;
        (*this->func_opts->func_opts_map) ["-r"] = &ls_opt_revers;
        (*this->func_opts->func_opts_map) ["-R"] = &ls_opt_recursive;


    }
    ~Extern_LS(){
        delete this->passes_to_apply;
    }



    int my_ls_execute(size_t nargs, char **argv){
        return 0;
    }

    int my_ls_collect_dir_entities(vector<fs::path> *vec_of_args){
        return 0;
    }

//TODO
    int my_ls_collect_dir_entities_rec(vector<fs::path> *vec_of_args){
        return 0;
    }

//TODO replace by function-comparator
    int sort_vect(vector<fs::path> *vec_of_args, bool comparator){
        return 0;
    }

//TODO revise output for structure
    int output_args(vector<fs::path> *vec_of_args, bool is_reversed){
        return 0;
    }

//TODO create structure for args and info
// TODO specify falgs
    int collect_additional_info(vector<fs::path> *vec_of_args, vector<string> *infos, char* flags){
        return 0;
    }


    int get_passes_from_args(size_t nargs, char **argv, vector<fs::path> *p_form_args){

        int i = 1; //argv index

        if (nargs == 1){
            p_form_args->push_back(fs::current_path());
            return 1;
        }


        // 1--getting pathes from args
        // 2--verifying options and setting flags
        // 3--sorting directories according to flags
        // 3.5 -- in case of recursion expanding directories while sorting on preliminar stages
        // 3.6 -- sorted vector allready can be printed with additional info
        // 3.4 -- on every stage check for additional info
        // 3.6 -- else just outputting


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

    int process_passes_from_saved(vector<fs::path> *p_form_args){

        //TODO sort vector here

        using vect = vector<fs::path> ;

        vect dir_entries_buf;



        //while first command option marker is not met in line of arguments
        for(fs::path p:*p_form_args){


            try
            {
                if (fs::exists(p))    // does p actually exist?
                {
                    //dir_entries_buf.push_back(p);

                    if (fs::is_regular_file(p)) {

                        // if p is file just save it and print report
                        dir_entries_buf.push_back(p);


                        //not final, just report
                        cout << p << " size is " << file_size(p) << '\n';

                    }
                    else if (is_directory(p))      // is p a directory?
                    {
                        if ( ((ls_option_flags*)this->func_opts->opts_flags)->recursive ) {
                            //if recursive flag -> we can proceed
                            cout << p << " is a directory containing:\n";
                            vect subdir_contain_ptr;

                            copy(fs::directory_iterator(p), fs::directory_iterator(), back_inserter(subdir_contain_ptr));


                            //TODO special sorting
                            sort(subdir_contain_ptr.begin(), subdir_contain_ptr.end());

                            process_passes_from_saved(&subdir_contain_ptr);
                        }
                        else{
                            dir_entries_buf.push_back(p);
                            cout <<  "/" << p.filename().c_str() << "\n";

                            //TODO printing out directory
                           // cout <<  "/" <<p.path().filename().c_str() << "\n";

                          //  std::cout<< p.path().filename().c_str()  << "\n";
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

        //now all entries are in vector
        for (vect::const_iterator it (dir_entries_buf.begin()); it != dir_entries_buf.end(); ++it)
        {
            cout << "   " << (*it).filename().c_str() << '\n';
        }

        return 0; //number of OK pathes
    }



    int recursively_get_dirs() {
        return 1;
    }




//show current directory
    int my_ls_inner(size_t nargs, char **argv)
    {

        //TODO dummy_command over iter



        cout << "from bottom_layer MY_LS_INNER" <<endl;

        for (fs::path p : (*this->passes_to_apply)){
            //passes are there from argument line

            cout << p << endl;
        }

        this->process_passes_from_saved(this->passes_to_apply);







        return 1;
        /*
        fs::path p = fs::current_path();
        if(fs::is_directory(p)) {
            std::cout << p << " is a directory containing:\n";

            //for (fs::path::iterator it = p.begin(); it != p.end(); ++it)
            //    cout << " " << *it << '\n';

            if (nargs>1 && args[1] == "-l" ){
                //detailed case


            }


            for (auto &entry : boost::make_iterator_range(fs::directory_iterator(p), {})) {
                if(fs::is_directory(entry.path())) {
                    //printf("IS DIRECTORY\n");
                    cout <<  "/" <<entry.path().filename().c_str() << "\n";
                }
                else if(fs::is_regular_file(entry.path()))
                    //printf("IS FILENAME\n");
                    std::cout<< entry.path().filename().c_str()  << "\n";
                //std::cout << entry << "\n";
            }

        }
         */
    }


//Overriding
/*

 //NOT FAIL
 ls /home/d1md1m/CLionProjects/Lab_2_shell/cmake-build-debug /home/d1md1m/CLionProjects/Lab_2_shell --sort -l -R

 //FAIL
ls /home/d1md1m/CLionProjects/Lab_2_shell/cmake-build-debug /home/d1md1m/CLionProjects/Lab_2_shell --sort N -l -R


  */

    int call(size_t nargs, char **argv) override {

       // this->nargs = nargs;
       // this->vargs = argv;

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

};


extern Extern_LS *extern_ls_obj;

//just activator-function
//TODO uncomment

/*
 */
#endif //LAB_2_SHELL_EXTERN_LS_H
