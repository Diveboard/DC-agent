CURRENTPATH=`pwd`
arch_flags=""
LIBDIVECOMPUTER="libdivecomputer-0.4.2"
if [ -e "${CURRENTPATH}/${LIBDIVECOMPUTER}" ]; then
	rm -rf "${CURRENTPATH}/${LIBDIVECOMPUTER}"
fi
tar -zxf ${LIBDIVECOMPUTER}.tar.gz
mkdir -p "${CURRENTPATH}/${LIBDIVECOMPUTER}/build-32" 
cd "${CURRENTPATH}/${LIBDIVECOMPUTER}/build-32"
../configure --prefix="${CURRENTPATH}/lib-32" "CFLAGS=-m32" "CXXFLAGS=-m32" "LDFLAGS=-m32" --build=i386-pc-linux-gnu --enable-shared=yes --enable-static=no
make
make install
cd ${CURRENTPATH}
echo "Done"

