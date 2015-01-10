/* File : exampleEncoder.i */
%module exampleEncoderInterface
%include "various.i"
%{
#include "exampleEncoder.h"
%}
%include "typemaps.i"
%include "various.i"
%include "arrays_java.i"

%apply char *BYTE {char*}

%apply int *OUTPUT { int *outFrameSize };
extern video_encode(char* inBytes, int frameSize, int counter, char* outBytes, double *outFrameSize);

/* Let's just grab the original header file here */
%include "exampleEncoder.h"