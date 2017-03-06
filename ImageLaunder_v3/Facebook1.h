//  Created by National Center for Media Forensics
//  University of Colorado Denver
//  ImageLaunder v.1 10/28/2016
//
//  Facebook1.h

#ifndef Facebook1_h
#define Facebook1_h
#include "Header.h"

static const unsigned int fb01_luminance_quant_tbl[DCTSIZE2] = {
    11, 8, 7, 11, 17, 28, 36, 43,
    8, 8, 10, 13, 18, 41, 42, 39,
    10, 9, 11, 17, 28, 40, 48, 39,
    10, 12, 15, 20, 36, 61, 56, 43,
    13, 15, 26, 39, 48, 76, 72, 54,
    17, 25, 39, 45, 57, 73, 79, 64,
    34, 45, 55, 61, 72, 85, 84, 71,
    50, 64, 67, 69, 78, 70, 72, 69
};
static const unsigned int fb01_chrominance_quant_tbl[DCTSIZE2] = {
    12, 13, 17, 33, 69, 69, 69, 69,
    13, 15, 18, 46, 69, 69, 69, 69,
    17, 18, 39, 69, 69, 69, 69, 69,
    33, 46, 69, 69, 69, 69, 69, 69,
    69, 69, 69, 69, 69, 69, 69, 69,
    69, 69, 69, 69, 69, 69, 69, 69,
    69, 69, 69, 69, 69, 69, 69, 69,
    69, 69, 69, 69, 69, 69, 69, 69
};

void resize1(const char * inputfilename){
    Image originalImage(inputfilename);
    int height = originalImage.rows(); //
    int width = originalImage.columns();
    if( height > 1440 || width > 1440){
        originalImage.resize("1440");
        originalImage.quality(100);
        originalImage.write(inputfilename);
    }
}
void Facebook_1(const char * inputfilename){
    resize1(inputfilename);
    FILE *original,*copy;
    const char *input_file = inputfilename;
    const char *output_file = inputfilename;
    
    struct jpeg_decompress_struct dcinfo;
    struct jpeg_error_mgr dcjerr;
    unsigned char *raw_image;
    unsigned char **row;
    int i,width,height,bytes_per_pixel;
    
    original = fopen(input_file,"rb");
    if(!original)
    {
        fprintf(stderr,"Unable to read image file.\n");
        exit(1);
    }
    
    dcinfo.err = jpeg_std_error(&dcjerr);
    jpeg_create_decompress(&dcinfo);
    jpeg_stdio_src(&dcinfo,original);
    jpeg_read_header(&dcinfo,TRUE);
    jpeg_start_decompress(&dcinfo);
    
    width = dcinfo.output_width;
    height = dcinfo.output_height;
    bytes_per_pixel = dcinfo.output_components;
    
    /* Create buffers for the uncompressed data */
    raw_image = (unsigned char *)malloc(bytes_per_pixel*width*height);
    row = (unsigned char **)malloc(sizeof(unsigned char *)*height);
    for(i=0;i<height;i++)
        row[i] = raw_image+(i*width*bytes_per_pixel);
    
    /* loop to read each scanline */
    while(dcinfo.output_scanline < height)
    {
        jpeg_read_scanlines(&dcinfo,&row[dcinfo.output_scanline],1);
    }
    
    /* raw image is stored in the buffer raw_image */
    /* cleanup and close */
    jpeg_finish_decompress(&dcinfo);
    jpeg_destroy_decompress(&dcinfo);
    fclose(original);
    
    /* Compress & Save */
    /* Raw data remains in the raw_image buffer. */
    
    copy = fopen(output_file,"wb");
    if(!copy)
    {
        fprintf(stderr,"Unable to create output image file.\n");
        exit(1);
    }
    
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr cjerr;
    cinfo.err = jpeg_std_error(&cjerr);
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo,copy);
    
    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;
    jpeg_set_defaults(&cinfo);
    
    int scale_factor = 100;
    bool force_baseline = false;
    
    jpeg_add_quant_table(&cinfo, 0, fb01_luminance_quant_tbl, scale_factor, force_baseline);
    jpeg_add_quant_table(&cinfo, 1, fb01_chrominance_quant_tbl, scale_factor, force_baseline);
    /* Begin compression */
    jpeg_start_compress(&cinfo,TRUE);
    /* write one scanline at a time */
    while(cinfo.next_scanline < cinfo.image_height)
    {
        jpeg_write_scanlines(&cinfo,&row[cinfo.next_scanline],1);
    }
    
    /* clean-up */
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    fclose(copy);
    
}

