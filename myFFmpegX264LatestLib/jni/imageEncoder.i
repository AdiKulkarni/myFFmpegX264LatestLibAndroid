/* File : imageEncoder.i */
%module imageEncoderInterface

%{
#include "imageEncoder.h"
%}

%include "typemaps.i"
%include "various.i"
%include "arrays_java.i"

%apply char *BYTE {char*}

%apply int *OUTPUT { int *outImageSize };
extern encodeImage(char* inImage, int inImageSize, int counter, char* outImage, int *outImageSize);

/* Let's just grab the original header file here */
%include "imageEncoder.h"
