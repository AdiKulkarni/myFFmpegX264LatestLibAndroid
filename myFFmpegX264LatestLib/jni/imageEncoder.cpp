#include "imageEncoder.h"

#define LOG_LEVEL 1
#define LOG_TAG "imageEncoder.cpp"
#define LOGI(level, ...) if (level <= LOG_LEVEL) {__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__);}
#define LOGE(level, ...) if (level <= LOG_LEVEL) {__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__);}

imageEncoder::~imageEncoder() {
    if (fooContext) {
        closeImageEncoder();
    }
}

imageEncoder::imageEncoder() :
        fooContext(NULL) {
    //init buffer
    output_buffer_size = 500000;
    output_buffer = (unsigned char*) malloc(output_buffer_size);
    initImageEncoder();
}

int imageEncoder::initImageEncoder() {

    avcodec_register_all();

    /* find the H264 video encoder */
    codec = avcodec_find_encoder(CODEC_ID_H264);
    if (!codec) {
        LOGI(LOG_LEVEL, "avcodec_find_encoder() run fail.");
        return -5;
    }

    //allocate context
    codecCtx = avcodec_alloc_context3(codec);

    /* put sample parameters */
    codecCtx->bit_rate = 400000;
    /* resolution must be a multiple of two */
    codecCtx->width = 640;
    codecCtx->height = 480;
    /* frames per second */
    codecCtx->time_base = (AVRational ) { 1, 15 };
    codecCtx->pix_fmt = PIX_FMT_YUV420P;
    codecCtx->codec_id = CODEC_ID_H264;
    codecCtx->codec_type = AVMEDIA_TYPE_VIDEO;

    /* open it */
    if (avcodec_open2(codecCtx, codec, NULL) < 0) {
        LOGI(LOG_LEVEL, "avcodec_open() run fail.");
        return -10;
    }

    fooContext = sws_getContext(codecCtx->width, codecCtx->height,
            PIX_FMT_YUV420P, codecCtx->width, codecCtx->height, PIX_FMT_YUV420P,
            SWS_FAST_BILINEAR, NULL, NULL, NULL);

    return 0;
}

void imageEncoder::encodeImage(char* inImage, int inImageSize, int counter,
        char* outImage, int *outImageSize) {

    if (!fooContext) {
        LOGI(LOG_LEVEL, "Not initialized, so cannot encode");
        exit(1);
    }
    int out_size;
    AVFrame *picture;
    AVFrame *outpic;
    uint8_t *outbuffer;

    //allocate frame
    picture = avcodec_alloc_frame();
    outpic = avcodec_alloc_frame();

    int nbytes = avpicture_get_size(PIX_FMT_YUV420P, codecCtx->width,
            codecCtx->height);
    outbuffer = (uint8_t*) av_malloc(nbytes);
    outpic->pts = counter;

    //fill picture with image
    avpicture_fill((AVPicture*) picture, (uint8_t*) inImage, PIX_FMT_YUV420P,
            codecCtx->width, codecCtx->height);

    //fill outpic with empty image
    avpicture_fill((AVPicture*) outpic, outbuffer, PIX_FMT_YUV420P,
            codecCtx->width, codecCtx->height);

    //rescale the image
    sws_scale(fooContext, picture->data, picture->linesize, 0, codecCtx->height,
            outpic->data, outpic->linesize);

    //encode the image
    out_size = avcodec_encode_video(codecCtx, output_buffer, output_buffer_size,
            outpic);

    // write to a outImage
    for (int i = 0; i < (int) output_buffer_size; i++)
        outImage[i] = (char) output_buffer[i];

    *outImageSize = (int) output_buffer_size;

    //release pictures
    av_free(outbuffer);
    av_free(picture);
    av_free(outpic);
}

void imageEncoder::closeImageEncoder() {
    free(output_buffer);
    avcodec_close(codecCtx);
    av_free(codecCtx);
}

uint8_t* imageEncoder::getOutputBuffer() {
    return output_buffer;
}

void imageEncoder::setOutputBuffer(uint8_t* outputBuffer) {
    output_buffer = outputBuffer;
}

int imageEncoder::getOutputBufferSize() {
    return output_buffer_size;
}

void imageEncoder::setOutputBufferSize(int outputBufferSize) {
    output_buffer_size = outputBufferSize;
}
