// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Created by d1md1m on 26.03.17.
//

#include "Directory.h"



    const boost::filesystem::path &Directory::getActual_path() const {
        return actual_path;
    }

    void Directory::setActual_path(const boost::filesystem::path &actual_path) {
        Directory::actual_path = actual_path;
    }

    bool Directory::isPath_was_changed() const {
        return path_was_changed;
    }

    void Directory::setPath_was_changed(bool path_was_changed) {
        Directory::path_was_changed = path_was_changed;
    }


Directory::Directory(){
        this->refresh_path();
        this->setPath_was_changed(true);
    }

    int Directory::refresh_path(){
        try
        {
            this->setActual_path(boost::filesystem::current_path());
        }
        catch (boost::filesystem::filesystem_error &e)
        {
            std::cerr << e.what() << '\n';
        }
        return EXIT_SUCCESS;
    }

    //by default shows info about this object, but can do for any
    bool Directory::contains_home(User *this_user ){
        if (this_user->getHome_dirrectory() != ""){

            if ( this->getActual_path().string().find(this_user->getHome_dirrectory().string()) != string::npos){
                //    printf ("TEST>>>>>>>>>>>>>CONTAINS HOME<<<<<<<<<<<< \n");
                return true;
            }
            //printf ("TEST>>>>>>>>>>>>>CONTAINS NO!!!  HOME<<<<<<<<<<<< \n");
        }
        return false;
    }
