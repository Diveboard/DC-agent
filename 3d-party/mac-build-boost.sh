#!/bin/sh
CURRENTPATH=`pwd`
arch_flags="-arch i386"
set -e
LIBBOOST="boost_1_55_0"
#if [ -e "${CURRENTPATH}/${LIBBOOST}" ]; then
#	rm -rf "${CURRENTPATH}/${LIBBOOST}"
#fi
#tar zxf ${LIBBOOST}.tar.gz
cd "${CURRENTPATH}/${LIBBOOST}"
#./bootstrap.sh
./b2 install --prefix="${CURRENTPATH}/lib-32" link=static cflags="$arch_flags" cxxflags="$arch_flags -stdlib=libstdc++" linkflags="$arch_flags -stdlib=libstdc++" --without-context --without-coroutine
cd ${CURRENTPATH}
echo "Done"

