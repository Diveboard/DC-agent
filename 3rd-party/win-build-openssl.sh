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
./Configure mingw shared no-ssl2 --cross-compile-prefix=i686-w64-mingw32- -mwindows --prefix="${CURRENTPATH}/lib-32"
# doesn't build properly in parallel
make -j1
make install
cd ${CURRENTPATH}
echo "Done"
