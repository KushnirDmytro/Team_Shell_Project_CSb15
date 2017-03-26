//
// Created by d1md1m on 26.03.17.
//

#ifndef LAB_2_SHELL_PROMPTCONSOLEINTERFACE_H
#define LAB_2_SHELL_PROMPTCONSOLEINTERFACE_H


#include <boost/filesystem/path.hpp>

#include "User.h"
#include "Directory.h"



class ConsoleView{
private:
    Directory *current_directoryPtr;

    string path_buffer;
    const u_int16_t max_path_length = 30; //yes, it is "Magic"

public:

    ConsoleView(Directory *directory_adr){
        this->setCurrent_directoryPtr(directory_adr);
    }

    const u_int16_t getMax_path_length() const {
        return max_path_length;
    }


    Directory *getCurrent_directoryPtr() const {
        return current_directoryPtr;
    }

    void setCurrent_directoryPtr(Directory *current_directoryPtr) {
        this->current_directoryPtr = current_directoryPtr;
    }




    const string &getPath_buffer() const {
        return path_buffer;
    }

    void setPath_buffer(const string &path_buffer) {
        this->path_buffer = path_buffer;
    }


    void refresh_path_buffer(){
        this->setPath_buffer(this->current_directoryPtr->getActual_path().c_str());
    }

    void display_host(){
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


    void display_all(){
        display_host();
        display_path();
    }


    void display_path(){

        string pref = "";
        string postf = "$";
        size_t was_trimmed = 0;


        if (this->current_directoryPtr->isPath_was_changed()){
            this->setPath_buffer(boost::filesystem::current_path().c_str());
        }

        if ( this->current_directoryPtr->contains_home() ) {
            pref.append("~");
        }

        if ((this->getPath_buffer().length() > this->getMax_path_length()) || this->current_directoryPtr->contains_home() ){
            was_trimmed = trim_path_to_size( &(this->path_buffer) , this->getMax_path_length());
        }
        if (was_trimmed){
            pref.append("...");
        }

        printf("%s%s%s", pref.c_str(), this->getPath_buffer().c_str(), postf.c_str());

        this->current_directoryPtr->setPath_was_changed(false);

    }



    size_t trim_path_to_size(string *path, unsigned int size, User *this_user = default_user){
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
                //cout << "path_trimmed ___" << *path << endl;
            }
        }
        return was_trimmed;
    }


};




#endif //LAB_2_SHELL_PROMPTCONSOLEINTERFACE_H
