#!/bin/sh
CURRENTPATH=`pwd`
arch_flags="-arch i386"
set -e
LIBDIVECOMPUTER="libdivecomputer-0.4.1"
if [ -e "${CURRENTPATH}/${LIBDIVECOMPUTER}" ]; then
	rm -rf "${CURRENTPATH}/${LIBDIVECOMPUTER}"
fi
tar zxf ${LIBDIVECOMPUTER}.tar.gz
mkdir -p "${CURRENTPATH}/${LIBDIVECOMPUTER}/build-32" 
cd "${CURRENTPATH}/${LIBDIVECOMPUTER}/build-32"
../configure --prefix="${CURRENTPATH}/lib-32" CFLAGS="$arch_flags" CXXFLAGS="$arch_flags -stdlib=libstdc++ -mmacosx-version-min=10.6" CPPFLAGS="$arch_flags" LDFLAGS="$arch_flags" OBJCFLAGS="$arch_flags" --enable-shared=yes --enable-static=no
make
make install
cd ${CURRENTPATH}
install_name_tool -id  @executable_path/libdivecomputer.dylib ${CURRENTPATH}/lib-32/lib/libdivecomputer.dylib
echo "Done"
