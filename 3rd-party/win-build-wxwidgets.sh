#!/bin/sh
CURRENTPATH=`pwd`
arch_flags=""
set -e
WXWIDGETS="wxWidgets-3.1.1"
if [ -e "${CURRENTPATH}/${WXWIDGETS}" ]; then
	rm -rf "${CURRENTPATH}/${WXWIDGETS}"
fi
tar xjvf ${WXWIDGETS}.tar.bz2
mkdir -p "${CURRENTPATH}/${WXWIDGETS}/build-32" 
cd "${CURRENTPATH}/${WXWIDGETS}/build-32"
../configure CFLAGS="$arch_flags" CXXFLAGS="$arch_flags" CPPFLAGS="$arch_flags" LDFLAGS="$arch_flags" OBJCFLAGS="$arch_flags" --disable-shared --enable-unicode --enable-stl --with-libjpeg=builtin --with-libpng=builtin --with-regex=builtin --with-libtiff=builtin --with-zlib=builtin --with-expat=builtin --prefix="${CURRENTPATH}/lib-32/wxWidgets-3.0" --host=i686-w64-mingw32 --build=i686-linux
make
make install
cd ${CURRENTPATH}
echo "Done"
