//
// Created by d1md1m on 28.03.17.
//

#ifndef LAB_2_SHELL_LINE_SPLITTER_H
#define LAB_2_SHELL_LINE_SPLITTER_H

#include <array>
#include <vector>
#include <cstring>

using namespace std;


class Line_splitter{
public:
    Line_splitter();

    // left it here = line delimiters " \t\r\n\a"
    vector <string> my_split_line(string input_str);

//produced Kovalchuk, Refactored & extracted by Kushnir
    int str_vector_to_chars(vector<string> *args, char** cargs);

    ~Line_splitter();
};

#endif //LAB_2_SHELL_LINE_SPLITTER_H
