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

cd "src"
patch  < "../../windows-build/windows_curl.patch"
cd ".."

ac_cv_path_PKGCONFIG=/usr/bin/pkg-config ac_cv_file___dev_urandom_=yes ./configure --prefix="${CURRENTPATH}/lib-32" --host=i686-w64-mingw32 --build=i686-linux --disable-static --with-ssl="${CURRENTPATH}/lib-32"
make
make install
cd ${CURRENTPATH}

echo "Done"
