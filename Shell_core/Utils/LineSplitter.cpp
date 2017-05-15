// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Created by d1md1m on 28.03.17.
//

#include "LineSplitter.h"
#include <tuple>

namespace sh_core {


    namespace utils {
        LineSplitter::LineSplitter() {}

        LineSplitter::~LineSplitter() {}


// left it here = line delimiters " \t\r\n\a"
        // algorithm :
        /* step 1)divide by tokens (using stringstream)
         * 2) recognise special tokens and unpack them up to normal ones
         * (will take special procedures)
         * 3) markup system for tokens
         * */

        //TODO finit state machine
        /*tokens to be decoded/unpacked and specified
         * 's' - simple string
         * ''' - token in brace
         * '"' - token in double brace
         * '`' - token in reverse brace
         *
         * 'a' - some argument for whatsoever (least semi-specified string)
         * '%' - regexp
         * 't' - token in string format listing
         * 'v' - variable
         * 'e' - external variable
         * 'p' - file_pass
         * '/' - dir_pass
         * 'i' - inner command
         * 'o' - outer command
         * 'm' - myshell script
         * '#' - comment
         * '|' - pipeCommand
         * '>' - file to output redirrect
         * '2' - file to Err redirect
         * '<' - file to input redirrect
         * '.' - last token
         * */

        /** 't' - token in string format listing formating
         *  '[text|token]' -- should be ok, token can't finish and text couldn't start from []
         * */


        const vector<token> LineSplitter::mySplitLine( string *input_str) const {
            input_str->append(" ");
            vector<token> args;

            string NTS = DELIMITERS;//next_tok_stats_from_not = DELIMITERS; //for delimer
            string EOT = "";//end_of_this_token = "";

            size_t pos = 0;
            string tok; // for toke

            size_t start;

            start = input_str->find_first_not_of(DELIMITERS);

            *input_str = input_str->substr(start);

            //to recognise ""''`` we need have by-char proceeding

            do
            {
                if( input_str->at(0)  )


                    tok = input_str->substr(0, pos);
                //  cout << tok << endl;
                args.push_back(token(tok, 's' ));
                *input_str = input_str->substr(start);



            }while ((pos = input_str->find(delim)) != string::npos) //repeat to the end of string
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


