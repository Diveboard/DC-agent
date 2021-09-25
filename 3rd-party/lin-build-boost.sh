#!/bin/sh
CURRENTPATH=`pwd`
arch_flags=""
set -e
LIBBOOST="boost_1_77_0"
if [ -e "${CURRENTPATH}/${LIBBOOST}" ]; then
	rm -rf "${CURRENTPATH}/${LIBBOOST}"
fi
tar xjf ${LIBBOOST}-src.tar.bz2 --exclude doc/html
tar xjf ${LIBBOOST}-libs.tar.bz2 --exclude doc/html
cd "${CURRENTPATH}/${LIBBOOST}"
./bootstrap.sh
./b2 install --prefix="${CURRENTPATH}/lib-32" link=static --without-context --without-coroutine --without-atomic --without-chrono --without-date_time --without-exception --without-filesystem --without-graph --without-graph_parallel --without-iostreams --without-locale --without-log --without-math --without-mpi --without-python --without-program_options --without-random --without-regex --without-serialization --without-test --without-timer --without-wave
cd ${CURRENTPATH}
echo "Done"

