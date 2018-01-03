#!/bin/sh
CURRENTPATH=`pwd`
arch_flags=""
set -e
LIBBOOST="boost_1_57_0"
if [ -e "${CURRENTPATH}/${LIBBOOST}" ]; then
	rm -rf "${CURRENTPATH}/${LIBBOOST}"
fi
tar zxf ${LIBBOOST}.tar.gz --exclude doc/html

cd "${CURRENTPATH}/${LIBBOOST}"
./bootstrap.sh
echo "using gcc : 4.4 : i686-w64-mingw32-g++
        :
        <rc>i686-w64-mingw32-windres
        <archiver>i686-w64-mingw32-ar
;" > user-config.jam
./b2 toolset=gcc target-os=windows variant=release threading=multi threadapi=win32 --prefix="${CURRENTPATH}/lib-32" link=static --without-context --without-coroutine --without-atomic --without-chrono --without-date_time --without-exception --without-filesystem --without-graph --without-graph_parallel --without-iostreams --without-locale --without-log --without-math --without-mpi --without-python --without-program_options --without-random --without-regex --without-serialization --without-signals --without-test --without-timer --without-wave --user-config=user-config.jam install
cd ${CURRENTPATH}
mv lib-32/lib/libboost_thread_win32.a lib-32/lib/libboost_thread.a
echo "Done"

