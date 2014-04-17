#!/bin/sh
CURRENTPATH=`pwd`
arch_flags="-arch i386"
set -e
LIBCONFIG="libconfig-1.4.9"
if [ -e "${CURRENTPATH}/${LIBCONFIG}" ]; then
	rm -rf "${CURRENTPATH}/${LIBCONFIG}"
fi
tar zxf ${LIBCONFIG}.tar.gz
cd "${CURRENTPATH}/${LIBCONFIG}"
./configure --prefix="${CURRENTPATH}/lib-32" CFLAGS="$arch_flags" CXXFLAGS="$arch_flags" CPPFLAGS="$arch_flags" LDFLAGS="$arch_flags" OBJCFLAGS="$arch_flags" --enable-shared=yes --enable-static=no
make
make install
cd ${CURRENTPATH}
echo "Done"
