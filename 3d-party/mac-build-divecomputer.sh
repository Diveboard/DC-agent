#!/bin/sh
CURRENTPATH=`pwd`
arch_flags="-arch i386"
set -e
mkdir -p ${CURRENTPATH}/lib-32/lib/

LIBDIVECOMPUTER="libdivecomputer"
if [ -e "${CURRENTPATH}/${LIBDIVECOMPUTER}" ]; then
	rm -rf "${CURRENTPATH}/${LIBDIVECOMPUTER}"
fi
#tar zxf ${LIBDIVECOMPUTER}.tar.gz
git clone git://git.libdivecomputer.org/libdivecomputer.git

# Compiling IRDA
cd "${CURRENTPATH}/irda_mac"
make
cp "${CURRENTPATH}/irda_mac/libirda.dylib" "${CURRENTPATH}/lib-32/lib"
install_name_tool -id  @executable_path/libirda.dylib ${CURRENTPATH}/lib-32/lib/libirda.dylib


#Patching libdivecomputer for IRDA support
cd "${CURRENTPATH}/${LIBDIVECOMPUTER}"
patch -N -p1 < "../irda_mac/libdivecomputer_irda_mac.1.patch"
cp "$CURRENTPATH/irda_mac/irda_mac.c" "$CURRENTPATH/${LIBDIVECOMPUTER}/src/"


mkdir -p "${CURRENTPATH}/${LIBDIVECOMPUTER}/build-32" 
cd "${CURRENTPATH}/${LIBDIVECOMPUTER}/build-32"
autoreconf .. --install
DYLD_LIBRARY_PATH="$CURRENTPATH/irda_mac" LIBS="-lirda" ../configure --prefix="${CURRENTPATH}/lib-32" CFLAGS="$arch_flags" CXXFLAGS="$arch_flags -stdlib=libstdc++ -mmacosx-version-min=10.6" CPPFLAGS="$arch_flags -I$CURRENTPATH/irda_mac/" LDFLAGS="$arch_flags -L$CURRENTPATH/irda_mac" OBJCFLAGS="$arch_flags"  --enable-shared=yes --enable-static=no
make
make install
cd ${CURRENTPATH}
install_name_tool -id  @executable_path/libdivecomputer.dylib ${CURRENTPATH}/lib-32/lib/libdivecomputer.dylib
install_name_tool -change libirda.dylib  @executable_path/libirda.dylib ${CURRENTPATH}/lib-32/lib/libdivecomputer.dylib
echo "Done"






