//
// Created by d1md1m on 18.06.17.
//

#ifndef MSHELL_TOKENIZER_H
#define MSHELL_TOKENIZER_H

#include <cstring>
#include <vector>
#include <string>
#include <boost/filesystem.hpp>

using std::vector;
using std::string;
using std::vector;
using token = std::pair<string, char>;
namespace sh_core {

    namespace utils {

        struct machine_state{
            bool isComment = false;
            bool isDoubleBrace = false;
            bool isBrace = false;
            bool isReverceBrace = false;
            bool isRegexp = false;
            bool isVariableCall = false;
            bool isVariableName = false;
            bool isVariableValue = false;
            bool isGlobal = false;
            bool ERROR_STATE = false;

//            bool isGlobalVar = false;

            void clean_all(){
                isVariableCall = false;
                isVariableName = false;
                isVariableValue = false;
                isComment = false;
                isDoubleBrace = false;
                isBrace = false;
                isReverceBrace = false;
                isRegexp = false;
                isGlobal = false;
                ERROR_STATE = false;
//                isGlobalVar = false;
            }

            bool isInAnyBrace(){
                return  (isDoubleBrace || isBrace || isReverceBrace);
            }

            bool isIndependant(){
                return !(isComment||isDoubleBrace || isBrace || isReverceBrace);
            }

            char getToken(){
                if (ERROR_STATE) return 'X';
                if (isComment) return '#';
                if (isDoubleBrace) return '"';
                if (isBrace) return '\'';
                if (isReverceBrace) return '`';
                if (isRegexp) return '%';
                //if (isGlobalVar) return 'G';
                if (isVariableCall) return '$';
                if (isVariableName) {
                    if (isGlobal) return 'e' ;
                    else return 'v';}
                if (isVariableValue) {
                    if (isGlobal) return 'E' ;
                    else return 'V';}
                else return 's';
            }

        };

        class Tokenizer {
        private:
            machine_state mst;

            const std::string special_symbols_ = "%_*/|><.&=\\";
            const std::string open_pair_symbols_ = "'\"`#";
            const std::string delimiters_ = " \t\r\n\a";


            vector<token> *tokens_vector_;
            int pairwise_token_proceed(std::stringstream *source,
                                       std::stringstream *workBuf);

            int comment_proceed(std::stringstream *source,
                                std::stringstream *workBuf);
            bool lastTokenEquals(const std::string *compare) const;

        public:

            bool isBadVarName(std::stringstream* candidate) const;

            Tokenizer();

            vector<token> * tokenize(const string *str);

            ~Tokenizer();
            void flush_buf_to_tokens(std::stringstream *workBuffer);
            inline vector<token>* form_result();

            };

    }
}




#endif //MSHELL_TOKENIZER_H
