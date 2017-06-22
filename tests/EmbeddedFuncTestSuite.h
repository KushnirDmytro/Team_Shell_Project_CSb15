//
// Created by bogdan on 21.06.17.
//

#ifndef MSHELL_EMBEDDEDFUNCTESTSUITE_H
#define MSHELL_EMBEDDEDFUNCTESTSUITE_H

#include <cxxtest/TestSuite.h>
#include "../Shell_core/EmbeddedFunc.h"

int myLsStaticLauncher(size_t nargs, char **argv) {
    // this class is needed only to be static initializer but can instead incapsulate behaviour
    // I just decide not to get behaviour out of class in generic case

    //printf("Initialization and checks performed successfully\n");
    return 0;
}

class EmbeddedFuncDerived : public sh_core::EmbeddedFunc{
    public:
    EmbeddedFuncDerived(const string &name, callable_function funct_to_assign,
                                const string &help_msg)
            :sh_core::EmbeddedFunc(name, funct_to_assign, help_msg){

    }
    int call(size_t nargs, char **args){
        return sh_core::EmbeddedFunc::call(nargs, args);
    }
};

class EmbeddedFuncTestSuite : public CxxTest::TestSuite
{

private:
    EmbeddedFuncDerived * embeddedFunc;
public:
//    void testNeedToPrintHelp(void) {
//
//
//
//        TS_ASSERT_EQUALS(embeddedFunc->needToPrintHelp(0, cargs), true);
//
//        embeddedFunc = new sh_core::EmbeddedFunc ("test_func", myLsStaticLauncher, "test help message", false);
//        TS_ASSERT_EQUALS(embeddedFunc->needToPrintHelp(0, cargs), false);
//    }

    void testCall(void) {
        char** cargs = new char*[0];
        embeddedFunc = new EmbeddedFuncDerived ("test_func", myLsStaticLauncher, "test help message");
        TS_ASSERT_EQUALS(embeddedFunc->call(0, cargs), 0);
    }
};

#endif //MSHELL_EMBEDDEDFUNCTESTSUITE_H
