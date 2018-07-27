#!/bin/sh
echo "BUILD BOOST"
./lin-build-boost.sh
echo "BUILD LIBICONV"
./lin-build-libiconv.sh
echo "BUILD RAPIDJSON"
./lin-build-rapidjson.sh
echo "BUILD DIVECOMPUTER"
./lin-build-divecomputer.sh
echo "BUILD OPENSSL"
./lin-build-openssl.sh
echo "BUILD LIBCURL"
./lin-build-libcurl.sh
echo "BUILD WXWIDGET"
./lin-build-wxwidgets.sh
