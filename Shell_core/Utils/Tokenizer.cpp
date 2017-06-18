// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Created by d1md1m on 18.06.17.
//


#include <cstring>
#include <vector>
#include <string>
#include <boost/filesystem.hpp>
#include <sstream>
#include <iostream>
#include "../EmbeddedFunc.h"



#include "Tokenizer.h"



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

namespace sh_core {

    namespace utils {
        Tokenizer::Tokenizer() {
           this->tokens_vector_ = new std::vector <token>;

        }

        Tokenizer::~Tokenizer() {
            delete tokens_vector_;
        }


        inline void Tokenizer::flush_buf_to_token (std::stringstream* workBuffer){
            if (workBuffer->rdbuf()->in_avail() != 0) {
                tokens_vector_->push_back(std::pair<string, char>(workBuffer->str(), mst.getToken()));
                workBuffer->str("");
                mst.clean_all();
            }
        }

        inline vector<token>* Tokenizer::form_result(){
            vector<token> *result_vector = new vector<token>();
            std::swap(result_vector, tokens_vector_); // < ============= not sure here
            return result_vector;
        }

        inline int Tokenizer::solve_pairwise_token(std::stringstream *source,
                                                              std::stringstream *workBuf){
            char ch;
            char tokenState = mst.getToken();
            if (tokenState == '#')
                // TODO consider multilinear comments proceeding
               printf("comment occuerd\n");
            else if (source->good()) {
                ch = static_cast<char> (source->get());

                while (ch != tokenState && source->good()) {
                    (*workBuf) << ch;
                    ch = static_cast<char> (source->get());
                };
            }

            if (source->bad()) {
                std::cout << "ERROR, can't find closing pair for opening char!" << std::endl;
                return EXIT_FAILURE;
            }

            else  {
                flush_buf_to_token(workBuf);
                return EXIT_SUCCESS;
            }
        }


        vector<token> * Tokenizer::tokenize(const string * input_str) {

            std::stringstream ss(*input_str);
            std::stringstream workBuffer;
            char ch;

            // preliminar preparetion
            if (ss.good()) ch = (char)ss.get();
            else {
                std::cout<< "EMPTY STRING "<<std::endl;
                return form_result();
            }

            while (ss.good()){
                //check for braces
                size_t founded_char_position = open_pair_symbols_.find_first_of(ch);
                if (founded_char_position != std::string::npos) {

                    flush_buf_to_token(&workBuffer);

                    switch (ch) {

                        case '"':{
                            mst.isDoubleBrace = true;
                            if (solve_pairwise_token(&ss, &workBuffer))
                                return form_result();  // situation of error
                            break;
                        }
                        case '\'':{
                            mst.isBrace = true;
                            if (solve_pairwise_token(&ss, &workBuffer))
                                return form_result();  // situation of error
                            break;
                        }
                        case '`':{
                            mst.isReverceBrace = true;
                            if (solve_pairwise_token(&ss, &workBuffer))
                                return form_result();  // situation of error
                            break;
                        }
                        case '#':{
                            mst.isComment = true;
                            if (solve_pairwise_token(&ss, &workBuffer))
                                return form_result();  // situation of error
                            break;
                        }

                        default: std::cout << "ERROR DECODING TOKENS" << std::endl;

                    }
                }
                else { //not braces

                    workBuffer << ch;
                    std::cout << (int)ch << std::endl;

                }

                ch = static_cast<char> (ss.get()) ;

            }

            flush_buf_to_token(&workBuffer);


            for (auto toks: *tokens_vector_){
                std::cout << "[" <<toks.first << "]" << " <" << toks.second << "> " << std::endl;
            }
            return form_result();
        }

        }

    }





//
//            while (char_index < end_of_string){
//                ch = (*input_str).at(char_index);
//
//                size_t founded_char_position = open_pair_symbols_.find_first_of(ch);
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
//                founded_char_position = delimiters_.find_first_of(ch);
//                if (founded_char_position != std::string::npos){
//                    if (mst.isIndependant())
//
//                        if (building_block.rdbuf()->in_avail() == 0) // check stringstream isEmpty
//                            continue;
//
//                        else{ // flushing buffer into our tocken vector
//
//                            tokens_vector_->push_back(std::pair(
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
//                founded_char_position = special_symbols_.find_first_of(ch);
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



//vector<token>  tokenVec(){
