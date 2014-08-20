#!/bin/sh
CURRENTPATH=`pwd`
arch_flags=""
set -e
WXWIDGETS="wxWidgets-3.0-trunk"
if [ -e "${CURRENTPATH}/${WXWIDGETS}" ]; then
	rm -rf "${CURRENTPATH}/${WXWIDGETS}"
fi
tar xzvf ${WXWIDGETS}.tar.gz
mkdir -p "${CURRENTPATH}/${WXWIDGETS}/build-32" 
cd "${CURRENTPATH}/${WXWIDGETS}/build-32"
../configure  "CFLAGS=-m32" "CXXFLAGS=-m32" "LDFLAGS=-m32" --build=i386-pc-linux-gnu --disable-shared --enable-unicode --enable-stl --with-libjpeg=builtin --with-libpng=builtin --with-regex=builtin --with-libtiff=builtin --with-zlib=builtin --with-expat=builtin --enable-webkit --enable-webview --enable-webviewwebkit --with-opengl --prefix="${CURRENTPATH}/lib-32/wxWidgets-3.0"
make
make install
cd ${CURRENTPATH}
echo "Done"

