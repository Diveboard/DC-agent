#!/bin/sh
echo "BUILD DIVECOMPUTER"
./mac-build-divecomputer.sh
echo "BUILD LIBCONFIG"
./mac-build-libconfig.sh
echo "BUILD RAPIDJSON"
./mac-build-rapidjson.sh
echo "BUILD BOOST"
./mac-build-boost.sh
echo "BUILD WXWIDGET"
./mac-build-wxwidgets.sh
