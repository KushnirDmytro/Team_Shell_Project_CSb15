//
// Created by d1md1m on 28.03.17.
//

#include "FileLaneIterator.h"



FileLaneIterator::FileLaneIterator(string filename){
if (boost::filesystem::exists(filename)){
infile.open(filename);

//cout << "ISOPEN:  " << infile.is_open() <<endl;
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

//cout << "ISOPEN" << infile.is_open() <<endl;
//cout << "EOF:  "  << infile.eof() << endl;
//cout << "GOOD:  "  << infile.good() << endl;
//ifstream infile(filename);
}; //initialize via passing filename to open


bool FileLaneIterator::fileIsReady(){
    return (infile.good() && infile.is_open());
}

void FileLaneIterator::getNextString(string *buf){

    //  cout << "ISOPEN" << infile.is_open() <<endl;
    // cout << "EOF:  "  << infile.eof() << endl;
    // cout << "GOOD:  "  << infile.good() << endl;
    // getchar();
    if (infile.is_open() && !infile.eof()){

        std::getline(infile, *buf);
        //infile.getline(buf, 512);
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