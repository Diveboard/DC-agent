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
./configure --prefix="${CURRENTPATH}/lib-32" --host=i686-w64-mingw32 --build=i686-linux CFLAGS="$arch_flags" CXXFLAGS="$arch_flags" CPPFLAGS="$arch_flags" LDFLAGS="$arch_flags" OBJCFLAGS="$arch_flags" --enable-shared=yes --enable-static=no
patch lib/iconv.c ../windows-build/libiconv.patch
make
make install
cd ${CURRENTPATH}

echo "Done"
