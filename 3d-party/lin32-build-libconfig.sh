#!/bin/sh
CURRENTPATH=`pwd`
arch_flags=""
set -e
LIBCONFIG="libconfig-1.4.9"
if [ -e "${CURRENTPATH}/${LIBCONFIG}" ]; then
	rm -rf "${CURRENTPATH}/${LIBCONFIG}"
fi
tar zxf ${LIBCONFIG}.tar.gz
cd "${CURRENTPATH}/${LIBCONFIG}"
./configure --prefix="${CURRENTPATH}/lib-32"  "CFLAGS=-m32" "CXXFLAGS=-m32" "LDFLAGS=-m32" --build=i386-pc-linux-gnu --enable-shared=yes --enable-static=no
make
make install
cd ${CURRENTPATH}
echo "Done"
