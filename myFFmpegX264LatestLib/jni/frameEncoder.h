//frameEncoder

#ifndef X264_FRAME_ENCODER_H
#define X264_FRAME_ENCODER_H

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

#define INBUF_SIZE 4096
#define AUDIO_INBUF_SIZE 20480
#define AUDIO_REFILL_THRESH 4096

class frameEncoder {

private:
            /* params the user should set */
            int in_width;
            int in_height;
            int out_width;
            int out_height;
            int fps; /* e.g. 25, 60, etc.. */
            int bitrate;
            int i_threads;
            int i_vbv_buffer_size;
            int i_slice_max_size;
            int b_frame_frq;

            /*Picture*/
            AVPixelFormat in_pixel_format;
            AVPixelFormat out_pixel_format;

            /* x264 */
            AVPicture pic_raw; /* used for our "raw" input container */
            x264_picture_t pic_in;
            x264_picture_t pic_out;
            x264_param_t params;
            x264_nal_t* nals;
            x264_t* encoder;
            int num_nals;

            /* input / output */
            struct SwsContext* sws;
            //FILE* pFile;
public:
            frameEncoder();
            ~frameEncoder();
            bool open(); /* open for encoding */
            void encodeFrame(char* inBytes, int frameSize, int pts, char* outBytes, int *outFrameSize); /* encode the given data */
            bool close(); /* close the encoder and file, frees all memory */
            bool validateSettings(); /* validates if all params are set correctly, like width,height, etc.. */
            void setParams(); /* sets the x264 params */
            int getFps() const;
            void setFps(int fps);
            int getInHeight() const;
            void setInHeight(int inHeight);
            int getInWidth() const;
            void setInWidth(int inWidth);
            int getNumNals() const;
            void setNumNals(int numNals);
            int getOutHeight() const;
            void setOutHeight(int outHeight);
            int getOutWidth() const;
            void setOutWidth(int outWidth);
            int getBitrate() const;
            void setBitrate(int bitrate);
            int getSliceMaxSize() const;
            void setSliceMaxSize(int sliceMaxSize);
            int getVbvBufferSize() const;
            void setVbvBufferSize(int vbvBufferSize);
            int getIThreads() const;
            void setIThreads(int threads);
            int getBFrameFrq() const;
            void setBFrameFrq(int frameFrq);

};

#endif
