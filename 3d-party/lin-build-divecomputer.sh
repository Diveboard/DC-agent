CURRENTPATH=`pwd`
arch_flags=""
LIBDIVECOMPUTER="libdivecomputer-0.6.0"
if [ -e "${CURRENTPATH}/${LIBDIVECOMPUTER}" ]; then
	rm -rf "${CURRENTPATH}/${LIBDIVECOMPUTER}"
fi
tar -zxf ${LIBDIVECOMPUTER}.tar.gz
#git clone https://github.com/libdivecomputer/libdivecomputer.git
mkdir -p "${CURRENTPATH}/${LIBDIVECOMPUTER}/build-32" 
cd "${CURRENTPATH}/${LIBDIVECOMPUTER}/build-32"
autoreconf .. --install
../configure --prefix="${CURRENTPATH}/lib-32" CFLAGS="$arch_flags" CXXFLAGS="$arch_flags" CPPFLAGS="$arch_flags" LDFLAGS="$arch_flags" OBJCFLAGS="$arch_flags" --enable-shared=yes --enable-static=no
make
make install
cd ${CURRENTPATH}
echo "Done"

