#!/bin/sh
CURRENTPATH=`pwd`
set -e
cd "3d-party/"
echo "BUILD 3D-PARTY"
./lin-build-all.sh
cd ${CURRENTPATH}
echo "BUILD CMAKE"
mkdir build
cd build
cmake ..
echo "BUILD PROJECT"
make
cd ..
echo "BUILD SETUP"
makeLinuxPkg.sh
echo "Done"
