#!/bin/bash

DIR="."

BUILDDIR="$PLUGINDIR/Contents/MacOS"

OUTDIR="$DIR/build/bin"

####

#### xar-ing the archive or creating a DMG file

####

# getting the space used 

SIZE=$(($(du -ks "build/bin/DiveboardAgent.app/" | sed 's/[^0-9].*//') + 500 ))

# Creating a dmg image

cp Mac/DiveboardAgent.dmg build/



DMGFILE=build/DiveboardAgent.dmg

DMGFINALFILE=$OUTDIR/diveboard-agent.dmg


device=$(hdiutil attach -readwrite -noverify -noautoopen "$DMGFILE" | egrep '^/dev/' | sed 1q | awk '{print $1}')
chmod -Rf go-w /Volumes/diveboardagent
cp -r build/bin/DiveboardAgent.app /Volumes/diveboardagent
cp -r Mac/Driver/* /Volumes/diveboardagent/Driver

sync

sync

hdiutil detach ${device}

rm -f "$DMGFINALFILE"

hdiutil convert "$DMGFILE" -format UDZO -imagekey zlib-level=9 -o "${DMGFINALFILE}"


