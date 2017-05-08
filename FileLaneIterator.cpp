// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Created by d1md1m on 28.03.17.
//

#include "FileLaneIterator.h"



FileLaneIterator::FileLaneIterator(string filename){
if (boost::filesystem::exists(filename)){
infile.open(filename);

if (infile.is_open()){
isGood = true;
printf("FILE {%s} IS OPENED\n", filename.c_str());
}
else{
printf("%s Could not be open\n", filename.c_str());
}
}
else{
perror("Such a file does not found\n");
isGood = false;
}
}; //initialize via passing filename to open


bool FileLaneIterator::fileIsReady(){
    return (infile.good() && infile.is_open());
}

void FileLaneIterator::getNextString(string *buf){
    if (infile.is_open() && !infile.eof()){
        std::getline(infile, *buf);
    }
    else {
        isGood = false;
    }
}

FileLaneIterator::~FileLaneIterator(){
    if(infile.is_open()){
        infile.close();
    }
}