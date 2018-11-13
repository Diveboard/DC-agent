#!/bin/sh
CURRENTPATH=`pwd`
set -e
cd "3rd-party/"
echo "BUILD 3RD-PARTY"
./win-build-all.sh
cd ${CURRENTPATH}
echo "BUILD CMAKE"
mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../3rd-party/windows-build/toolchain-mingw32.cmake -DCMAKE_SYSTEM_NAME=Windows ..
echo "BUILD PROJECT"
make
cd ..
echo "BUILD SETUP"
makensis platforms/windows/diveboard.nsi
echo "Done"
