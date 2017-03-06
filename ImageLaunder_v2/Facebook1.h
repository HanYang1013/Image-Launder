//  Created by National Center for Media Forensics
//  University of Colorado Denver
//  ImageLaunder v.1 10/28/2016
//
//  Facebook1.h

#ifndef Facebook1_h
#define Facebook1_h
#include "Header.h"

static const unsigned int fb01_luminance_quant_tbl[DCTSIZE2] = {
    9,6, 6, 9, 14, 23, 30, 35,
    7, 7, 8, 11, 15, 34, 35, 32,
    8, 8, 9, 14, 23, 33, 40, 32,
    8, 10, 13, 17, 30, 50, 46, 36,
    10, 13, 21, 32, 39, 63, 60, 45,
    14, 20, 32, 37, 47, 60, 66, 53,
    28, 37, 45, 50, 60, 70, 70, 59,
    42, 53, 55, 57, 65, 58, 60, 57
};
static const unsigned int fb01_chrominance_quant_tbl[DCTSIZE2] = {
    10, 10, 14, 27, 57, 57, 57, 57,
    10, 12, 15, 38, 57, 57, 57, 57,
    14, 15, 32, 57, 57, 57, 57, 57,
    27, 38, 57, 57, 57, 57, 57, 57,
    57, 57, 57, 57, 57, 57, 57, 57,
    57, 57, 57, 57, 57, 57, 57, 57,
    57, 57, 57, 57, 57, 57, 57, 57,
    57, 57, 57, 57, 57, 57, 57, 57
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
    
    resolution(height, width);
    int scale_factor = 100;
    bool force_baseline = TRUE;
    
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

char * findFFDA_1(string filename){
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

char * HT1 (string filename){
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

char * QT1 (string filename){
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
                    break;
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



char * readHeader_a_1(string filename){
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
        cout << "error to open headerA" << endl << endl;
    
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

char * readHeader_b_1(string filename){
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
        cout << "error to open headerB" << endl << endl;
    
    return headerBuffer_b;
}

void combineImage_1(string filename, char * headerBuffer, char *headerBuffer_b, char * storeBuffer){
    
    char * QTbuffer = new char [QTsize];
    char * HTbuffer = new char [HTsize];
    
    QTbuffer = QT1(original_image_name);
    HTbuffer = HT1(original_image_name);
    
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
    
    delete[] resolutionBuffer;
}

#endif /* Facebook1_h */
