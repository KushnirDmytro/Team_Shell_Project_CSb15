//
// Created by d1md1m on 20.06.17.
//

#include "Reducer.h"
#include "Tokenizer.h"


namespace shell_core {
    namespace utils {

        Reducer::Reducer() {}

        bool Reducer::closesExecutionUnit(const char ch) const {
            return  ( ch=='\n' || ch=='.' || ch=='|' );
        }

        std::vector<arg_desk_pair*>* Reducer::reduce(const vector<token> *toks) {
            std::vector<arg_desk_pair*> *res = new std::vector<arg_desk_pair* >;


            arg_desk_pair *new_execution_unit =
                    new std::pair < std::vector < std::string * > *, chennelDesriptStruct * >;

            new_execution_unit->first = new std::vector < std::string * >;
            new_execution_unit->second = new chennelDesriptStruct;




            for (auto el : *toks ){


//                switch (el.second){
//                    case '\n':      //just EOL
//                    case '.' :      //time to sent result to executioner
//                    case '|' :      // need to alter channelDescriptor
//                }




                if ( closesExecutionUnit(el.second) ) {

                    if (el.second == '|')
                        isConveyerOpened = true;

                    if (el.second == '.') {
                        res->push_back(new_execution_unit);
                        break;
                    }
                }



            }

            return res;

        }

        Reducer::~Reducer() {}

    }
}
