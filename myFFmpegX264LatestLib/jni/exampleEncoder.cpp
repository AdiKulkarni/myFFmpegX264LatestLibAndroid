#include "exampleEncoder.h"

#define LOG_LEVEL 1
#define LOG_TAG "exampleEncoder.cpp"
#define LOGI(level, ...) if (level <= LOG_LEVEL) {__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__);}
#define LOGE(level, ...) if (level <= LOG_LEVEL) {__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__);}

exampleEncoder::exampleEncoder() {
    //default constructor
    setBitrate(400000);
    setFps(15);
    setGopSize(10);
    setHeight(640);
    setWidth(480);
    setMaxBframes(0);

}

void exampleEncoder::initialize() {

    avcodec_register_all();
    //filename = "/sdcard/file";

    /* find the mpeg1 video encoder */
    codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!codec) {
        LOGI(LOG_LEVEL, "Codec not found\n");
        exit(1);
    }

    codecContext = avcodec_alloc_context3(codec);
    if (!codecContext) {
        LOGI(LOG_LEVEL, "Could not allocate video codec context\n");
        exit(1);
    }

    /* put sample parameters */
    codecContext->bit_rate = i_bitrate;
    /* resolution must be a multiple of two */
    codecContext->width = i_width;
    codecContext->height = i_height;
    /* frames per second */
    codecContext->time_base = (AVRational ) { 1, i_fps };
    /* emit one intra frame every ten frames
     * check frame pict_type before passing frame
     * to encoder, if frame->pict_type is AV_PICTURE_TYPE_I
     * then gop_size is ignored and the output of encoder
     * will always be I frame irrespective to gop_size
     */
    codecContext->framerate = (AVRational ) { 1, i_fps };
    codecContext->gop_size = i_gopSize;
    codecContext->max_b_frames = i_maxBframes;
    codecContext->pix_fmt = AV_PIX_FMT_YUV420P;
    //codecContext->ticks_per_frame = 0;
    //codecContext->delay = 0;
    //codecContext->b_quant_offset = 0.0;
    //codecContext->refs = 0;
    codecContext->slices = 1;
    //codecContext->has_b_frames = 0;
    codecContext->thread_count = 2;

    av_opt_set(codecContext->priv_data, "zerolatency", "ultrafast", 0);

    /* open it */
    if (avcodec_open2(codecContext, codec, NULL) < 0) {
        LOGI(LOG_LEVEL, "Could not open codec\n");
        exit(1);
    }

    //f = fopen(filename, "wb");
    //if (!f) {
    //   LOGI(LOG_LEVEL, "Could not open %s\n", filename);
    //    exit(1);
    // }

    frame = av_frame_alloc();
    if (!frame) {
        LOGI(LOG_LEVEL, "Could not allocate video frame\n");
        exit(1);
    }

    frame->format = codecContext->pix_fmt;
    frame->width = codecContext->width;
    frame->height = codecContext->height;

    /* the image can be allocated by any means and av_image_alloc() is
     * just the most convenient way if av_malloc() is to be used */
    ret = av_image_alloc(frame->data, frame->linesize, codecContext->width, codecContext->height,
            codecContext->pix_fmt, 32);
    if (ret < 0) {
        LOGI(LOG_LEVEL, "Could not allocate raw picture buffer\n");
        exit(1);
    }
}

/*
 * Video encoding
 */
void exampleEncoder::video_encode(char* inBytes, int frameSize, int counter,
        char* outBytes, int *outFrameSize) {

    /* encode 1 frame of video */
    av_init_packet(&pkt);
    pkt.data = NULL; // packet data will be allocated by the encoder
    pkt.size = 0;

    // Copy data from the 3 input buffers (YV12 is I420 but U and V reversed)
    //Y
    frame->data[0] = (uint8_t*) inBytes;
    //U or Cb
    frame->data[2] = (uint8_t*) inBytes + frame->width * frame->height;
    //V or Cr
    frame->data[1] = (uint8_t*) inBytes + frame->width * frame->height
            + frame->width * frame->height / 4;

    frame->pts = counter;

    /* encode the image */
    ret = avcodec_encode_video2(codecContext, &pkt, frame, &got_output);

    if (ret < 0) {
        LOGI(LOG_LEVEL, "Error encoding frame\n");
        exit(1);
    }

    if (got_output) {
        //fwrite(pkt.data, 1, pkt.size, f);
        //LOGI(LOG_LEVEL, "Write frame %3d (size=%5d)\n", counter, pkt.size);

        /*Here first four bytes in the nal indicates packet length*/
        //unsigned char bytes[4];
        //bytes[0] = ((int) pkt.size >> 24) & 0xFF;
        //bytes[1] = ((int) pkt.size >> 16) & 0xFF;
        //bytes[2] = ((int) pkt.size >> 8) & 0xFF;
        //bytes[3] = (int) pkt.size & 0xFF;
        //for (int i = 0; i < 4; i++)
        //    outBytes[i] = bytes[i];

        for (int i = 0; i < (int) pkt.size; i++)
            outBytes[i] = (char) pkt.data[i];
        *outFrameSize = (int) pkt.size;
        av_free_packet(&pkt);

    }
}

void exampleEncoder::close() {

    /* add sequence end code to have a real mpeg file */
    uint8_t endcode[] = { 0, 0, 1, 0xb7 };
    //fwrite(endcode, 1, sizeof(endcode), f);
    //fclose(f);
    avcodec_close(codecContext);
    av_free(codecContext);
    av_freep(&frame->data[0]);
    av_frame_free(&frame);
}

int exampleEncoder::getBitrate() {
    return i_bitrate;
}

void exampleEncoder::setBitrate(int bitrate) {
    i_bitrate = bitrate;
}

int exampleEncoder::getFps() {
    return i_fps;
}

void exampleEncoder::setFps(int fps) {
    i_fps = fps;
}

int exampleEncoder::getGopSize() {
    return i_gopSize;
}

void exampleEncoder::setGopSize(int gopSize) {
    i_gopSize = gopSize;
}

int exampleEncoder::getHeight() {
    return i_height;
}

void exampleEncoder::setHeight(int height) {
    i_height = height;
}

int exampleEncoder::getMaxBframes() {
    return i_maxBframes;
}

void exampleEncoder::setMaxBframes(int maxBframes) {
    i_maxBframes = maxBframes;
}

int exampleEncoder::getWidth() {
    return i_width;
}

void exampleEncoder::setWidth(int width) {
    i_width = width;
}
