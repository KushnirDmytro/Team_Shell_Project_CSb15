// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Created by d1md1m on 28.03.17.
//

#include "LineSplitter.h"


//#include <array>
//#include <cstring>
//#include <vector>
//#include <boost/filesystem.hpp>
//#include "../EmbeddedFunc.h"


namespace sh_core {

    namespace utils {
        LineSplitter::LineSplitter() {}

        LineSplitter::~LineSplitter() {}


// left it here = line delimiters " \t\r\n\a"
        const vector<string> LineSplitter::mySplitLine( string *input_str) const {
            input_str->append(" ");
            vector<string> args;

            string delim = " "; //for delimer
            size_t pos = 0;
            string tok; // for toke

            unsigned long start;

            start = input_str->find_first_not_of(" ");

            *input_str = input_str->substr(start);

            while ((pos = input_str->find(delim)) != string::npos) {
                tok = input_str->substr(0, pos);
                //  cout << tok << endl;
                args.push_back(tok);
                input_str->erase(0, pos + delim.length());
            }
            return args;
        }


//produced Kovalchuk, Refactored & extracted by Kushnir
        int LineSplitter::convertStrVectorToChars(const vector<string> *args, char **cargs) const {
            size_t i;

            for (i = 0; i < args->size(); ++i) {
                cargs[i] = new char[(*args)[i].size() + 1];
                strcpy(cargs[i], (*args)[i].c_str());
            }
            cargs[i] = NULL;
            return 0;
        }





    }

}


