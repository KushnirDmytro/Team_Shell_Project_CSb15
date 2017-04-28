// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Created by d1md1m on 26.03.17.
//

#include "ConsoleView.h"




ConsoleView::ConsoleView(Directory *directory_adr){
        this->setCurrent_directoryPtr(directory_adr);
    }

    const u_int16_t ConsoleView::getMax_path_length() const {
        return max_path_length;
    }


    Directory *ConsoleView::getCurrent_directoryPtr() const {
        return current_directoryPtr;
    }

    void ConsoleView::setCurrent_directoryPtr(Directory *current_directoryPtr) {
        this->current_directoryPtr = current_directoryPtr;
    }




    const string &ConsoleView::getPath_buffer() const {
        return path_buffer;
    }

    void ConsoleView::setPath_buffer(const string &path_buffer) {
        this->path_buffer = path_buffer;
    }


    void ConsoleView::refresh_path_buffer(){
        this->setPath_buffer(this->current_directoryPtr->getActual_path().c_str());
    }

    void ConsoleView::display_host(){
        printf("\n");
        if (default_user->getName().length() > 0 || default_user->getHostname().length() > 0){
            if (default_user->getName().length() > 0 ){
                printf("%s",default_user->getName().c_str());
            }
            if (default_user->getHostname().length() > 0){
                printf("@%s", default_user->getHostname().c_str());
            }
            printf(": ");
        }
    };


    void ConsoleView::display_all(){
        display_host();
        display_path();
    }


    void ConsoleView::display_path(){

        string pref = "";
        string postf = "$";
        string temp_buf = "";
        size_t was_trimmed = 0;
        if (this->current_directoryPtr->isPath_was_changed()){
            temp_buf = boost::filesystem::current_path().string();
        }
        else{
            printf("%s", this->getPath_buffer().c_str());
            return;
        }

       // else{
        //    return;
       // }

        if ( this->current_directoryPtr->contains_home() ) {
            pref.append("~");
        }

        if ((temp_buf.length() > this->getMax_path_length()) || this->current_directoryPtr->contains_home() ){
            was_trimmed = trim_path_to_size( &(temp_buf) , this->getMax_path_length());
        }
        if (was_trimmed){
            pref.append("...");
        }
        this->setPath_buffer(pref.append(temp_buf).append(postf));

        printf("%s", this->getPath_buffer().c_str());
        this->current_directoryPtr->setPath_was_changed(false);

    }



    size_t ConsoleView::trim_path_to_size(string *path, unsigned int size, User *this_user){
        size_t position;
        size_t was_trimmed = 0;
        boost::filesystem::path path_buf = *path;

        if (this->current_directoryPtr->contains_home(this_user)){
            *path = path->substr(this_user->getHome_dirrectory().string().length());
            //cout << "TEST>>>>>>>>>>?????????????" <<endl;
            //cout << *path << endl;
        }
        string path_delimiter = "/";
        while (path->length() > size){
            position = path->find(path_delimiter);
            if (position != string::npos){
                *path = path->substr(position+1);
                was_trimmed += position+1;
               // cout << "path_trimmed ___" << *path << "Trimmed "<< was_trimmed << endl;
            }
        }
        return was_trimmed;
    }



