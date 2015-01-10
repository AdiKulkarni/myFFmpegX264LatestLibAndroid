/* File : frameEncoder.i */
%module frameEncoderInterface

%{
#include "frameEncoder.h"
%}

%include "typemaps.i"
%include "various.i"
%include "arrays_java.i"

%apply char *BYTE {char*}

%apply int *OUTPUT { int *outFrameSize };
extern encodeFrame(char* inBytes, int frameSize, int pts, char* outBytes, int *outFrameSize);

/* Let's just grab the original header file here */
%include "frameEncoder.h"
