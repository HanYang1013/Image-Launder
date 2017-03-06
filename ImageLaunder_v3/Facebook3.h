//  Created by National Center for Media Forensics
//  University of Colorado Denver
//  ImageLaunder v.1 10/28/2016
//
//  Facebook3.h

#ifndef Facebook3_h
#define Facebook3_h
#include "Header.h"

static const unsigned int fb03_luminance_quant_tbl[DCTSIZE2] = {
    9, 9, 9, 16, 22, 30, 38, 46,
    9, 9, 9, 16, 22, 30, 38, 46,
    9, 9, 16, 22, 30, 38, 46, 55,
    16, 16, 22, 30, 38, 46, 55, 64,
    22, 22, 30, 38, 46, 55, 64, 72,
    30, 30, 38, 46, 55, 64, 72, 72,
    38, 38, 46, 55, 64, 72, 72, 72,
    46, 46, 55, 64, 72, 72, 72, 72
};
static const unsigned int fb03_chrominance_quant_tbl[DCTSIZE2] = {
    11, 12, 18, 31, 75, 75, 75, 75,
    12, 17, 17, 51, 75, 75, 75, 75,
    18, 17, 42, 75, 75, 75, 75, 75,
    31, 51, 75, 75, 75, 75, 75, 75,
    75, 75, 75, 75, 75, 75, 75, 75,
    75, 75, 75, 75, 75, 75, 75, 75,
    75, 75, 75, 75, 75, 75, 75, 75,
    75, 75, 75, 75, 75, 75, 75, 75
};

void resize3(const char * inputfilename){
    Image originalImage(inputfilename);
    int height = originalImage.rows(); //
    int width = originalImage.columns();
    if( height > 1440 || width > 1440){
        originalImage.resize("1440");
        originalImage.quality(100);
        originalImage.write(inputfilename);
    }
}

void Facebook_3( const char * inputfilename){
    resize3(inputfilename);
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

    jpeg_simple_progression(&cinfo);
    int scale_factor = 100;
    bool force_baseline = FALSE;
    
    jpeg_add_quant_table(&cinfo, 0, fb03_luminance_quant_tbl, scale_factor, force_baseline);
    jpeg_add_quant_table(&cinfo, 1, fb03_chrominance_quant_tbl, scale_factor, force_baseline);
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

int QT3 (string filename){
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

char * saveQT_3(string filename){
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

char * readHeader_3(string filename){
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
    
    return headerBuffer;
}


void combineImage_3(string filename, char * headerBuffer,char * storeBuffer){
    
    ofstream header_result (filename, ios::binary);
    if(header_result){
        header_result.write (headerBuffer,headerSize);
    }
    else
        cout << "error to open result image" << endl;
    header_result.close();
    
    ofstream result (filename, ios::binary | ios::app);
    if(result){
        result.write(storeBuffer,storeSize);
    }
    else
        cout << "error to open result image" << endl;
    result.close();
}



#endif /* Facebook3_h */
