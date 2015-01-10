#include "frameEncoder.h"

#define LOG_LEVEL 1
#define LOG_TAG "frameEncoder.cpp"
#define LOGI(level, ...) if (level <= LOG_LEVEL) {__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__);}
#define LOGE(level, ...) if (level <= LOG_LEVEL) {__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__);}

frameEncoder::frameEncoder() :
        in_width(0), in_height(0), in_pixel_format(PIX_FMT_YUV420P), out_width(
                0), out_height(0), out_pixel_format(PIX_FMT_YUV420P), fps(0), encoder(
                NULL), sws(NULL), num_nals(0) {
    memset((char*) &pic_raw, 0, sizeof(pic_raw));
}

frameEncoder::~frameEncoder() {
    if (sws) {
        close();
    }
}

bool frameEncoder::open() {

    int r = 0;
    int nheader = 0;
    int header_size = 0;

    // @todo add validate which checks if all params are set (in/out width/height, fps,etc..);
    if (!validateSettings()) {
        return false;
    }

    if (encoder) {
        LOGI(LOG_LEVEL, "Already opened. first call close()");
        return false;
    }

    if (out_pixel_format != AV_PIX_FMT_YUV420P) {
        LOGI(LOG_LEVEL,
                "At this moment the output format must be AV_PIX_FMT_YUV420P");
        return false;
    }

    // set encoder parameters
    setParams();

    // create the encoder using our params
    encoder = x264_encoder_open(&params);
    if (!encoder) {
        LOGI(LOG_LEVEL, "Cannot open the encoder");
        goto error;
    }

    // write headers
    r = x264_encoder_headers(encoder, &nals, &nheader);
    if (r < 0) {
        LOGI(LOG_LEVEL, "x264_encoder_headers() failed");
        return false;
    }

    // We are skipping writing headers here :(
    header_size = nals[0].i_payload + nals[1].i_payload + nals[2].i_payload;

    sws = sws_getContext(in_width, in_height, in_pixel_format, out_width,
            out_height, out_pixel_format, SWS_FAST_BILINEAR, NULL, NULL, NULL);

    x264_picture_alloc(&pic_in, X264_CSP_I420, out_width, out_height);

    return true;
    error: close();
    return false;
}

void frameEncoder::encodeFrame(char* inBytes, int frameSize, int pts,
        char* outBytes, int *outFrameSize) {

    // copy the pixels into our "raw input" container.
    int bytes_filled = avpicture_fill(&pic_raw, (uint8_t*) inBytes,
            in_pixel_format, in_width, in_height);

    if (!bytes_filled) {
        LOGI(LOG_LEVEL, "Cannot fill the raw input buffer");
        exit(1);
    }

    if (!sws) {
        LOGI(LOG_LEVEL, "Cannot create SWS context");
        exit(1);
    }

    //convert to I420 for x264
    int h = sws_scale(sws, pic_raw.data, pic_raw.linesize, 0, in_height,
            pic_in.img.plane, pic_in.img.i_stride);

    if (h != out_height) {
        LOGI(LOG_LEVEL, "scale failed: %d", h);
        exit(1);
    }

    // and encode and store into pic_out
    pic_in.i_pts = pts;

    int frame_size = x264_encoder_encode(encoder, &nals, &num_nals, &pic_in,
            &pic_out);

    if (frame_size) {

        /*Here first four bytes proceeding the nal unit indicates frame length*/
        unsigned char header[4];
        header[0] = (frame_size >> 24) & 0xFF;
        header[1] = (frame_size >> 16) & 0xFF;
        header[2] = (frame_size >> 8) & 0xFF;
        header[3] = frame_size & 0xFF;

        for (int i = 0; i < frame_size + 4; i++)
            if (i < 4)
                outBytes[i] = header[i];
            else
                outBytes[i] = (char) nals[0].p_payload[i];

        *outFrameSize = frame_size + 4;

        /*for (int i = 0; i < frame_size; i++)
             outBytes[i] = (char) nals[0].p_payload[i];

        *outFrameSize = frame_size;*/
    }
}

