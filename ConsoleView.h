//
// Created by d1md1m on 26.03.17.
//

#ifndef LAB_2_SHELL_PROMPTCONSOLEINTERFACE_H
#define LAB_2_SHELL_PROMPTCONSOLEINTERFACE_H


#include <boost/filesystem/path.hpp>

#include "User.h"
#include "Directory.h"



//User * default_user;


//#include "default_definitions.h"


class ConsoleView{
private:
    Directory *current_directoryPtr;

    string path_buffer;
    const u_int16_t max_path_length = 30; //yes, it is "Magic"

public:

    ConsoleView(Directory *directory_adr);

    const u_int16_t getMax_path_length() const ;
    Directory *getCurrent_directoryPtr() const ;

    void setCurrent_directoryPtr(Directory *current_directoryPtr) ;




    const string &getPath_buffer() const ;

    void setPath_buffer(const string &path_buffer) ;

    void refresh_path_buffer();

    void display_host();
    void display_all();


    void display_path();


    size_t trim_path_to_size(string *path, unsigned int size, User *this_user = default_user);


};




#endif //LAB_2_SHELL_PROMPTCONSOLEINTERFACE_H
