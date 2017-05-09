#include <iostream>
#include <vector>
#include <string>
#include "deleter.h"
#include <vector>
#include <boost/filesystem.hpp>
#include <sstream>

using namespace std;
int main(int argc, char **argv) {
    int args_size = 0;
    while(argv[args_size] != NULL) {
        args_size += 1;
    }
    vector<string> vector_two_strings(argv, argv + args_size);
    istringstream iss(vector_two_strings[1]);
    vector<string> tokens;
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter(tokens));
    bool is_R = false;
    bool is_f = false;
    bool is_help = false;

    if(find(tokens.begin(), tokens.end(), "-f") != tokens.end()) {
        is_f = true;
        tokens.erase(remove(tokens.begin(), tokens.end(),"-f")), tokens.end();

    }
    if(find(tokens.begin(), tokens.end(), "-R") != tokens.end()) {
        is_R = true;
        tokens.erase(remove(tokens.begin(), tokens.end(),"-R")), tokens.end();
    }
    if(find(tokens.begin(), tokens.end(), "-h") != tokens.end()) {
        is_help = true;
        tokens.erase(remove(tokens.begin(), tokens.end(),"-h")), tokens.end();
    } else if(find(tokens.begin(), tokens.end(), "--help") != tokens.end()) {
        is_help = true;
        tokens.erase(remove(tokens.begin(), tokens.end(),"--help")), tokens.end();
    }
    deleter(tokens, is_help, is_R, is_f);
}