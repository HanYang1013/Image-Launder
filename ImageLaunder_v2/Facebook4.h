//  Created by National Center for Media Forensics
//  University of Colorado Denver
//  ImageLaunder v.1 10/28/2016
//
//  Facebook4.h

#ifndef Facebook4_h
#define Facebook4_h
#include "Header.h"

static const unsigned int fb04_luminance_quant_tbl[DCTSIZE2] = {
    5, 3, 3, 5, 7, 12, 15, 18,
    4, 4, 4, 6, 8, 17, 18, 17,
    4, 4, 5, 7, 12, 17, 21, 17,
    4, 5, 7, 9, 15, 26, 24, 19,
    5, 7, 11, 17, 20, 33, 31, 23,
    7, 11, 17, 19, 24, 31, 34, 28,
    15, 19, 23, 26, 31, 36, 36, 30,
    22, 28, 29, 29, 34, 30, 31, 30
};
static const unsigned int fb04_chrominance_quant_tbl[DCTSIZE2] = {
    5, 5, 7, 14, 30, 30, 30, 30,
    5, 6, 8, 20, 30, 30, 30, 30,
    7, 8, 17, 30, 30, 30, 30, 30,
    14, 20, 30, 30, 30, 30, 30, 30,
    30, 30, 30, 30, 30, 30, 30, 30,
    30, 30, 30, 30, 30, 30, 30, 30,
    30, 30, 30, 30, 30, 30, 30, 30,
    30, 30, 30, 30, 30, 30, 30, 30
};
void resize4(const char * inputfilename){
    Image originalImage(inputfilename);
    int height = originalImage.rows(); //
    int width = originalImage.columns();
    if( height > 2048 || width > 2048){
        originalImage.resize("2048");
        originalImage.quality(100);
        originalImage.write(inputfilename);
    }
}


void Facebook_4( const char * inputfilename){
    resize4(inputfilename);
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
    jpeg_simple_progression(&cinfo);
    int scale_factor = 100;
    bool force_baseline = FALSE;
    
    jpeg_add_quant_table(&cinfo, 0, fb04_luminance_quant_tbl, scale_factor, force_baseline);
    jpeg_add_quant_table(&cinfo, 1, fb04_chrominance_quant_tbl, scale_factor, force_baseline);
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
