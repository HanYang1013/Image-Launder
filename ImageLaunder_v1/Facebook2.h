//  Created by National Center for Media Forensics
//  University of Colorado Denver
//  ImageLaunder v.1 10/28/2016
//
//  Facebook2.h

#ifndef Facebook2_h
#define Facebook2_h
#include "Header.h"

void Facebook_2(Image originalImage,int quality_number){
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
    tempImage.interlaceType(NoInterlace);
    
    /**** Compressing ****/
    tempImage.quality(quality_number);
    
    /**** Save processed image ****/
    tempImage.write(input_image_name);
}

char * findFFDA_2(string filename){
    ifstream image (filename, ios::binary | ios::ate);
    char * storeBuffer; // store FFDA to the end of image in buffer
    char * imagebuffer; // store total image in buffer
    storeSize = 0; // size of FFDA to the end
    int imageLength = 0;  //size if total image
    
    string previousLine = " ";
    string followingLine = " ";
    int start_index = 0;
    
    if (image) {
        // get length of file:
        imageLength = image.tellg();
        image.seekg (0, image.beg);
        
        // allocate memory:
        imagebuffer = new char [imageLength];
        memset(imagebuffer, 0, sizeof(char)*imageLength);
        // read data as a block:
        image.read (imagebuffer,imageLength);
        
        for(int i = 0; i < imageLength; i++){
            followingLine = int_to_hex(static_cast<int16_t>(imagebuffer[i]));
            if (followingLine == "ffda"){
                if(previousLine == "ffff"){
                    start_index = i + 10 ;
                    storeSize = imageLength - start_index;
                    storeBuffer = new char[imageLength - start_index];
                    memset(storeBuffer, 0, sizeof(char)*(imageLength - start_index));
                    int m = 0;
                    for(int j = start_index; j < imageLength; j++)
                    {
                        storeBuffer[m] = imagebuffer[j];
                        m++;
                    }
                    break;
                }
            }
            else
                previousLine = followingLine;
            
        }
        
        image.close();
        delete[] imagebuffer;
        
    }
    else
        cout<<"image open error";
    
    return storeBuffer;
}

char * readHeader_a_2(string filename){
    fstream header (filename, ios::binary | ios::ate | ios::in | ios::out);
    headerSize_a = 0;
    char *headerBuffer = nullptr;
    char *timeBuffer = new char [19];
    if(header){
        headerSize_a = header.tellg();
        header.seekg (0, header.beg);
        headerBuffer = new char [headerSize_a + 1];
        memset(headerBuffer, 0, sizeof(char)*headerSize_a);
        header.read (headerBuffer,headerSize_a);
    }
    else
        cout << "error to open header A" << endl << endl;
    
    timeBuffer = randomTime();
    int timeCount = 0;
    for(int i = 156; i < 175; i++)
    {
        headerBuffer[i] = timeBuffer[timeCount];
        timeCount = timeCount + 1;
    }
    
    timeCount = 0;
    for(int i = 290; i < 309; i++)
    {
        headerBuffer[i] = timeBuffer[timeCount];
        timeCount = timeCount + 1;
    }
    
    timeCount = 0;
    for(int i = 310; i < 329; i++)
    {
        headerBuffer[i] = timeBuffer[timeCount];
        timeCount = timeCount + 1;
    }
    
    timeBuffer = randomTimeT();
    timeCount = 0;
    for(int i = 685; i < 704; i++)
    {
        headerBuffer[i] = timeBuffer[timeCount];
        timeCount = timeCount + 1;
    }
    
    timeCount = 0;
    for(int i = 722; i < 741; i++)
    {
        headerBuffer[i] = timeBuffer[timeCount];
        timeCount = timeCount + 1;
    }
    
    timeCount = 0;
    for(int i = 795; i < 814; i++)
    {
        headerBuffer[i] = timeBuffer[timeCount];
        timeCount = timeCount + 1;
    }
    
    raw_time = 0;
    
    int resolutionCount = 0;
    for(int i = 2973; i < 2977; i++)
    {
        headerBuffer[i] = resolutionBuffer[resolutionCount];
        resolutionCount = resolutionCount + 1;
    }
    return headerBuffer;
}

