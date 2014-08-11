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


./configure --prefix="${CURRENTPATH}/lib-32" CFLAGS="$arch_flags" CXXFLAGS="$arch_flags" CPPFLAGS="$arch_flags" LDFLAGS="$arch_flags" OBJCFLAGS="$arch_flags" --enable-shared=yes --enable-static=no
make
make install
cd ${CURRENTPATH}
echo "Done"
