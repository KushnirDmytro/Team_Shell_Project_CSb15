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


#include "LineSplitter.h"



namespace sh_core {

    namespace utils {
        LineSplitter::LineSplitter() {}

        LineSplitter::~LineSplitter() {}


// left it here = line delimiters " \t\r\n\a"



        // left it here = line delimiters " \t\r\n\a"
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


        struct machine_state{
            bool isComment = false;
            bool isDoubleBrace = false;
            bool isBrace = false;
            bool isReverceBrace = false;
            bool isRegexp = false;

            void clean_all(){
                isComment = false;
                isDoubleBrace = false;
                isBrace = false;
                isReverceBrace = false;
                isRegexp = false;
            }

            bool isInAnyBrace(){
                return  (isDoubleBrace || isBrace || isReverceBrace);
            }

            bool isIndependant(){
                return !(isComment||isDoubleBrace || isBrace || isReverceBrace);
            }

            char getToken(){
                return 's';
            }
        };

        inline bool isSpace(char ch) {
            return (ch == ' ' || ch == '\t'); // Yes TABS are allso Spaces, HOLY WAR is over
        }

        const vector<string> LineSplitter::mySplitLine( string *input_str) const {

            std::stringstream ss(*input_str);

            std::vector <std::pair<std::string, char>> *tokens_vector = new std::vector <std::pair<std::string, char>>;

            const std::string special_symbols = "'\"`%/#|><._*&=";
            const std::string open_pair_symbols = "'\"`#";
            const std::string delimiters = " \t\r\n\a";

            machine_state mst ;

            std::stringstream building_block;// = new std::stringstream;


            size_t char_index = 0;
            size_t end_of_string = input_str->length();
            char ch;

            if (ss.good()) ch = (char)ss.get();
            else std::cout<< "EMPTY STRING "<<std::endl;

            while (ss.good()){


                size_t founded_char_position = open_pair_symbols.find_first_of(ch);
                if (founded_char_position != std::string::npos) {

                    if (building_block.rdbuf()->in_avail() != 0) {
                        tokens_vector->push_back(std::pair<string, char>(building_block.str(), 'S'));
                        building_block.str("");
                        mst.isDoubleBrace = false;
                    }

                    switch (ch) {

                        case '"':{
                            mst.isDoubleBrace = true;

                            if (ss.good()) {
                                ch = static_cast<char> (ss.get());

                                while (ch != '"' && ss.good()) {
                                    building_block << ch;
                                    ch = static_cast<char> (ss.get());
                                };

                            }
                            if (!ss.good()) {
                                std::cout << "ERROR, can't find closing pair for opening char!" << std::endl;
                               // return NULL;
                            }
                            else {
                                tokens_vector->push_back(std::pair<string, char>( building_block.str(), '"' ));
                                building_block.str("");
                                mst.isDoubleBrace = false;
                            }
                            break;
                        }

                        default: std::cout << "ERROR DECODING TOKENS" << std::endl;

                    }
                    continue;
                }
                else { //not braces

                    building_block << ch;
                    std::cout << (int)ch << std::endl;

                }
                ch = static_cast<char> (ss.get()) ;

            }

            if (building_block.rdbuf()->in_avail() != 0) {
                tokens_vector->push_back(std::pair<string, char>(building_block.str(), 'k'));
                building_block.str("");
                mst.isDoubleBrace = false;
            }


//
//            while (char_index < end_of_string){
//                ch = (*input_str).at(char_index);
//
//                size_t founded_char_position = open_pair_symbols.find_first_of(ch);
//                if (founded_char_position != std::string::npos) {
//
//                    switch (ch) {
//
//                        case '"':{
//                            mst.isDoubleBrace = true;
//                            ++char_index;
//                            if (char_index >= end_of_string){
//
//                            }
//                            ch = (*input_str).at(char_index);
//
//                            while (ch != '"' && char_index < end_of_string){
//                                building_block << ch;
//                                ++char_index;
//                                ch = (*input_str).at(char_index);
//                            };
//                            if (char_index == end_of_string) {
//                                std::cout << "ERROR, can't find closing pair for opening char!" << std::endl;
//                                return nullptr;
//                            }
//
//
//                            else {
//                                building_block.
//                            }
//                            break;
//                        }
//
//                        default: std::cout << "ERROR DECODING TOKENS" << std::endl;
//
//                    }
//                    continue;
//                }
//                else { //not braces
//
//                }
//
//
//                /*
//                founded_char_position = delimiters.find_first_of(ch);
//                if (founded_char_position != std::string::npos){
//                    if (mst.isIndependant())
//
//                        if (building_block.rdbuf()->in_avail() == 0) // check stringstream isEmpty
//                            continue;
//
//                        else{ // flushing buffer into our tocken vector
//
//                            tokens_vector->push_back(std::pair(
//                                    building_block.str(), mst.getToken()
//                            ));
//                            building_block.clear();
//                    }
//
//                    if (isSpace(ch)){
//                        building_block << ch;
//                    }
//                    continue;
//                }
//                */
//
//
//                /*
//                founded_char_position = special_symbols.find_first_of(ch);
//                if (founded_char_position != std::string::npos){ //if among special symbols
//
//
//                    switch (ch){
//                        case '#': {
//                            if ( !(mst.isInAnyBrace()) )
//                                mst.isComment = true;
//                            else
//                                building_block<<ch;
//                            break;
//                        }
//                        case ' ':{
//                            if ( mst.isIndependant() ){
//
//                            }
//                        }
//                        default: printf("Error matching pattern occured\n");
//                    }
//                }
//                else{
//                    building_block << ch;
//                }
//
//                continue;
//
//
//                 */
//                ++char_index;
//            }



            for (auto toks: *tokens_vector){
                std::cout << "[" <<toks.first << "]" << " <" << toks.second << "> " << std::endl;
            }



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


