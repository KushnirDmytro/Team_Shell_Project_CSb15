//
// Created by d1md1m on 31.03.17.
//

#ifndef LAB_2_SHELL_EXTERN_LS_H
#define LAB_2_SHELL_EXTERN_LS_H


#include "External_func.h"


/*
command_option ls_opt_help{

};
command_option ls_opt_l{

};
command_option ls_opt_sort{

};
command_option ls_opt_revers{

};
command_option ls_opt_recursive{

};

options_struct ls_opts = {
        ls_opts.func_opts_map = {
                {"--help", ls_opt_help},
                {"-h", ls_opt_help},
                {"-l", ls_opt_l},
                {"--sort", ls_opt_sort},
                {"-r", ls_opt_revers},
                {"-R", ls_opt_recursive}
                // ,{"", ls_opt_},
        }
};

*/


class Extern_LS : public External_func{

private:

public:
    Extern_LS(const string &name,
             callable_function funct_to_assign,
             Function_options *options,
             string &help_msg):
            External_func(name,
                          funct_to_assign,
                          options,
                          help_msg)
    {}



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


        char *arg_buf_ptr = argv[0];


        fs::path p; //path to directory (buffer)

        while ( i<nargs && arg_buf_ptr[0] != '-'){
            arg_buf_ptr = argv[i];

            p = fs::path(arg_buf_ptr);

            try
            {
                if (fs::exists(p))    // does p actually exist?
                {

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

                    if (fs::is_regular_file(p))        // is p a regular file?
                        cout << p << " size is " << file_size(p) << '\n';

                    else if (is_directory(p))      // is p a directory?
                    {
                        cout << p << " is a directory containing:\n";

                        //STAGE OF DIR ENTRIES COLLECTIONc

                        typedef vector<fs::path> vec;             // store paths,
                        vec v;                                // so we can sort them later

                        copy(fs::directory_iterator(p), fs::directory_iterator(), back_inserter(v));




                        //TODO COMPARING RULES

                        sort(v.begin(), v.end());             // sort, since directory iteration
                        // is not ordered on some file systems


                        // OUTPUT FINAL RESULT SECTION
                        // TODO CHECK IF REVERSE NEEDED
                        for (vec::const_iterator it (v.begin()); it != v.end(); ++it)
                        {
                            cout << "   " << *it << '\n';
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





            ++i;
        }





        return 0; //number of OK pathes
    }



//show current directory
    int my_ls_inner(size_t nargs, char **argv)
    {
        vector<fs::path> p_form_args;
        get_passes_from_args(nargs, argv, &p_form_args);

        for (fs::path p : p_form_args){
            cout << p << endl;
        }


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



};


extern Extern_LS *extern_ls_obj;

//just activator-function
//TODO uncomment

/*
 */
#endif //LAB_2_SHELL_EXTERN_LS_H
