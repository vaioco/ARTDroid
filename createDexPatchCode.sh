#!/bin/bash


echo "working with: $1"

cp -v $1 /private/tmp/

OLDIR=`pwd`

cd /private/tmp
echo "cleaning.."
rm classes.dex
#echo "running dx"
#~/Library/Android/sdk/build-tools/23.0.2/dx --debug --dex --output=a.apk ~/Documents/seccon.jar
apkname=$(basename "$1")
echo "working on $apkname"
unzip -o $apkname
ls -la classes.dex

adb push ./classes.dex /data/local/tmp/target.dex

cd $OLDIR
