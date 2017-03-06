//  Created by National Center for Media Forensics
//  University of Colorado Denver
//  ImageLaunder v.1 10/28/2016
//
//  main.cpp
//
#include "Header.h"
#include "Facebook1.h"
#include "Facebook2.h"
#include "Facebook3.h"
#include "Facebook4.h"

int main(int argc, const char * argv[]) {
/*
    if(argc != 3){
        cout << "ERROR occur on input parameters" << endl;
        exit(-1);
        
    }
    else{
        
        original_image_name = argv[1];
        header_number = argv[2];
        
    }*/
    
    InitializeMagick(*argv);
    
    /***    DELETE AND UNCOMMENT ABOVE  ****/
    original_image_name = "original.png";
    header_number = "1";
    /***    DELETE AND UNCOMMENT ABOVE  ****/

    input_type = image_type(original_image_name);
    original_image_name = convertToJPEG(original_image_name);
    const char* char_name = original_image_name.c_str();
    
    int case_number;
    stringstream convert(header_number);
    convert>>case_number;
    
    if(case_number > 4){
        
        cout << "ERROR, Case Number must be 1, 2, 3, or 4" << endl;
        exit(-1);
    }
    switch (case_number) {
        case 1:
            header_name = header_name + "/FB01";
            Facebook_1(char_name);
            combineImage_1(output_image_name, readHeader_1(header_name),saveQT_1(original_image_name));
            MD5_rename();
            std::remove(temp_name_char);
            break;
        case 2:
            header_name = header_name + "/FB02";
            Facebook_2(char_name);
            combineImage_2(output_image_name, readHeader_2(header_name),saveQT_2(original_image_name));
            MD5_rename();
            std::remove(temp_name_char);
            break;

        case 3:
            header_name = header_name + "/FB03";
            Facebook_3(char_name);
            combineImage_3(output_image_name, readHeader_3(header_name),saveQT_3(original_image_name));
            MD5_rename();
            std::remove(temp_name_char);
            break;

        case 4:
            header_name = header_name + "/FB04";
            Facebook_4(char_name);
            combineImage_4(output_image_name, readHeader_4(header_name),saveQT_4(original_image_name));
            MD5_rename();
            std::remove(temp_name_char);
            break;
    }
    return 0;
}
