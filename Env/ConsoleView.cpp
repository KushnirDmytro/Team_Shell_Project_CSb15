// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Created by d1md1m on 26.03.17.
//

#include "ConsoleView.h"

namespace shell {

    ConsoleView::ConsoleView(Directory *directory_adr, User *user) {
        setCurrentDirectory(directory_adr);
        setCurrentUser(user);
    }


    void ConsoleView::setCurrentUser(User *user) {
        current_user_ = user;
    }


    const u_int16_t ConsoleView::getMaxPathLength() const {
        return max_path_length_;
    }


    User *ConsoleView::getCurrentUser() const {
        return current_user_;
    }

    Directory *ConsoleView::getCurrentDirectory() const {
        return current_directory_;
    }

    void ConsoleView::setCurrentDirectory(Directory *current_directory) {
        current_directory_ = current_directory;
    }

    const std::string &ConsoleView::getPathBuffer() const {
        return path_buffer_;
    }

    void ConsoleView::setPathBuffer(const std::string &path_buffer) {
        path_buffer_ = path_buffer;
    }

    void ConsoleView::refreshPathBuffer() {
        setPathBuffer(current_directory_->getActualPath().c_str());
    }

    inline void ConsoleView::displayHost() const {
        printf("\n");
        if (current_user_->getName().length() > 0 || current_user_->getHostname().length() > 0) {
            //TODO refactor to stringbuilder
            if (current_user_->getName().length() > 0) {
                printf("%s", current_user_->getName().c_str());
            }

            if (current_user_->getHostname().length() > 0) {
                printf("@%s", current_user_->getHostname().c_str());
            }

            printf(": ");
        }
    };


    void ConsoleView::displayPromptMsg(){
        displayHost();
        displayPath();
    }


    void ConsoleView::displayPath(){

        string prefix = "";
        string postf = "$";
        string temp_console_disp_buf;
        size_t n_chars_trimmed = 0;

        if (current_directory_->doesPathWasChanged()) {
            temp_console_disp_buf = fs::current_path().string();
        }
        else {
            printf("%s", getPathBuffer().c_str());
            return;
        }

        if (current_directory_->containsHisHome(current_user_)) {
            prefix.append("~");
        }

        if ((temp_console_disp_buf.length() > getMaxPathLength()) || current_directory_->containsHisHome(current_user_)) {
            n_chars_trimmed = trimPathToSize( &temp_console_disp_buf, getMaxPathLength());
        }

        if (n_chars_trimmed) {
            prefix.append("...");
        }

        setPathBuffer(prefix.append(temp_console_disp_buf).append(postf));

        printf("%s", getPathBuffer().c_str());
        current_directory_->setPathWasChanged(false);
    }


    size_t ConsoleView::trimPathToSize() const{
        size_t position;
        size_t was_trimmed = 0;
        string path_buf = current_directory_->getActualPath().string();

        if (current_directory_->containsHisHome(current_user_)) {
            path_buf = path_buf.substr(current_user_->getHome_dirrectory().string().length());
        }
        string path_delimiter = "/";
        while (path_buf.length() > max_path_length_) {
            position = path_buf.find(path_delimiter);
            if (position != string::npos) {
                path_buf = path_buf.substr(position + 1);
                was_trimmed += position + 1;
            }
        }
        return was_trimmed;
    }

}

