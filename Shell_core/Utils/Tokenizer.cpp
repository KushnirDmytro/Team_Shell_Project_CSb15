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
// * 'v' - variable name
//  'V' -variable value
//  * 'e' - external variable name
//    'E' external variable value
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
            this->tokens_vector_ = new std::vector<token>;

        }

        Tokenizer::~Tokenizer() {
            delete tokens_vector_;
        }


        inline void Tokenizer::flush_buf_to_tokens(std::stringstream *workBuffer) {
            if (workBuffer->rdbuf()->in_avail() != 0) {
                tokens_vector_->push_back(std::pair<string, char>(workBuffer->str(), mst.getToken()));
                std::cout << "Flushing buf [" << (*workBuffer).str() << "]" <<std::endl;
                std::cout << "Vector NOW :" << std::endl;
                for (auto el: *tokens_vector_){
                    std::cout << "str: [" << el.first << "]" << "ch: {"<<  el.second << "}" << std::endl;
                }
                workBuffer->str("");
                mst.clean_all();
            }
        }

        inline vector<token> *Tokenizer::form_result() {
            vector<token> *result_vector = new vector<token>();
            std::swap(result_vector, tokens_vector_); // < ============= not sure here
            return result_vector;
        }
        
        inline bool Tokenizer::lastTokenEquals(const std::string *compare) const{
            bool result;
            if ((tokens_vector_->size() > 0))
                result = strcmp( (tokens_vector_->end())->first.c_str(), (*compare).c_str()) == 0;
            else
                result = false;
            return result;
        }

        inline int Tokenizer::comment_proceed(std::stringstream *source,
                                              std::stringstream *workBuf) {
            char ch;
            if (source->good()) {
                ch = static_cast<char> (source->get());

                while (ch != '\n' && ch != '\r' && ch!= '\\' && source->good()) { //seek for ending of comment
                    (*workBuf) << ch;
                    ch = static_cast<char> (source->get());
                };

                if (!source->good()) { // comment is up to the end of executable string
                    std::cout << "Can't find closing comment statement" << std::endl;
                }
                 else {
                    std::cout << "comment closed" << std::endl;
                }
                flush_buf_to_tokens(workBuf); //flushing value to continue work
                    return EXIT_SUCCESS;
            }
            perror("Reading buffer errror\n");
            return EXIT_FAILURE;
        }

        inline int Tokenizer::pairwise_token_proceed(std::stringstream *source,
                                                     std::stringstream *workBuf) {
            char ch;
            char tokenState = mst.getToken();

            if (source->good()) {
                ch = static_cast<char> (source->get());

                while (ch != tokenState && source->good()) {
                    (*workBuf) << ch;
                    ch = static_cast<char> (source->get());
                };
            }

            if (!(source->good())) {
                perror("ERROR, can't find closing pair for opening char\n");
                return EXIT_FAILURE;
            } else {
                flush_buf_to_tokens(workBuf);
                return EXIT_SUCCESS;
            }
        }


        vector<token> *Tokenizer::tokenize(const string *input_str) {

            std::stringstream ss(*input_str);
            std::stringstream workBuffer;
            char ch;

            // preliminar preparetion
            if (ss.good()) ch = (char) ss.get();
            else {
                std::cout << "EMPTY STRING " << std::endl;
                return form_result();
            }

            while (ss.good()) {
                //check for braces
                size_t founded_char_position = open_pair_symbols_.find_first_of(ch);
                if (founded_char_position != std::string::npos) {

                    flush_buf_to_tokens(&workBuffer);



                    switch (ch) {

                        // =======================checking pairwise tokens ======================
                        case '"': {
                            mst.isDoubleBrace = true;
                            if (pairwise_token_proceed(&ss, &workBuffer))
                                return form_result();  // situation of error
                            break;
                        }
                        case '\'': {
                            mst.isBrace = true;
                            if (pairwise_token_proceed(&ss, &workBuffer))
                                return form_result();  // situation of error
                            break;
                        }
                        case '`': {
                            mst.isReverceBrace = true;
                            if (pairwise_token_proceed(&ss, &workBuffer))
                                return form_result();  // situation of error
                            break;
                        }
                        case '#': {
                            mst.isComment = true;
                            if (comment_proceed(&ss, &workBuffer))
                                return form_result();  // situation of error
                            break;
                        }

                        default:
                            std::cout << "ERROR DECODING TOKENS" << std::endl;

                    }
                    //   workBuffer.str("");
                } else { //===========================================delimiters check

                    founded_char_position = delimiters_.find_first_of(ch);
                    if (founded_char_position != std::string::npos) {

                        flush_buf_to_tokens(&workBuffer);

                        if (ss.good()) {

                            bool isDelimiter = delimiters_.find(ch) != std::string::npos;

                            std::stringstream::pos_type previuos_position = ss.tellg();

                            while (isDelimiter
                                   && ss.good()) {
                                previuos_position = ss.tellg();
                                ch = static_cast<char> (ss.get());
                                isDelimiter = delimiters_.find(ch) != std::string::npos;
                            };
                            if (ss.good())
                                ss.seekg(previuos_position);
                        }

                        if (!ss.good()) {
                          //  std::cout << "ERROR, can't find closing pair for opening char!" << std::endl;
                            return form_result();
                        }



                    } else {
// ==============================proceed other special tokens ===============================
                        founded_char_position = special_symbols_.find_first_of(ch);
                        if (founded_char_position != std::string::npos) {

                            //flush_buf_to_tokens(&workBuffer);

                            switch (ch) {

                                case '_': {
                                }
                                case '*': {
                                }
                                case '%': {
                                    mst.isRegexp = true;
                                    workBuffer << ch; // we will need this char
                                    break;
                                }
                                case '\\': { //actually doing nothing, just
                                    break;
                                }

                                    //=========================+EXECUTION REDIRECTION BLOCK
                                case '>': {
                                    flush_buf_to_tokens(&workBuffer);
                                    if (strcmp(workBuffer.str().c_str(), "2") == 0) {
                                        tokens_vector_->push_back(token("", '2')); //STD ERR
                                        workBuffer.str("");
                                    } else
                                        tokens_vector_->push_back(token("", '>')); // STD OUT
                                    break;
                                }
                                case '<': {
                                    flush_buf_to_tokens(&workBuffer);
                                    tokens_vector_->push_back(token("", '<')); //STD IN
                                    break;
                                }
                                case '&': {
//
//                                    if (lastTokenEquals(new string("<"))) TODO consider situation with double redirect
                                    tokens_vector_->push_back(token("", '&')); //silent backstage mode for previous
                                    break;
                                } // TODO make silent mode
                                case '|': {
                                    tokens_vector_->push_back(token("", '|')); //conveyor
                                    break;
                                }

                                    //=========================+EXECUTION REDIRECTION BLOCK =============END=============


                                case '$': {
                                    flush_buf_to_tokens(&workBuffer); //proved

                                    // TODO check if such a variable is defined
                                    mst.isVariableCall = true;
                                    break;
                                }

                                case '=': {

                                    bool isGlobal = false;
                                    mst.isVariableName = true;
                                    isGlobal = lastTokenEquals(new string("export"));
                                    mst.isGlobal = isGlobal;
                                    flush_buf_to_tokens(&workBuffer); //previous token part proceeded
                                    mst.isVariableValue = true;
                                    mst.isGlobal = isGlobal;


                                    // TESTBLOCK

                                    environment->varManager_->declareVariableLocally(new string ("_a"), new string ("b"));
                                    environment->varManager_->declareVariableGlobally(new string("_a"),
                                    new string("testVal1"), false);
                                    environment->varManager_->declareVariableGlobally(new string("_a"),
                                                                                      new string("testVal2"), true);

                                    environment->varManager_->declareVariableGlobally(new string("_a"),
                                                                                      new string("testVal3"), false);


                                    std::cout << environment->varManager_->getGlobalVar(new string("_a"))->c_str() << std::endl;

                                    environment->varManager_->show_local_variables();

                                    // TODO REALIZE IDEA OF ENVIRONMENT
                                     break;
                                }

                                default:
                                    std::cout << "ERROR DECODING TOKENS" << std::endl;

                            }

                        }
                        else{
                            // ============================== unspecified symbols processing=======================

                            workBuffer << ch;
                            std::cout << (char)ch << std::endl;
                        }

                    }

                }

                ch = static_cast<char> (ss.get());
                }

            flush_buf_to_tokens(&workBuffer);

            for (auto toks: *tokens_vector_) {
                std::cout << "[" << toks.first << "]" << " <" << toks.second << "> " << std::endl;
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