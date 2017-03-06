//  Created by National Center for Media Forensics
//  University of Colorado Denver
//  ImageLaunder v.1 10/28/2016
//
//  Facebook4.h

#ifndef Facebook4_h
#define Facebook4_h
#include "Header.h"

void Facebook_4(Image originalImage,int quality_number){
    Image tempImage = originalImage;
    /**** resizing ****/
    int height = tempImage.rows(); //
    int width = tempImage.columns();
    if( height > 2048 || width > 2048){
        tempImage.resize("2048");
        height = tempImage.rows();
        width = tempImage.columns();
        
    }
    
    resolution(height, width);
    
    tempImage.strip();
    tempImage.defineValue("jpeg","sampling-factor","2x2");
    
    /**** DCT ****/
    tempImage.interlaceType(PlaneInterlace);
    
    /**** Compressing ****/
    tempImage.quality(quality_number);
    
    /**** Save processed image ****/
    tempImage.write(input_image_name);
    
}

int QT4 (string filename){
    char * QTbuffer; // store quantization table to buffer
    char * imagebuffer; // store total image in buffer
    ifstream image (filename, ios::binary | ios::ate);
    QTsize = 0; // size of quantiation table
    int imageLength = 0;  //size if total image
    string previousLine = " ";
    string followingLine = " ";
    string ffc4mark1 = " ";
    string ffc4mark2 = " ";
    int start_index = 0;
    
    if (image) {
        imageLength = image.tellg();
        image.seekg (0, image.beg);
        imagebuffer = new char [imageLength];
        memset(imagebuffer, 0, sizeof(char)*imageLength);
        image.read (imagebuffer,imageLength);
        
        for(int i = 0; i < imageLength; i++){
            followingLine = int_to_hex(static_cast<int16_t>(imagebuffer[i]));
            if (followingLine == "ffdb"){
                if(previousLine == "ffff"){
                    ffc4mark1 = int_to_hex(static_cast<int16_t>(imagebuffer[i + 137]));
                    ffc4mark2 = int_to_hex(static_cast<int16_t>(imagebuffer[i + 138]));
                    if(ffc4mark1 == "ffff" && ffc4mark2 == "ffc2"){
                        start_index = i - 1;
                    }
                    break;
                }
            }
            else
                previousLine = followingLine;
        }
    }
    delete[] imagebuffer;
    image.close();
    return start_index;
}

char * saveQT_4(string filename){
    ifstream image (filename, ios::binary | ios::ate);
    char * storeBuffer = nullptr; // store FFDA to the end of image in buffer
    char * imagebuffer; // store total image in buffer
    storeSize = 0; // size of FFDA to the end
    int imageLength = 0;  //size if total image
    
    int start_index = QT4(filename);
    
    if (image) {
        imageLength = image.tellg();
        image.seekg (0, image.beg);
        imagebuffer = new char [imageLength];
        memset(imagebuffer, 0, sizeof(char)*imageLength);
        image.read (imagebuffer,imageLength);
        
        
        storeSize = imageLength - start_index;
        storeBuffer = new char[storeSize];
        memset(storeBuffer, 0, sizeof(char) * storeSize);
        
        int m = 0;
        for(int i = start_index; i<= imageLength; i++){
            storeBuffer[m] = imagebuffer[i];
            m++;
        }
        
        
    }
    return storeBuffer;
}

char * readHeader_a_4(string filename){
    ifstream header (filename, ios::binary | ios::ate);
    headerSize_a = 0;
    char *headerBuffer = nullptr;
    if(header){
        headerSize_a = header.tellg();
        header.seekg (0, header.beg);
        headerBuffer = new char [headerSize_a + 1];
        memset(headerBuffer, 0, sizeof(char)*headerSize_a);
        header.read (headerBuffer,headerSize_a);
    }
    else
        cout << "error to open header A" << endl << endl;
    
    for(int i = 597; i < 617; i++)
    {
        headerBuffer[i] = genRandom();
    }
    return headerBuffer;
}


void combineImage_4(string filename, char * headerBuffer,char * storeBuffer){
    
    ofstream header_a_result (filename, ios::binary);
    if(header_a_result){
        header_a_result.write (headerBuffer,headerSize_a);
    }
    else
        cout << "error to open result image" << endl;
    header_a_result.close();
    
    ofstream result (filename, ios::binary | ios::app);
    if(result){
        result.write(storeBuffer,storeSize);
    }
    else
        cout << "error to open result image" << endl;
    result.close();
}

#endif /* Facebook4_h */
