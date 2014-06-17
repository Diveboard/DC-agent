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
./config --prefix="${CURRENTPATH}/lib-32 --shared" 
make
make install
cd ${CURRENTPATH}
echo "Done"
