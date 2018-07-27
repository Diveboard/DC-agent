#!/bin/sh
CURRENTPATH=`pwd`
set -e
LIBRAPIDJSON="rapidjson"
if [ -e "${CURRENTPATH}/${LIBRAPIDJSON}" ]; then
	rm -rf "${CURRENTPATH}/${LIBRAPIDJSON}"
fi
unzip ${LIBRAPIDJSON}-0.11.zip
cd "${CURRENTPATH}/${LIBRAPIDJSON}"
cp -R include/ "${CURRENTPATH}/lib-32/include"
cd ${CURRENTPATH}
echo "Done"
