LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := avformat
LOCAL_SRC_FILES := ffmpeg/libavformat/libavformat.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := avcodec
LOCAL_SRC_FILES := ffmpeg/libavcodec/libavcodec.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := swscale
LOCAL_SRC_FILES := ffmpeg/libswscale/libswscale.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := avutil
LOCAL_SRC_FILES := ffmpeg/libavutil/libavutil.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := swresample
LOCAL_SRC_FILES := ffmpeg/libswresample/libswresample.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := postproc
LOCAL_SRC_FILES := ffmpeg/libpostproc/libpostproc.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := x264
LOCAL_SRC_FILES := x264/libx264.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE  := ffmpeg-x264
# These need to be in the right order
FFMPEG_LIBS := $(addprefix ffmpeg/, \
	libavformat/libavformat.a \
	libavcodec/libavcodec.a \
	libswscale/libswscale.a \
	libavutil/libavutil.a \
	libswresample/libswresample.a \
libpostproc/libpostproc.a)

LOCAL_CFLAGS += -g -Iffmpeg -Iffmpeg-x264 -frtti -std=c++11 -Wno-deprecated-declarations
LOCAL_STATIC_LIBRARIES:= avformat avcodec swscale avutil swresample postproc x264
LOCAL_LDLIBS += -llog -lz -Ipthread #$(FFMPEG_LIBS) x264/libx264.a
LOCAL_PRELINK_MODULE := true
LOCAL_SRC_FILES := exampleEncoder.cpp exampleEncoder_wrap.cpp frameEncoder.cpp frameEncoder_wrap.cpp
#LOCAL_SRC_FILES := imageEncoder.cpp imageEncoder_wrap.cpp
include $(BUILD_SHARED_LIBRARY)
