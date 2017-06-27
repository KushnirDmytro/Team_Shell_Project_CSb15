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
using token = std::pair<string, char>;

namespace sh_core {

    namespace utils {

        struct machine_state{
            bool toIgnoreDelimitersAndComments = false; // do not reset autimatically
            bool ERROR_STATE = false;
            bool isExtention = false;
            bool isComment = false;
            bool isDoubleBrace = false;
            bool isInnerCommand = false;
            bool isOuterCommand = false;
            bool isBrace = false;
            bool isReverceBrace = false;
            bool isRegexp = false;
            bool isVariableCall = false;
            bool isVariableName = false;
            bool isVariableValue = false;
            bool isGlobal = false;
            bool isFile = false;
            bool isDirectory = false;
            bool isMshScript = false;

//            bool isGlobalVar = false;

            void clean_all(){
                ERROR_STATE = false;
                isVariableCall = false;
                isExtention = false;
                isVariableName = false;
                isVariableValue = false;
                isComment = false;
                isDoubleBrace = false;
                isBrace = false;
                isReverceBrace = false;
                isRegexp = false;
                isGlobal = false;
                isFile = false;
                isDirectory = false;
                isInnerCommand = false;
                isOuterCommand = false;
                isMshScript = false;
//                isGlobalVar = false;
            }

            bool isInAnyBrace(){
                return  (isDoubleBrace || isBrace || isReverceBrace);
            }

            bool isIndependant(){
                return !(isComment||isDoubleBrace || isBrace || isReverceBrace);
            }

            char getToken(){
                if (ERROR_STATE) return '!';
                if (isVariableCall) return '$';
                if (isFile) return 'f';
                if (isMshScript) return 'm';
                if (isDirectory) return 'd';
                if (isInnerCommand) return 'i';
                if (isOuterCommand) return 'o';
                if (isComment) return '#';
                if (isDoubleBrace) return '"';
                if (isBrace) return '\'';
                if (isReverceBrace) return '`';
                if (isRegexp) return '%';
                //if (isGlobalVar) return 'G';
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

            const std::string special_symbols_ = "$%_*/|><.&=\\";
            const std::string open_pair_symbols_ = "'\"`#";
            const std::string delimiters_ = " \t\r\n\a";
            const std::string EOL_ = "\r\n\a";


            vector<token> *tokens_vector_;
            int pairwise_token_proceed(std::stringstream *source,
                                       std::stringstream *workBuf);

            int comment_proceed(std::stringstream *source,
                                std::stringstream *workBuf);
            bool lastTokenStringEquals(const std::string *compare) const;

            bool lastCreatedTokenMarksEqual(const char compare) const;

            bool canBeVariableName(string* to_check);
            int concretize_attempt(std::stringstream *workBuffer);

        public:
            Tokenizer();

            int setIgnoreDelimiters(bool value);

            int getIgnoreDeliminers(bool &valueAddr);

            vector<token> * tokenize(const string *str);

            ~Tokenizer();
            void flush_buf_to_tokens(std::stringstream *workBuffer);


            void TRUNC_buf(std::stringstream *workBuffer);

            inline vector<token>* form_result();

            };

    }
}




#endif //MSHELL_TOKENIZER_H
