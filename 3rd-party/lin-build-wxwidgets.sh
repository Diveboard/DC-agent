#!/bin/sh
CURRENTPATH=`pwd`
arch_flags="-DWX_WEB_EXTENSIONS_DIRECTORY='\"${CURRENTPATH}/lib-32/wxWidgets-3.1/lib/wx/3.1.2/web-extensions\"'"
set -e
WXWIDGETS="wxWidgets-3.1.2"
if [ -e "${CURRENTPATH}/${WXWIDGETS}" ]; then
	rm -rf "${CURRENTPATH}/${WXWIDGETS}"
fi
tar xjvf ${WXWIDGETS}.tar.bz2
mkdir -p "${CURRENTPATH}/${WXWIDGETS}/build-32" 
cd "${CURRENTPATH}/${WXWIDGETS}/build-32"
export DOCBOOK_TO_MAN="xmlto man --skip-validation"
../configure CFLAGS="$arch_flags" CXXFLAGS="$arch_flags" CPPFLAGS="$arch_flags" LDFLAGS="$arch_flags" OBJCFLAGS="$arch_flags" --disable-shared --enable-unicode --enable-stl --with-libjpeg --with-libpng --with-regex=builtin --with-libtiff --with-zlib --with-expat=builtin --enable-webkit --enable-webview --enable-webviewwebkit --with-opengl --with-gtk=3 --prefix="${CURRENTPATH}/lib-32/wxWidgets-3.1" --disable-rpath
make
make install
cd ${CURRENTPATH}
echo "Done"

