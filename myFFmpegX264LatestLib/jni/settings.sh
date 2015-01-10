#!/bin/bash

# set to path of your NDK (or export NDK to environment)
#NDK=/Users/jkoehl/Dev/android-ndk

if [[ "x$NDK" == "x" ]]; then
NDK=~/adt-bundle-linux-x86_64-20140702/ndk_64/android-ndk-r10c
fi
# i use only a small number of formats - set this to 0 if you want everything.
# changed 0 to the default, so it'll compile shitloads of codecs normally
if [[ "x$minimal_featureset" == "x" ]]; then
minimal_featureset=0
fi

## stop editing

if [[ ! -d $NDK ]]; then
  echo "$NDK is not a directory. Exiting."
  exit 1
fi

function current_dir {
  echo "$(cd "$(dirname $0)"; pwd)"
}

export RANLIB="arm-linux-androideabi-ranlib"
export PATH=$PATH:$NDK:$(current_dir)/toolchain/bin

#echo $PATH
#echo $RANLIB

