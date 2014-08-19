#!/bin/sh
CURRENTPATH=`pwd`
arch_flags=""
set -e
LIBCURL="curl-7.20.0"
if [ -e "${CURRENTPATH}/${LIBCURL}" ]; then
	rm -rf "${CURRENTPATH}/${LIBCURL}"
fi
tar zxf ${LIBCURL}.tar.gz
cd "${CURRENTPATH}/${LIBCURL}"

./configure --prefix="${CURRENTPATH}/lib-32" --build=i386-pc-linux-gnu --disable-static --with-ssl="${CURRENTPATH}/lib-32" "CFLAGS=-m32" "CXXFLAGS=-m32" "LDFLAGS=-m32"
make
make install
cd ${CURRENTPATH}

echo "Done"
