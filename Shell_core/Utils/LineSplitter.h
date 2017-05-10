//
// Created by d1md1m on 28.03.17.
//

#ifndef LAB_2_SHELL_LINE_SPLITTER_H
#define LAB_2_SHELL_LINE_SPLITTER_H

#include <array>
#include <cstring>
#include <vector>

using std::vector;
using std::string;

namespace sh_core {

    namespace utils {
        class LineSplitter {
        public:
            LineSplitter();

            // left it here = line delimiters " \t\r\n\a"
            const vector<string> mySplitLine(string *input_str) const;

//produced Kovalchuk, Refactored & extracted by Kushnir
            int convertStrVectorToChars(const vector<string> *args, char **cargs) const;

            ~LineSplitter();
        };
    }
}

#endif //LAB_2_SHELL_LINE_SPLITTER_H