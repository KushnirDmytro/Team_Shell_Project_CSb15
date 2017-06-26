// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Created by d1md1m on 28.03.17.
//


#include <cstring>
#include <vector>
#include <string>
#include <boost/filesystem.hpp>
#include <sstream>
#include <iostream>
#include "../EmbeddedFunc.h"
#include "Tokenizer.h"
#include "../ReducerToTasks.h"

#include "LineSplitter.h"



namespace sh_core {

    namespace utils {
        LineSplitter::LineSplitter() {}

        LineSplitter::~LineSplitter() {}


// left it here = line delimiters_ " \t\r\n\a"



        // left it here = line delimiters_ " \t\r\n\a"
        // algorithm :
        // /* step 1)divide by tokens (using stringstream)
        // * 2) recognise special tokens and unpack them up to normal ones
        // * (will take special procedures)
        // * 3) markup system for tokens
        // * */
        // TODO finit state machine
        // /*tokens to be decoded/unpacked and specified
        //    * 's' - simple string
        // * ''' - token in brace
        //    * '"' - token in double brace
        // * '`' - token in reverse brace
        //      * 'a' - some argument for whatsoever (least semi-specified string)
        //       * '%' - regexp
        // * 't' - token in string format listing
        // * 'v' - variable
        //  * 'e' - external variable
        //     * 'p' - file_pass
        //   * '/' - dir_pass
        //    * 'i' - inner command
        //    * 'o' - outer command
        // * 'm' - myshell script
        //   * '#' - comment
        //    * '|' - pipeCommand
        //    * '>' - file to output redirrect
        //       * '2' - file to Err redirect
        //    * '<' - file to input redirrect
        //    * '.' - last tokeb
        //    * '=' - setting variable
        //     * */
        //    /** 't' - token in string format listing formating
        //     *  '[text|token]' -- should be ok, token can't finish and text couldn't start from []
        //       * */

        void alternateExec(){

        }

        const vector<string> LineSplitter::mySplitLine( string *input_str, std::vector<arg_desk_pair> ** tasks) const {


            Tokenizer *toker = new Tokenizer();
            ReducerToTasks *r2t = new ReducerToTasks();

            vector<token > *vec = toker->tokenize(input_str);

            *tasks = r2t->reduce(vec);

            // ALTERNATE EXEC BRANCH






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
            delete toker;
            return args;
        }


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


