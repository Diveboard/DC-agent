#!/bin/sh
CURRENTPATH=`pwd`
arch_flags=""
set -e
LIBICONV="libiconv-1.14"
if [ -e "${CURRENTPATH}/${LIBICONV}" ]; then
	rm -rf "${CURRENTPATH}/${LIBICONV}"
fi
tar zxf ${LIBICONV}.tar.gz
cd "${CURRENTPATH}/${LIBICONV}"

patch -p0 < "../linux-build/libiconv-glibc-2.16.patch" 


./configure --prefix="${CURRENTPATH}/lib-32"  "CFLAGS=-m32" "CXXFLAGS=-m32" "LDFLAGS=-m32" --build=i386-pc-linux-gnu --enable-shared=yes --enable-static=no
make
make install
cd ${CURRENTPATH}
echo "Done"
