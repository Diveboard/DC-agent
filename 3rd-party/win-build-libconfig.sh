#!/bin/sh
CURRENTPATH=`pwd`
arch_flags=""
set -e
LIBCONFIG="libconfig-1.5"
if [ -e "${CURRENTPATH}/${LIBCONFIG}" ]; then
	rm -rf "${CURRENTPATH}/${LIBCONFIG}"
fi
tar zxf ${LIBCONFIG}.tar.gz
cd "${CURRENTPATH}/${LIBCONFIG}"
./configure --prefix="${CURRENTPATH}/lib-32" --host=i686-w64-mingw32 --build=i686-linux CFLAGS="$arch_flags" CXXFLAGS="$arch_flags" CPPFLAGS="$arch_flags" LDFLAGS="$arch_flags" OBJCFLAGS="$arch_flags" --enable-shared=yes --enable-static=no
make
make install
cd ${CURRENTPATH}
echo "Done"
