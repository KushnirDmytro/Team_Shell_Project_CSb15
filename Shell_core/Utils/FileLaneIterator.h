//
// Created by d1md1m on 28.03.17.
//

#ifndef LAB_2_SHELL_FILEITERATOR_H
#define LAB_2_SHELL_FILEITERATOR_H

#include <array>
#include <fstream>
#include <boost/filesystem/operations.hpp>

using std::string;

namespace env {

    namespace utils {

        class FileLaneIterator {
        private:
            std::ifstream infile;
            bool isGood;

        public:
            FileLaneIterator(const string filename);

            bool fileIsReady();

            void getNextString(string *buf);

            ~FileLaneIterator();
        };
    }

}

#endif //LAB_2_SHELL_FILEITERATOR_H
