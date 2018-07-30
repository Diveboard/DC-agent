CURRENTPATH=`pwd`
arch_flags=""
LIBDIVECOMPUTER="libdivecomputer-0.5.0"
if [ -e "${CURRENTPATH}/${LIBDIVECOMPUTER}" ]; then
	rm -rf "${CURRENTPATH}/${LIBDIVECOMPUTER}"
fi
tar -zxf ${LIBDIVECOMPUTER}.tar.gz
$git clone git://git.libdivecomputer.org/libdivecomputer.git
mkdir -p "${CURRENTPATH}/${LIBDIVECOMPUTER}/build-32" 
cd "${CURRENTPATH}/${LIBDIVECOMPUTER}/build-32"
autoreconf .. --install
../configure --prefix="${CURRENTPATH}/lib-32" --host=i686-w64-mingw32 --build=i686-linux CFLAGS="$arch_flags" CXXFLAGS="$arch_flags" CPPFLAGS="$arch_flags" LDFLAGS="$arch_flags" OBJCFLAGS="$arch_flags" --enable-shared=yes --enable-static=no --enable-examples=no --without-bluez
make
make install
cd ${CURRENTPATH}
echo "Done"
