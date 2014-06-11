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
./configure --prefix="${CURRENTPATH}/lib-32" CFLAGS="$arch_flags" CXXFLAGS="$arch_flags" CPPFLAGS="$arch_flags" LDFLAGS="$arch_flags" OBJCFLAGS="$arch_flags" --enable-shared=yes --enable-static=no
make
make install
cd ${CURRENTPATH}

echo "Done"