int QT1 (string filename){
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

char * saveQT_1(string filename){
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

void FBMD_Generate_1(){
    srand((unsigned)time(0));
    finalBuffer = new char[64];
    int ran[8];
    ran[0] = (rand() % 6 + 1) + 906;    //1-5
    ran[1] = (rand() % 17553) + 4098 ; // 17553 is 21651 - 4098
    ran[2] = (rand() % (57594 - ran[1])) + ran[1] ;
    ran[3] = (rand() % (60321 - ran[2])) + ran[2] ;
    ran[4] = (rand() % (62668 - ran[3])) + ran[3] ;
    ran[5] = (rand() % (83546 - ran[4])) + ran[4] ;
    ran[6] = (rand() % (138698 - ran[5])) + ran[5] ;
    ran[7] = (rand() % (140521 - ran[6])) + ran[6] ;
    
    int count = 0;
    int resultcount = 0;
    
    for(int n = 0; n < 8; n++){
        count = 0;
        string ranString = int_to_hex(ran[n]);
        
        char * ranbuffer = new char[ranString.length() + 1];
        strcpy(ranbuffer, ranString.c_str());
        
        char * resultbuffer= new char[8];
        for(int j = 0; j< 8; j++){
            resultbuffer[j] = '0';
        }
        
        for(int j = 8 - ranString.length(); j < 8; j++){
            resultbuffer[j] = ranbuffer[count];
            count ++;
        }
        char * randomResult = new char[8];
        for(int j = 0; j< 8; j++){
            randomResult[j] = '0';
            
        }
        
        randomResult[0] = resultbuffer[6];
        randomResult[1] = resultbuffer[7];
        randomResult[2] = resultbuffer[4];
        randomResult[3] = resultbuffer[5];
        randomResult[4] = resultbuffer[2];
        randomResult[5] = resultbuffer[3];
        randomResult[6] = resultbuffer[1];
        randomResult[7] = resultbuffer[2];
        
        for(int j = 0; j< 8; j++){
            finalBuffer[resultcount] = randomResult[j];
            resultcount++;
        }
        
        delete[] randomResult;
        delete[] resultbuffer;
        delete[] ranbuffer;
    }

}


char * readHeader_1(string filename){
    ifstream header (filename, ios::binary | ios::ate);
    headerSize = 0;
    char *headerBuffer = nullptr;
    if(header){
        headerSize = header.tellg();
        header.seekg (0, header.beg);
        headerBuffer = new char [headerSize + 1];
        memset(headerBuffer, 0, sizeof(char)*headerSize);
        header.read (headerBuffer,headerSize);
    }
    else
        cout << "error to open header A" << endl << endl;
    
    for(int i = 597; i < 617; i++)
    {
        headerBuffer[i] = genRandom();
    }
    
    FBMD_Generate_1();
    int count = 0;
    for(int i = 90; i < 177; i++)
    {
        headerBuffer[i] = finalBuffer[count];
        count ++;
    }
    
    return headerBuffer;
    
}

void combineImage_1(string filename, char * headerBuffer,char * storeBuffer){
    
    ofstream header_a_result (filename, ios::binary);
    if(header_a_result){
        header_a_result.write (headerBuffer,headerSize);
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

#endif /* Facebook1_h */
