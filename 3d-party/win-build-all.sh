#!/bin/sh
echo "BUILD BOOST"
./win-build-boost.sh
echo "BUILD LIBCONFIG"
./win-build-libconfig.sh
echo "BUILD LIBICONV"
./win-build-libiconv.sh
echo "BUILD RAPIDJSON"
./win-build-rapidjson.sh
echo "BUILD DIVECOMPUTER"
./win-build-divecomputer.sh
echo "BUILD OPENSSL"
./win-build-openssl.sh
echo "BUILD LIBCURL"
./win-build-libcurl.sh
echo "BUILD WXWIDGET"
./win-build-wxwidgets.sh
