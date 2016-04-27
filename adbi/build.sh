#!/bin/bash

set -x

cd hijack/jni
ndk-build
cd ../..

cd instruments
cd base/jni
ndk-build -B V=1
cd ../..

cd example/jni
ndk-build
cd ../..

cd ..

