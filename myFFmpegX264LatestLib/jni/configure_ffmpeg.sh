#!/bin/bash
pushd `dirname $0`
. settings.sh

if [[ $minimal_featureset == 1 ]]; then
  echo "Using minimal featureset"
  featureflags=" \
--enable-decoder=mjpeg --enable-demuxer=mjpeg --enable-parser=mjpeg \
--enable-demuxer=image2 --enable-muxer=mp4 --enable-encoder=libx264 --enable-libx264 \
--enable-decoder=rawvideo \
--enable-protocol=file \
--enable-protocol=rtp
--enable-muxer=rtp \
--enable-hwaccels"
fi

if [[ $DEBUG == 1 ]]; then
  echo "DEBUG = 1"
  DEBUG_FLAG="--disable-stripping"
fi

# I haven't found a reliable way to install/uninstall a patch from a Makefile,
# so just always try to apply it, and ignore it if it fails. Works fine unless
# the files being patched have changed, in which cause a partial application
# could happen unnoticed.
#patch -N -p1 --reject-file=- < redact-plugins.patch
#patch -N -p1 --reject-file=- < arm-asm-fix.patch
#patch -d ffmpeg -N -p1 --reject-file=- < \
#        ARM_generate_position_independent_code_to_access_data_symbols.patch
#patch -d ffmpeg -N -p1 --reject-file=- < \
#        ARM_intmath_use_native-size_return_types_for_clipping_functions.patch
#patch -d ffmpeg -N -p1 --reject-file=- < \
#       enable-fake-pkg-config.patch

pushd ffmpeg

export PKG_CONFIG_LIBDIR=../x264
export PKG_CONFIG_PATH=../x264

RANLIB="arm-linux-androideabi-ranlib"
PKG_CONFIG=/home/aditya/workspace/myFFmpegX264LatestLib/jni/toolchain/arm-linux-androideabi/bin/arm-linux-androideabi-pkg-config
./configure \
--enable-cross-compile \
--arch=arm5te \
--enable-runtime-cpudetect \
--target-os=linux \
--disable-stripping \
--cross-prefix=arm-linux-androideabi- \
--prefix=../output \
--disable-asm \
--enable-version3 \
--disable-shared \
--enable-static \
--enable-gpl \
--enable-memalign-hack \
--extra-cflags="-fPIC -DANDROID -D__thumb__ -mthumb -Wfatal-errors -Wno-deprecated -I../x264 -lx264" \
--enable-decoder=h264 \
--enable-demuxer=mov \
--enable-muxer=mp4 \
--enable-encoder=libx264 \
--enable-libx264 \
--enable-protocol=file \
--enable-decoder=aac \
--enable-encoder=aac \
$featureflags \
--disable-ffmpeg \
--disable-ffplay \
--disable-ffprobe \
--disable-ffserver \
--disable-indev=v4l2 \
--extra-ldflags="-L../x264" \
--extra-libs="-lgcc" \
--enable-hwaccel=h264_vaapi \
--enable-hwaccel=h264_dxva2 \
--enable-hwaccel=mpeg4_vaapi \
--enable-demuxer=h264 \
--enable-demuxer=mpegvideo \
--enable-demuxer=h263 \
--enable-demuxer=mpegps \
--enable-demuxer=mjpeg \
--enable-demuxer=rtsp \
--enable-demuxer=rtp \
--enable-demuxer=hls \
--enable-muxer=rtsp \
--enable-muxer=mp4 \
--enable-muxer=mjpeg \
--enable-protocol=rtp \
--enable-protocol=tcp \
--enable-protocol=udp \
--enable-protocol=hls \
--enable-protocol=http \
--enable-decoder=xsub \
--enable-decoder=jacosub \
--enable-decoder=dvdsub \
--enable-decoder=dvbsub \
--enable-decoder=subviewer \
--enable-decoder=rawvideo \
--enable-encoder=rawvideo \
--enable-decoder=mjpeg \
--enable-encoder=mjpeg \
--enable-decoder=h263 \
--enable-decoder=mpeg4 \
--enable-encoder=mpeg4 \
--enable-decoder=h264 \
--enable-parser=h264 \
--enable-decoders \
--enable-encoders \
--enable-parsers \
--enable-hwaccels \
--enable-muxers \
--enable-avformat \
--enable-avcodec \
--enable-avresample \
--enable-zlib \
--disable-doc \
--disable-ffplay \
--disable-ffmpeg \
--disable-ffplay \
--disable-ffprobe \
--disable-ffserver \
--disable-avfilter \
--disable-avdevice \
--enable-nonfree

popd; popd
