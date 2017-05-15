//
// Created by d1md1m on 28.03.17.
//

#ifndef LAB_2_SHELL_LINE_SPLITTER_H
#define LAB_2_SHELL_LINE_SPLITTER_H

#include <array>
#include <cstring>
#include <vector>
#include <boost/filesystem.hpp>
#include "../EmbeddedFunc.h"

using std::vector;
using std::string;
using std::vector;
namespace fs = boost::filesystem;

namespace sh_core {
//using DELIMITERS = " \t\r\n\a";
    const string DELIMITERS = " \t\r\n\a";

    using token = std::tuple<string, char>;


    namespace utils {
        class LineSplitter {
        public:
            LineSplitter();

            // left it here = line delimiters " \t\r\n\a"
            const vector<token> mySplitLine(string *input_str) const;

//produced Kovalchuk, Refactored & extracted by Kushnir
            int convertStrVectorToChars(const vector<string> *args, char **cargs) const;

            //TODO solve problem of spaced filenames
            ~LineSplitter();
        };
    }
}

#endif //LAB_2_SHELL_LINE_SPLITTER_H