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
    
    original_image_name = "original.jpg";
    input_type = image_type(original_image_name);
    original_image_name = convertToJPEG(original_image_name);
    
    header_number = "4";
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
            header_name_a = header_name_a + "/FB01/FB01a";
            header_name_b = header_name_b + "/FB01/FB01b";
            Facebook_1(char_name);
            combineImage_1(output_image_name, readHeader_a_1(header_name_a), readHeader_b_1(header_name_b), findFFDA_1(original_image_name));
            MD5_rename();
            std::remove(temp_name_char);
            break;
        case 2:
            header_name_a = header_name_a + "/FB02/FB02a";
            header_name_b = header_name_b + "/FB02/FB02b";
            Facebook_2(char_name);
            combineImage_2(output_image_name, readHeader_a_2(header_name_a), readHeader_b_2(header_name_b), findFFDA_2(original_image_name));
            MD5_rename();
            std::remove(temp_name_char);
            break;
        case 3:
            header_name_a = header_name_a + "/FB03/FB03a";
            header_name_b = header_name_b + "/FB03/FB03b";
            header_name_c = header_name_c + "/FB03/FB03c";
            Facebook_3(char_name);
            combineImage_3(output_image_name, readHeader_a_3(header_name_a),readHeader_b_3(header_name_b), readHeader_c_3(header_name_c), findFFDA_3(original_image_name));
            MD5_rename();
            std::remove(temp_name_char);
            break;
        case 4:
            header_name_a = header_name_a + "/FB04/FB04a";
            Facebook_4(char_name);
            combineImage_4(output_image_name, readHeader_a_4(header_name_a),saveQT_4(original_image_name));
            MD5_rename();
            std::remove(temp_name_char);
            break;
    }
    return 0;
}
