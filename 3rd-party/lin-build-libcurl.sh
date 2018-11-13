#!/bin/sh
CURRENTPATH=`pwd`
arch_flags=""
set -e
LIBCURL="curl-7.47.0"
if [ -e "${CURRENTPATH}/${LIBCURL}" ]; then
	rm -rf "${CURRENTPATH}/${LIBCURL}"
fi
tar zxf ${LIBCURL}.tar.gz
cd "${CURRENTPATH}/${LIBCURL}"

./configure --prefix="${CURRENTPATH}/lib-32" --disable-static --with-ssl="${CURRENTPATH}/lib-32"
make
make install
cd ${CURRENTPATH}

echo "Done"
