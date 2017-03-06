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

stringstream timestring;
template< typename T >
void time_t_to_string(T t)
{
    timestring << t;
}

time_t raw_time_generator(){
    srand(time(NULL));
    time_t raw_time = (time_t)(rand() % 100 * 795927 + 1391212800);
    //rand() % 100 generates a number between 0 and 99, raw time is 1391212800 to 1470009600
    return raw_time;
}

time_t raw_time = raw_time_generator();

char * randomTime(){
    string time = " ";
    char * timeBuffer = new char [19];
    struct tm *ptm = gmtime(&raw_time);
    /*Year*/
    time_t_to_string(ptm->tm_year + 1900);
    time_t_to_string(":");
    
    /*Month*/
    if(ptm->tm_mon + 1 < 10){
        time_t_to_string(0);
        time_t_to_string(ptm->tm_mon + 1);
    }
    else
        time_t_to_string(ptm->tm_mon + 1);
    time_t_to_string(":");
    
    /*Day*/
    if(ptm->tm_mday < 10){
        time_t_to_string(0);
        time_t_to_string(ptm->tm_mday);
    }
    else
        time_t_to_string(ptm->tm_mday);
    time_t_to_string(" ");
    
    /*Hour*/
    if(ptm->tm_hour < 10){
        time_t_to_string(0);
        time_t_to_string(ptm->tm_hour);
    }
    else
        time_t_to_string(ptm->tm_hour);
    time_t_to_string(":");
    
    /*Minute*/
    if(ptm->tm_min < 10){
        time_t_to_string(0);
        time_t_to_string(ptm->tm_min);
    }
    else
        time_t_to_string(ptm->tm_min);
    time_t_to_string(":");
    
    /*Second*/
    if(ptm->tm_sec < 10){
        time_t_to_string(0);
        time_t_to_string(ptm->tm_sec);
    }
    else
        time_t_to_string(ptm->tm_sec);
    
    time = timestring.str();
    timestring.str("");
    strcpy(timeBuffer, time.c_str());
    
    return timeBuffer;
    
}

char * randomTimeT(){
    string time = " ";
    char * timeBuffer = new char [19];
    struct tm *ptm = gmtime(&raw_time);
    /*Year*/
    time_t_to_string(ptm->tm_year + 1900);
    time_t_to_string("-");
    
    /*Month*/
    if(ptm->tm_mon + 1 < 10){
        time_t_to_string(0);
        time_t_to_string(ptm->tm_mon + 1);
    }
    else
        time_t_to_string(ptm->tm_mon + 1);
    time_t_to_string("-");
    
    /*Day*/
    if(ptm->tm_mday < 10){
        time_t_to_string(0);
        time_t_to_string(ptm->tm_mday);
    }
    else
        time_t_to_string(ptm->tm_mday);
    time_t_to_string("T");
    
    /*Hour*/
    if(ptm->tm_hour < 10){
        time_t_to_string(0);
        time_t_to_string(ptm->tm_hour);
    }
    else
        time_t_to_string(ptm->tm_hour);
    time_t_to_string(":");
    
    /*Minute*/
    if(ptm->tm_min < 10){
        time_t_to_string(0);
        time_t_to_string(ptm->tm_min);
    }
    else
        time_t_to_string(ptm->tm_min);
    time_t_to_string(":");
    
    /*Second*/
    if(ptm->tm_sec < 10){
        time_t_to_string(0);
        time_t_to_string(ptm->tm_sec);
    }
    else
        time_t_to_string(ptm->tm_sec);
    
    time = timestring.str();
    timestring.str("");
    strcpy(timeBuffer, time.c_str());
    
    return timeBuffer;
}


static const char alphanum[] =
"0123456789"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz";

char genRandom()  // Random string generator function.
{
    return alphanum[rand() % (sizeof(alphanum) - 1)];
}


char * resolutionBuffer = new char [4];
void resolution(int height, int width){
    unsigned char height_high = (height>>8) & 0xff;
    unsigned char height_low = height & 0xff;
    
    unsigned char width_high=(width>>8) & 0xff;
    unsigned char width_low = width & 0xff;
    
    resolutionBuffer[0] = height_high;
    resolutionBuffer[1] = height_low;
    resolutionBuffer[2] = width_high;
    resolutionBuffer[3] = width_low;
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
struct tm * gmtime (const time_t * timer);
time_t raw_time_generator(); // generate second passed
char * randomTime(); //generate a random time string with format 1
char * randomTimeT(); // generate a random time string with format 2
void Facebook_1(const char * inputfilename); //resize, recompress, DCT
void Facebook_1_unjpg(Image originalImage,int quality_number);
char * findFFDA_1(string filename); //find ffda and save ffda to end end to store buffer
char * readHeader_a_1(string filename); //save and change header to header buffer
char * readHeader_b_1(string filename);
void combineImage_1(string filename, char * headerBuffer,char *headerBuffer_b, char * storeBuffer); //combine
char * QT1 (string filename);
char * HT1 (string filename);
/*Facebook Header 1 section end*/

/*Facebook Header 2 section*/
char genRandom();  // Random string generator function.
void Facebook_2(const char * inputfilename); //resize, recompress, DCT
void Facebook_2_unjpg(Image originalImage,int quality_number);
char * findFFDA_2(string filename); //find ffda and save ffda to end end to header buffer
char * readHeader_a_2(string filename); //save and change header to header buffer
char * readHeader_b_2(string filename); //save and change header to header buffer
void combineImage_2(string filename, char * headerBuffer,char *headerBuffer_b, char * storeBuffer); //combinevoid renameImage2(string filename);
char * QT2 (string filename);
char * HT2 (string filename);
/*Facebook Header 2 section end*/

/*Facebook Header 3 section*/
void Facebook_3( const char * inputfilename); //resize, recompress, DCT
void Facebook_3_unjpg(Image originalImage,int quality_number);
char * findFFDA_3(string inputfilename); //find ffda and save ffda to end end to header buffer
char * readHeader_a_3(string filename); //save and change header to header buffer
char * readHeader_b_3(string filename);
char * readHeader_c_3(string filename);
char * HT3 (string filename);
char * QT3 (string filename);
void combineImage_3(string filename, char * headerBuffer,char *headerBuffer_b, char *headerBuffer_c, char * storeBuffer);
/*Facebook Header 3 section end*/

/*Facebook Header 4 section*/
void Facebook_4( const char * inputfilename); //resize, recompress, DCT
void Facebook_4_unjpg(Image originalImage,int quality_number);
char * saveQT_4(string filename); //find ffda and save ffda to end end to header buffer
char * readHeader_a_4(string filename); //save and change header to header buffer
int  QT4 (string filename);
void combineImage_4(string filename, char * headerBuffer,char * storeBuffer);
/*Facebook Header 4 section end*/

/*variables section*/
string original_image_name = " "; //original image name
string output_image_name = "output.jpg"; // result image name
string header_name_a = "IMAGE_PROCESS_INPUT_DATA"; //facebook header name
string header_name_b = "IMAGE_PROCESS_INPUT_DATA"; //facebook header name
string header_name_c = "IMAGE_PROCESS_INPUT_DATA"; //facebook header name
string header_number = "0"; //header number depends on user's input

int headerSize_a; // size of header a
int headerSize_b; //size of header b
int headerSize_c; //size of header c
int storeSize; // size from FFDA to end
int HTsize; // huffmantable size
int QTsize; //quantization table size
int input_type = 0;
/*variables section end*/

#endif /* Header_h */
