#!/bin/sh
CURRENTPATH=`pwd`
arch_flags=""
set -e
OPENSSL="openssl-1.0.1h"
if [ -e "${CURRENTPATH}/${OPENSSL}" ]; then
	rm -rf "${CURRENTPATH}/${OPENSSL}"
fi
tar zxf ${OPENSSL}.tar.gz
cd "${CURRENTPATH}/${OPENSSL}"
./Configure mingw shared --cross-compile-prefix=i686-w64-mingw32- -mwindows --prefix="${CURRENTPATH}/lib-32"
make -k
make -k
make
make install
cd ${CURRENTPATH}
echo "Done"
