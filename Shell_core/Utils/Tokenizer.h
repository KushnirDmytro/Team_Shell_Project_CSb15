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
                if (isComment) return '#';
                if (isDoubleBrace) return '"';
                if (isBrace) return '\'';
                if (isReverceBrace) return '`';
                if (isRegexp) return '%';
                else return 's';
            }

        };

        class Tokenizer {
        private:
            machine_state mst;

            const std::string special_symbols_ = "'\"`%/#|><._*&=";
            const std::string open_pair_symbols_ = "'\"`#";
            const std::string delimiters_ = " \t\r\n\a";


            vector<token> *tokens_vector_;
            int solve_pairwise_token(std::stringstream *source,
                                                std::stringstream *workBuf);

        public:
            Tokenizer();

            vector<token> * tokenize(const string *str);

            ~Tokenizer();
            void flush_buf_to_token (std::stringstream* workBuffer);
            inline vector<token>* form_result();

            };

    }
}




#endif //MSHELL_TOKENIZER_H
