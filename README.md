#myFFmpegX264LatestLibAndroid
FFmpeg library with libx264 for android.  

##Dependencies
This project uses SWIG framework for development of JNI interface.

##Features
This project provides a nice and simple JNI interface to use ffmpeg based h264 software encoder on android devices. This encoder is highly configurable unlike existing android API's like MediaCodec or MediaRecorder.

##Building
Ensure NDK_BASE env variable is set to the location of your NDK, example:

export NDK_BASE=/path/to/android-ndk

###Then execute:
git submodule update --init --recursive

cd ~/git/myFFmpegX264LatestLibAndroid/myFFmpegX264LatestLibAndroid/jni

./configure_make_everything.sh

After compilation, build the project in Eclipse.
