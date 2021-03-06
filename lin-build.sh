#!/bin/sh
CURRENTPATH=`pwd`
set -e
cd "3rd-party/"
echo "BUILD 3RD-PARTY"
./lin-build-all.sh
cd ${CURRENTPATH}
echo "BUILD CMAKE"
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
echo "BUILD PROJECT"
make
cd ..
echo "BUILD SETUP"
./makeLinuxPkg.sh
echo "Done"
