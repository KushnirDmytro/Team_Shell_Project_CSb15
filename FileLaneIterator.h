//
// Created by d1md1m on 28.03.17.
//

#ifndef LAB_2_SHELL_FILEITERATOR_H
#define LAB_2_SHELL_FILEITERATOR_H

#include <array>
#include <fstream>
#include <boost/filesystem/operations.hpp>

using namespace std;

class FileLaneIterator{
private:
    ifstream infile;
    bool isGood;
public:

    FileLaneIterator(string filename){
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


    bool fileIsReady(){
        return (infile.good() && infile.is_open());
    }

    void getNextString(string *buf){

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

    ~FileLaneIterator(){
        if(infile.is_open()){
            infile.close();
        }
    }
};


#endif //LAB_2_SHELL_FILEITERATOR_H
