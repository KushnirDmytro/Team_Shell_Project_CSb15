//
// Created by d1md1m on 20.06.17.
//

#include <iostream>
#include "Reducer.h"
#include "Tokenizer.h"


namespace shell_core {
    namespace utils {

        Reducer::Reducer() {}

        bool Reducer::closesExecutionUnit(const char ch) const {
            return  ( ch=='\n' || ch=='.' || ch=='|' );
        }


        inline int create_new_exec_unit(arg_desk_pair* unit_addr){
            unit_addr = new std::pair < std::vector < std::string * > *, chennelDesriptStruct * >;
            unit_addr->first = new std::vector < std::string * >;
            unit_addr->second = new chennelDesriptStruct;
            return EXIT_SUCCESS;
        }

        std::vector<arg_desk_pair*>* Reducer::reduce(const vector<token> *toks) {

            for(auto el: *toks){
                std::cout<<"st[" <<el.first << "]---{" << el.second << "}\n";
            }

            std::vector<arg_desk_pair*> *res = new std::vector<arg_desk_pair* >;


            arg_desk_pair *new_execution_unit = nullptr;
            create_new_exec_unit(new_execution_unit);


            //TODO consider do we need it??
            chennelDesriptStruct* next_operation_descriptor = new chennelDesriptStruct;

            int *pipeSides [2];


            if (!toks->empty()){

            } else{
                perror("Empty taks\n");
                return res;
            }

            auto it = toks->front();

            bool  first_node = true;

            for (auto el : *toks ){

                if (first_node){
                    if (el.second == '\n'){
                        //Need to skip this step
                        continue;
                    }

                    create_new_exec_unit(new_execution_unit);

                    if (RS.isConveyerOpened){
                        new_execution_unit->second->indeskPtr = pipeSides[READ_SIDE];
                        RS.isConveyerOpened = false;
                    }
                    first_node = false;
                }

                switch (el.second){
                    case 's': {
                        new_execution_unit->first->push_back(new string(el.first));
                        break;
                    }
                    case 'v':{
             //           environment->varManager_->declareVariableLocally(new string ("_a"),
//                                                                                     new string ("b"));
//
                    }
                    default: perror("unknown task token\n");
                }

                if ( closesExecutionUnit(el.second) ) {

                    // solving conveyer request

                    if (el.second == '|') {
                        if (!RS.isConveyerOpened)
                            RS.isConveyerOpened = true;
                        else
                            perror("Cant provide Piping, allready bussy\n");
                        pipe(*pipeSides);
                    }

                    if (el.second == '.') {
                        res->push_back(new_execution_unit);
                        std::cout << "met END_OF_TASK\n";
                        break;
                    }

                }



//                switch (el.second){
//                    case '\n':      //just EOL
//                    case '.' :      //time to sent result to executioner
//                    case '|' :      // need to alter channelDescriptor
//                }



            }

            return res;

        }

        Reducer::~Reducer() {}

    }
}