bool frameEncoder::close() {
    if (encoder) {
        x264_picture_clean(&pic_in);
        memset((char*) &pic_in, 0, sizeof(pic_in));
        memset((char*) &pic_out, 0, sizeof(pic_out));

        x264_encoder_close(encoder);
        encoder = NULL;
    }

    if (sws) {
        sws_freeContext(sws);
        sws = NULL;
    }
    memset((char*) &pic_raw, 0, sizeof(pic_raw));

    return true;
}

void frameEncoder::setParams() {
    x264_param_default_preset(&params, "veryfast", "zerolatency");

    params.i_threads = getIThreads();
    params.i_width = getOutWidth();
    params.i_height = getOutHeight();
    params.i_fps_num = getFps();
    params.i_fps_den = 1;

    // B frames
    params.i_bframe = getBFrameFrq();
    params.b_sliced_threads = true;
    params.b_vfr_input = true;
    //params.i_timebase_num = 1;
    //params.i_timebase_den = 1000;

    // Intra refres:
    params.i_keyint_max = getFps();
    params.i_keyint_min = 1;
    params.b_intra_refresh = 1;

    // Rate control:
    params.rc.i_rc_method = X264_RC_CRF;
    params.rc.f_rf_constant = 25;
    params.rc.f_rf_constant_max = 35;

    // For streaming:
    params.rc.i_vbv_buffer_size = getVbvBufferSize();
    params.rc.i_vbv_max_bitrate = getBitrate();
    params.b_repeat_headers = 1;
    params.b_annexb = 1;
    params.i_slice_max_size = getSliceMaxSize();

    x264_param_apply_profile(&params, "baseline");

}

bool frameEncoder::validateSettings() {
    if (!in_width) {
        LOGI(LOG_LEVEL, "No in_width set");
        return false;
    }
    if (!in_height) {
        LOGI(LOG_LEVEL, "No in_height set");
        return false;
    }
    if (!out_width) {
        LOGI(LOG_LEVEL, "No out_width set");
        return false;
    }
    if (!out_height) {
        LOGI(LOG_LEVEL, "No out_height set");
        return false;
    }
    if (in_pixel_format == AV_PIX_FMT_NONE) {
        LOGI(LOG_LEVEL, "No in_pixel_format set");
        return false;
    }
    if (out_pixel_format == AV_PIX_FMT_NONE) {
        LOGI(LOG_LEVEL, "No out_pixel_format set");
        return false;
    }
    return true;
}

int frameEncoder::getFps() const {
    return fps;
}

void frameEncoder::setFps(int fps) {
    this->fps = fps;
}

int frameEncoder::getInHeight() const {
    return in_height;
}

void frameEncoder::setInHeight(int inHeight) {
    in_height = inHeight;
}

int frameEncoder::getInWidth() const {
    return in_width;
}

void frameEncoder::setInWidth(int inWidth) {
    in_width = inWidth;
}

int frameEncoder::getNumNals() const {
    return num_nals;
}

void frameEncoder::setNumNals(int numNals) {
    num_nals = numNals;
}

int frameEncoder::getOutHeight() const {
    return out_height;
}

void frameEncoder::setOutHeight(int outHeight) {
    out_height = outHeight;
}

int frameEncoder::getOutWidth() const {
    return out_width;
}

void frameEncoder::setOutWidth(int outWidth) {
    out_width = outWidth;
}

int frameEncoder::getBitrate() const {
    return bitrate;
}

void frameEncoder::setBitrate(int bitrate) {
    bitrate = bitrate;
}

int frameEncoder::getSliceMaxSize() const {
    return i_slice_max_size;
}

void frameEncoder::setSliceMaxSize(int sliceMaxSize) {
    i_slice_max_size = sliceMaxSize;
}

int frameEncoder::getVbvBufferSize() const {
    return i_vbv_buffer_size;
}

void frameEncoder::setVbvBufferSize(int vbvBufferSize) {
    i_vbv_buffer_size = vbvBufferSize;
}

int frameEncoder::getIThreads() const {
    return i_threads;
}

void frameEncoder::setIThreads(int threads) {
    i_threads = threads;
}

int frameEncoder::getBFrameFrq() const {
    return b_frame_frq;
}

void frameEncoder::setBFrameFrq(int frameFrq) {
    b_frame_frq = frameFrq;
}
