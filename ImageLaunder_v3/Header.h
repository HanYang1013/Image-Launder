//  Created by National Center for Media Forensics
//  University of Colorado Denver
//  ImageLaunder v.1 10/28/2016
//
//  Header.h


#ifndef Header_h
#define Header_h
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <string>
#include <sstream>
#include <time.h>
#include <cstdio>
#include <setjmp.h>
#include <stdint.h>

#include "jpeglib.h"
#include <Magick++.h>
#include "MD5.h"


using namespace std;
using namespace Magick;

template< typename T >
string int_to_hex( T i )
{
    stringstream stream;
    stream << hex << i;
    return stream.str();
}

static const char alphanum[] =
"0123456789"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz";

char genRandom()  // Random string generator function.
{
    return alphanum[rand() % (sizeof(alphanum) - 1)];
}


void MD5_rename(){
    char oldname[] = "output.jpg";
    char newname[] = " ";
    string hash = md5("output.jpg");
    hash = hash + ".jpg";
    strcpy(newname, hash.c_str());
    std::rename(oldname,newname);
}

int image_type(string filename){
    int type = 0;
    istringstream iss(filename);
    vector<std::string> tokens;
    string token;
    while (getline(iss, token, '.')) {
        if (!token.empty())
            tokens.push_back(token);
    }
    if(tokens[tokens.size()-1] == "jpg" || tokens[tokens.size()-1] == "jpeg"){
        type = 1;
    }
    else{
        type = 0;
    }
    return type;
}

string temp_name = "temp_name.jpg"; // only be used when file is not jpeg
const char * temp_name_char = temp_name.c_str();
string convertToJPEG(string filename){
    Image originalImage(filename);
    originalImage.quality(100);
    originalImage.write(temp_name);
    return temp_name;
    
}

/*Facebook Header 1 section*/
void Facebook_1( const char * inputfilename);
void Facebook_1_unjpg(Image originalImage,int quality_number);
char * saveQT_1(string filename);
char * readHeader_1(string filename);
int  QT1 (string filename);
void combineImage_1(string filename, char * headerBuffer,char * storeBuffer);
/*Facebook Header 1 section end*/

/*Facebook Header 2 section*/
void Facebook_2( const char * inputfilename); //resize, recompress, DCT
void Facebook_2_unjpg(Image originalImage,int quality_number);
char * saveQT_2(string filename); //find ffda and save ffda to end end to header buffer
char * readHeader_2(string filename); //save and change header to header buffer
int  QT2 (string filename);
void combineImage_2(string filename, char * headerBuffer,char * storeBuffer);
/*Facebook Header 2 section end*/

/*Facebook Header 3 section*/
void Facebook_3( const char * inputfilename); //resize, recompress, DCT
void Facebook_3_unjpg(Image originalImage,int quality_number);
char * saveQT_3(string filename); //find ffda and save ffda to end end to header buffer
char * readHeader_3(string filename); //save and change header to header buffer
int  QT3 (string filename);
void combineImage_3(string filename, char * headerBuffer,char * storeBuffer);
/*Facebook Header 3 section end*/

/*Facebook Header 4 section*/
void Facebook_4( const char * inputfilename); //resize, recompress, DCT
void Facebook_4_unjpg(Image originalImage,int quality_number);
char * saveQT_4(string filename); //find ffda and save ffda to end end to header buffer
char * readHeader_4(string filename); //save and change header to header buffer
int  QT4 (string filename);
void combineImage_4(string filename, char * headerBuffer,char * storeBuffer);
/*Facebook Header 4 section end*/

/*variables section*/
string original_image_name = " "; //original image name
string output_image_name = "output.jpg"; // result image name
string header_name = "resources"; //facebook header name
string header_number = "0"; //header number depends on user's input

int headerSize; // size of header a
int storeSize; // size from FFDA to end
int QTsize; //quantization table size
char * finalBuffer = nullptr;
int input_type = 0;
/*variables section end*/

#endif /* Header_h */
