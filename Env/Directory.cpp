// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Created by d1md1m on 26.03.17.
//

#include <string>
#include <iostream>
#include <boost/filesystem/path.hpp>


#include "Directory.h"

namespace env {

    Directory::Directory() {
        refreshPath();
        setPathWasChanged(true);
    }

    const fs::path &Directory::getActualPath() const {
        return actual_path_;
    }

    void Directory::setActualPath(const fs::path &actual_path) {
        Directory::actual_path_ = actual_path;
    }

    bool Directory::doesPathWasChanged() const {
        return path_was_changed_;
    }

    void Directory::setPathWasChanged(bool path_was_changed) {
        Directory::path_was_changed_ = path_was_changed;
    }

    int Directory::refreshPath() {
        try {
            setActualPath(fs::current_path());
        }
        catch (fs::filesystem_error &e) {
            std::cerr << e.what() << '\n';
        }
        return EXIT_SUCCESS;
    }

    //by default shows info about this object, but can do for any
    bool Directory::suchDirectoryContainsHisHome(const std::string *directory_to_check, const User *this_user) const {

        if (this_user->getHome_dirrectory() != "") {
            if (directory_to_check->find(this_user->getHome_dirrectory().string()) != string::npos) {
                return true;
            }
        }
        return false;

    }
}