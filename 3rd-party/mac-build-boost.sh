#!/bin/sh
CURRENTPATH=`pwd`
arch_flags="-arch i386"
set -e
LIBBOOST="boost_1_68_0"
if [ -e "${CURRENTPATH}/${LIBBOOST}" ]; then
	rm -rf "${CURRENTPATH}/${LIBBOOST}"
fi
tar xjf ${LIBBOOST}.tar.bz2
cd "${CURRENTPATH}/${LIBBOOST}"
./bootstrap.sh
./b2 install --prefix="${CURRENTPATH}/lib-32" link=static cflags="$arch_flags" cxxflags="$arch_flags -stdlib=libstdc++" linkflags="$arch_flags -stdlib=libstdc++" --without-coroutine --without-atomic --without-chrono --without-date_time --without-exception --without-filesystem --without-graph --without-graph_parallel --without-iostreams --without-locale --without-log --without-math --without-mpi --without-python --without-program_options --without-random --without-regex --without-serialization --without-signals --without-test --without-timer --without-wave 
cd ${CURRENTPATH}
echo "Done"

