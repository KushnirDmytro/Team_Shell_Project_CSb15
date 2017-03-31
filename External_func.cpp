//
// Created by d1md1m on 29.03.17.
//


#include "External_func.h"
User * default_user;

Directory *current_directory;

Line_splitter* def_line_split;

Interpreter* default_interpreter;


ConsoleView *console;

map <string, Embedded_func*> embedded_lib;



namespace fs = boost::filesystem;



//====================BUILT-IN COMMANDS ============


extern Embedded_func *my_ls_obj;

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





                if (fs::is_regular_file(p))        // is p a regular file?
                    cout << p << " size is " << file_size(p) << '\n';

                else if (is_directory(p))      // is p a directory?
                {
                    cout << p << " is a directory containing:\n";

                    //STAGE OF DIR ENTRIES COLLECTION

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
int my_ls(size_t nargs, char **argv)
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



//show current directory
int my_pwd(size_t nargs, char **args)
{
    current_directory->refresh_path();
    printf("%s", current_directory->getActual_path().c_str());
    return 1;
}

//changes directory
int my_cd(size_t nargs, char **args)
{
    if (args[1] == NULL) { //has to have at least one arg
        fprintf(stderr, "my_Shell: expected argument to \"cd\"\n");
    } else {
        string str(args[1]);
        if (str == home_dir_call){
            if ( boost::filesystem::is_directory( default_user->getHome_dirrectory() ) ){
                boost::filesystem::current_path(default_user->getHome_dirrectory());
            }
        }
        else if (boost::filesystem::is_directory(args[1])){
            boost::filesystem::current_path(args[1]);
        }
        else{
            //TODO filesystem errors
            //boost::filesystem::filesystem_error()
            perror("\n my_Shell failed to change dir \t");
        }

    }
    current_directory->setActual_path(boost::filesystem::current_path());
    current_directory->setPath_was_changed(true);
    return 1;
}

//shows help info
int my_help(size_t nargs, char **args)
{

    printf("Write command and arguments, if needed, then press Enter\n");
    printf("To get detailed information, write <command name> -help or <command name> -h:\n");

    return 1;
}

//just exits, that is it
int my_exit(size_t nargs, char **args)
{
    printf("my_Shell says GoodBye to You and wishes a good day ;O) ");
    return 0;
}

//executes in this shell external ".msh" files
int my_sh(size_t nargs, char **args)
{
    if (nargs > 1){
        string file_path;
        file_path = current_directory->getActual_path().string();
        file_path.append("/");
        file_path.append(args[1]);
        if (boost::filesystem::is_regular_file(args[1]) ){
            FileLaneIterator *iter = new FileLaneIterator(args[1]);
            string st;
            cout << "------------FILE READING IN PROCESS------------------" << endl;
            int i =0;
            int status;
            while(iter->fileIsReady()){
                iter->getNextString(&st);
                printf("String #%d red \n", i++);
                cout << st << endl;
                // st.append(" ");
                status = default_interpreter->proceed_sting(&st);
                if (!status){
                    return 0;
                };

            }
        }
        else if (boost::filesystem::is_regular_file(file_path) ){
            FileLaneIterator *iter = new FileLaneIterator(file_path);
            string st;
            cout << "------------FILE READING IN PROCESS------------------" << endl;
            while(iter->fileIsReady()){
                iter->getNextString(&st);
                cout << st << endl;
                return default_interpreter->proceed_sting(&st);
            }
        }
        else{
            perror(args[1]);
            perror(file_path.c_str());
        }


    }
    return 1;
}





//====================BUILT-IN COMMANDS END============
