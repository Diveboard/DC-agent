#!/bin/sh
CURRENTPATH=`pwd`
set -e
cd "3d-party/"
echo "BUILD 3D-PARTY"
./win-build-all.sh
cd ${CURRENTPATH}
echo "BUILD CMAKE"
mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../3d-party/windows-build/Toolchain-mingw32.cmake -DCMAKE_SYSTEM_NAME=Windows ..
echo "BUILD PROJECT"
make
cd ..
echo "BUILD SETUP"
makensis Win/Diveboard.nsi
echo "Done"
