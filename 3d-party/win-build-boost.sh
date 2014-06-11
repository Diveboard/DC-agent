#!/bin/sh
CURRENTPATH=`pwd`
arch_flags=""
set -e
LIBBOOST="boost_1_55_0"
if [ -e "${CURRENTPATH}/${LIBBOOST}" ]; then
	rm -rf "${CURRENTPATH}/${LIBBOOST}"
fi
tar zxf ${LIBBOOST}.tar.gz
cd "${CURRENTPATH}/${LIBBOOST}"
# run bootstrap.bat mingw from windows command prompt
cmd
./b2 toolset=gcc install --prefix="${CURRENTPATH}/lib-32" link=static --without-context --without-coroutine
cd ${CURRENTPATH}
echo "Done"

