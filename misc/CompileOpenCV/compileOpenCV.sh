#!/bin/bash

###############################################################
#       This script compiles OpenCV for IDS NXT               #
#       Copy this script to your shared folder                #
#       (normaly <WindowsHomeFolder>\Documents\NXT).          #
#       You can download opencv automatically                 #
#       or download it manually.                              #
#       Open IDS NXT Vision App Build Engine in VirtualBox    #
#       To change keyboard layout to german do                #
#       "sudo loadkeys de"                                    #
#       Change to directory: cd ~/share/                      #
#       Run "bash ./build.sh"                                 #
#       After running this script you can find OpenCV in your #
#       script folder                                         #
###############################################################

OPENCV_VERSION=4.4.0

# Download OpenCV or use already downloaded file
# If you download the openCV sources manually zip has to be named <OPENCV_VERSION>.zip e.g. 4.4.0.zip
DL_OPENCV=true

# Step 1: Save current directory
ORIGINAL_PWD=$(pwd)

# Step 2: Source IDS NXT Toolchain
TOOL_CHAIN_VERSION=$(ls /opt/eldvas)
. /opt/eldvas/$TOOL_CHAIN_VERSION/environment-setup-aarch64-eldvas-linux

# Step 3: Create build directory don't use the shared folder because it can not handle symlinks
cd ~

if [[ -d "OpenCV4NXT" ]]
then
    echo " delete directory OpenCV4NXT"
    rm -r OpenCV4NXT
fi

mkdir OpenCV4NXT
cd OpenCV4NXT

# Step 4.a: Download OpenCV
if [ "$DL_OPENCV" = true ] ; then
    wget https://github.com/opencv/opencv/archive/$OPENCV_VERSION.zip
else
# Or copy existing opencv source into this folder
    if [[ -f $ORIGINAL_PWD/$OPENCV_VERSION.zip ]]
    then
        cp $ORIGINAL_PWD/$OPENCV_VERSION.zip .
    else
        echo "OpenCV source file does not exists. Check file and/or enable the auto-download in this script."
        exit
    fi
fi

# Step 5: unzip OpenCV
if [[ ! -f $OPENCV_VERSION.zip ]]
then
    echo "OpenCV source does not exists."
    exit
fi

unzip $OPENCV_VERSION.zip
cd opencv-$OPENCV_VERSION

# Step 6: Configure OpenCV
rm -rf build
mkdir build
cd build
INSTALLDIR=$(pwd)/cvInstall
mkdir $INSTALLDIR
echo $INSTALLDIR

# Step 7: Build OpenCV
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$INSTALLDIR -DBUILD_opencv_apps=OFF -DBUILD_TESTS=OFF -DBUILD_PERF_TESTS=OFF -DBUILD_JAVA=OFF -DENABLE_PRECOMPILED_HEADERS=OFF -DBUILD_SHARED_LIBS=ON -DWITH_IPP=OFF -DBUILD_PNG=OFF ..

# use -j2 to limit memory and CPU recources. 
# To speed up compiling increase the memory and CPU recources of the virtual machine 
make -j2

# Step 8: Create result as compressed folder
make install

#zip duplicates each symlink. Use this one for VApp deploying because windows can't handle symlinks
zip -r ../cvInstall.zip cvInstall
cd ..

# Step 9: Move result to shared folder
mkdir $ORIGINAL_PWD/build
mv cvInstall.* $ORIGINAL_PWD/build/

# Step 10: Cleanup
rm -r ~/OpenCV4NXT
