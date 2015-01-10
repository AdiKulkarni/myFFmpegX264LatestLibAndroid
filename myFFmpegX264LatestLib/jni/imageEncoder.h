#ifndef IMAGE_ENCODER_H
#define IMAGE_ENCODER_H

#include <inttypes.h>
#include <stdio.h>
#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <cstring>
#include <fstream>

extern "C" {
#include <x264/x264.h>
#include <android/log.h>
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavutil/audioconvert.h>
#include <libavutil/common.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>
#include <libswscale/swscale.h>
}

class imageEncoder {

private:

        AVCodec *codec;
        AVCodecContext *codecCtx;
        uint8_t *output_buffer;
        int output_buffer_size;
        struct SwsContext* fooContext;

public:
        imageEncoder();
        ~imageEncoder();
        int initImageEncoder();
        void encodeImage(char* inImage, int inImageSize, int counter, char* outImage, int *outImageSize);
        void closeImageEncoder();
        uint8_t* getOutputBuffer();
        void setOutputBuffer(uint8_t* outputBuffer);
        int getOutputBufferSize();
        void setOutputBufferSize(int outputBufferSize);

};
#endif
