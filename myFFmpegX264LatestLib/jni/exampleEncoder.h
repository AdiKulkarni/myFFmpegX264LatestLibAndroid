//frameEncoder

#ifndef X264_EXAMPLE_ENCODER_H
#define X264_EXAMPLE_ENCODER_H

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

class exampleEncoder {

private:

        AVCodec *codec;
        AVCodecContext *codecContext;
        int ret, got_output, i_width, i_height, i_bitrate, i_fps, i_gopSize, i_maxBframes;
        FILE *f;
        AVFrame *frame;
        AVPacket pkt;
        const char *filename;

public:
        exampleEncoder();
        void initialize();
        void video_encode(char* inBytes, int frameSize, int counter, char* outBytes, int *outFrameSize);
        void close();
        int getBitrate();
        void setBitrate(int bitrate);
        int getFps();
        void setFps(int fps);
        int getGopSize();
        void setGopSize(int gopSize);
        int getHeight();
        void setHeight(int height);
        int getMaxBframes();
        void setMaxBframes(int maxBframes);
        int getWidth();
        void setWidth(int width);

};

#endif
