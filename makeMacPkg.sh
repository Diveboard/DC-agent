#!/bin/bash

USER=$(whoami)
if [ "root" != "$USER" ]; then
	echo "remember to call"
	echo "sudo $0"
	exit 1
fi

# make sure the image isn't mounted
hdiutil detach /Volumes/DiveboardAgent

DIR="."
BUILDDIR="$PLUGINDIR/Contents/MacOS"
OUTDIR="$DIR/build/bin"

# calculate the space used 
SIZE=$(($(du -ks "build/bin/DiveboardAgent.app/" | sed 's/[^0-9].*//') + 500 ))

# create the dmg image
cp platforms/macos/DiveboardAgent.dmg build/

DMGFILE=build/DiveboardAgent.dmg
DMGFINALFILE=$OUTDIR/diveboard-agent.dmg

device=$(hdiutil attach -readwrite -noverify -noautoopen "$DMGFILE" | egrep '^/dev/' | sed 1q | awk '{print $1}')
chmod -Rf go-w /Volumes/diveboardagent
cp -R build/bin/DiveboardAgent.app /Volumes/diveboardagent
cp -R platforms/macos/Driver /Volumes/diveboardagent

sync
sync

hdiutil detach ${device}
rm -f "$DMGFINALFILE"
hdiutil convert "$DMGFILE" -format UDZO -imagekey zlib-level=9 -o "${DMGFINALFILE}"

