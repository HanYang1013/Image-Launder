ImageLaunder v.1 10/28/2016

*** Filelist:
main.cpp
Facebook1.h	
Facebook2.h	
Facebook3.h	
Facebook4.h	
Header.h		
MD5.h		
resources/FB01a	
resources/FB01b	
resources/FB02a	
resources/FB02b	
resources/FB03a	
resources/FB03b	
resources/FB03c	
resources/FB04a

*** Description:
This software is designed to take an input image file, recompress the image data, and package with rebuilt file header into a new .jpg file using the new file’s MD5 hash checksum as its filename. 

The criteria for compression and header structure is based on characteristics from Facebook usage in the following 4 scenarios:
1. Upload from a mobile device and download to a mobile device
2. Upload from a desktop/laptop computer and download and download to a mobile device
3. Upload from a mobile device and download do a desktop/laptop computer
4. Upload from a desktop/laptop computer and download to a desktop/laptop computer

*** Requirements:
ImageMagick 7.0.3-5 Q16 x86_64 2016-10-15 
	Available from: http://www.imagemagick.org/script/binary-releases.php
	Copyright: Copyright (C) 1999-2016 ImageMagick Studio LLC
	License: http://www.imagemagick.org/script/license.php

*** Usage:
The program can be compiled directly in the command line using the following flags: `Magick++-config --cxxflags --cppflags --ldflags --libs`

Program takes the following input Arguments:
arg [1]. image file, adhering to many types with a complete list here: http://www.imagemagick.org/script/formats.php
arg [2]. an integer number equal to or greater than 1 and less than or equal to 4. This is the choice of 4 scenarios above.

*** To do for v.2
Remaining challenges to solve include: 
quantization of DCT using defined chrominance and luminance quantization tables
fixing orientation based on input image’s orientation
writing compressed jpeg file with defined restart intervals (DRI) and restart interval markers (RST).

