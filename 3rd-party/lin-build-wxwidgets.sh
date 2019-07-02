#!/bin/sh
CURRENTPATH=`pwd`
arch_flags=""
set -e
WXWIDGETS="wxWidgets-3.1.2"
if [ -e "${CURRENTPATH}/${WXWIDGETS}" ]; then
	rm -rf "${CURRENTPATH}/${WXWIDGETS}"
fi
tar xjvf ${WXWIDGETS}.tar.bz2
mkdir -p "${CURRENTPATH}/${WXWIDGETS}/build-32" 
cd "${CURRENTPATH}/${WXWIDGETS}/build-32"
../configure CFLAGS="$arch_flags" CXXFLAGS="$arch_flags" CPPFLAGS="$arch_flags" LDFLAGS="$arch_flags" OBJCFLAGS="$arch_flags" --disable-shared --enable-unicode --enable-stl --with-libjpeg --with-libpng --with-regex=builtin --with-libtiff --with-zlib --with-expat=builtin --enable-webkit --enable-webview --enable-webviewwebkit --with-opengl --prefix="${CURRENTPATH}/lib-32/wxWidgets-3.1" --disable-rpath
make
make install
cd ${CURRENTPATH}
echo "Done"

