#!/bin/sh
CURRENTPATH=`pwd`
arch_flags="-arch i386"
set -e
LIBCONFIG="libconfig-1.5"
if [ -e "${CURRENTPATH}/${LIBCONFIG}" ]; then
	rm -rf "${CURRENTPATH}/${LIBCONFIG}"
fi
tar zxf ${LIBCONFIG}.tar.gz
cd "${CURRENTPATH}/${LIBCONFIG}"
./configure --prefix="${CURRENTPATH}/lib-32" CFLAGS="$arch_flags -mmacosx-version-min=10.6" CXXFLAGS="$arch_flags -stdlib=libstdc++ -mmacosx-version-min=10.6" CPPFLAGS="$arch_flags" LDFLAGS="$arch_flags" OBJCFLAGS="$arch_flags" --enable-shared=yes --enable-static=no
make
make install
cd ${CURRENTPATH}
install_name_tool -id  @executable_path/libconfig++.dylib ${CURRENTPATH}/lib-32/lib/libconfig++.dylib

echo "Done"
