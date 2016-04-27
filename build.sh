#!/bin/sh

sh clean.sh

#echo 'compiling adbi...'

cd adbi
sh build.sh
cd ../


echo 'compiling artdroid...'

cd artdroid/jni
ndk-build clean
ndk-build -B V=1
cd ../../

cd example/artdroid_demo/jni
ndk-build clean
ndk-build -B xV=1
cd ../../../
