//
// Created by d1md1m on 26.03.17.
//

#ifndef LAB_2_SHELL_PROMPTCONSOLEINTERFACE_H
#define LAB_2_SHELL_PROMPTCONSOLEINTERFACE_H


#include <boost/filesystem/path.hpp>
#include <string>

#include "User.h"
#include "Directory.h"


class ConsoleView{
private:
    Directory *current_directory_;
    User* current_user_;
    std::string path_buffer_ = "";

    const u_int16_t max_path_length_ = 30; //yes, it is "Magic"

    size_t trimPathToSize() const;

public:
    // TODO refactor in private

    ConsoleView(Directory *directory_adr, User* current_user);

    const u_int16_t getMaxPathLength() const ;
    void setCurrentDirectory(Directory *dir);
    Directory *getCurrentDirectory() const ;
    void setCurrentUser(User *user);
    User* getCurrentUser() const;

    const std::string &getPathBuffer() const ;
    void setPathBuffer(const std::string &path_buffer) ;

    void refreshPathBuffer();

    void displayHost() const;
    void displayPromptMsg();
    void displayPath();
};


#endif //LAB_2_SHELL_PROMPTCONSOLEINTERFACE_H
