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
# this doesn't build on the first shot, which in turn
# leads to a libcurl being built without ssl support
# ... so we need to retry here and hope for the best.
make
make
make
make
make
make install
cd ${CURRENTPATH}
echo "Done"
