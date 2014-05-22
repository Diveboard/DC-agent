#!/bin/sh
CURRENTPATH=`pwd`
arch_flags="-arch i386"
set -e
WXWIDGETS="wxWidgets-3.0-trunk"
if [ -e "${CURRENTPATH}/${WXWIDGETS}" ]; then
	rm -rf "${CURRENTPATH}/${WXWIDGETS}"
fi
tar xzvf ${WXWIDGETS}.tar.gz
mkdir -p "${CURRENTPATH}/${WXWIDGETS}/build-32" 
cd "${CURRENTPATH}/${WXWIDGETS}/build-32"
../configure CFLAGS="$arch_flags" CXXFLAGS="$arch_flags -stdlib=libstdc++" CPPFLAGS="$arch_flags" LDFLAGS="$arch_flags" OBJCFLAGS="$arch_flags" --with-macosx-version-min=10.6  --disable-shared --enable-unicode --enable-stl --with-libjpeg=builtin --with-libpng=builtin --with-regex=builtin --with-libtiff=builtin --with-zlib=builtin --with-expat=builtin --prefix="$(pwd)" 
make
cd ${CURRENTPATH}
echo "Done"