char * readHeader_b_2(string filename){
    fstream header_b (filename, ios::binary | ios::ate | ios::in);
    headerSize_b = 0;
    char *headerBuffer_b = nullptr;
    if(header_b){
        headerSize_b = header_b.tellg();
        header_b.seekg (0, header_b.beg);
        headerBuffer_b = new char [headerSize_b + 1];
        memset(headerBuffer_b, 0, sizeof(char)*headerSize_b);
        header_b.read (headerBuffer_b,headerSize_b);
    }
    else
        cout << "error to open header B" << endl << endl;
    
    return headerBuffer_b;
}

char * HT2 (string filename){
    char * HTbuffer; // store huffmantable to in buffer
    char * imagebuffer = nullptr; // store total image in buffer
    ifstream image (filename, ios::binary | ios::ate);
    HTsize = 0; // size of huffman table
    int imageLength = 0;  //size if total image
    string previousLine = " ";
    string followingLine = " ";
    int current_index[10] = {0};
    int current_index_number = 0;
    int start_index = 0;
    int end_index = 0;
    
    if (image) {
        imageLength = image.tellg();
        image.seekg (0, image.beg);
        imagebuffer = new char [imageLength];
        memset(imagebuffer, 0, sizeof(char)*imageLength);
        image.read (imagebuffer,imageLength);
        
        for(int i = 0; i < imageLength; i++){
            followingLine = int_to_hex(static_cast<int16_t>(imagebuffer[i]));
            if (followingLine == "ffc4"){
                if(previousLine == "ffff"){
                    current_index[current_index_number] = i-1;
                    current_index_number ++;
                }
            }
            else
                previousLine = followingLine;
        }
        
        for(int i = current_index[3]; i<imageLength; i++){
            followingLine = int_to_hex(static_cast<int16_t>(imagebuffer[i+1]));
            if(followingLine == "ffff"){
                end_index = i+1;
                break;
            }
        }
        
        start_index = current_index[0];
        HTsize = end_index - start_index;
        HTbuffer = new char [HTsize];
        memset(HTbuffer, 0, sizeof(char)*HTsize);
        int m = 0;
        
        for(int i = start_index; i < end_index; i++){
            HTbuffer[m] = imagebuffer[i];
            m++;
        }
        
    }
    delete[] imagebuffer;
    image.close();
    return HTbuffer;
}

char * QT2 (string filename){
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
                    if(ffc4mark1 == "ffff" && ffc4mark2 == "ffc0"){
                        start_index = i - 1;
                    }
                }
            }
            else
                previousLine = followingLine;
        }
        QTsize = 138;
        QTbuffer = new char [QTsize];
        for(int i = 0; i<= QTsize; i++){
            QTbuffer[i] = imagebuffer[start_index];
            start_index ++;
        }
        
    }
    delete[] imagebuffer;
    image.close();
    return QTbuffer;
    
}


void  combineImage_2(string filename, char * headerBuffer,char *headerBuffer_b, char * storeBuffer){
    char * QTbuffer = new char [QTsize];
    char * HTbuffer = new char [HTsize];
    
    QTbuffer = QT2(input_image_name);
    HTbuffer = HT2(input_image_name);
    
    ofstream header_a_result (filename, ios::binary);
    if(header_a_result){
        header_a_result.write (headerBuffer,headerSize_a);
    }
    else
        cout << "error to open result image" << endl;
    header_a_result.close();
    
    ofstream HTresult (filename, ios::binary | ios::app);
    if(HTresult){
        HTresult.write(HTbuffer,HTsize);
    }
    else
        cout << "error to open result image" << endl;
    HTresult.close();
    
    ofstream QTresult (filename, ios::binary | ios::app);
    if(QTresult){
        QTresult.write(QTbuffer,QTsize);
    }
    else
        cout << "error to open result image" << endl;
    QTresult.close();
    
    ofstream header_b_result (filename, ios::binary | ios::app);
    if(header_b_result){
        header_b_result.write(headerBuffer_b,headerSize_b);
    }
    else
        cout << "error to open result image" << endl;
    header_b_result.close();
    
    ofstream result (filename, ios::binary | ios::app);
    if(result){
        result.write(storeBuffer,storeSize);
    }
    else
        cout << "error to open result image" << endl;
    result.close();
}


#endif /* Facebook2_h */
