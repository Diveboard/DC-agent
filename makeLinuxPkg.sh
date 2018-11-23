#!/bin/bash
DIR=$(dirname $(readlink -f $0))
BINDIR=$DIR/build/bin
SHAREDIR=$BINDIR/share

DESKTOP="[Desktop Entry]
Name=Diveboard Agent
GenericName=Diveboard Agent
Comment=Sync your dives on diveboard
Exec=DiveboardAgent
Terminal=false
Type=Application
Icon=diveboard
Categories=Network;
StartupNotify=false"

echo "Generate share folder"
mkdir -p $SHAREDIR
mkdir -p $SHAREDIR/applications
mkdir -p $SHAREDIR/icons/hicolor

echo -e "$DESKTOP" > $SHAREDIR/applications/diveboard-agent.desktop

echo "Generate icons"
mkdir -p $SHAREDIR/icons/hicolor/16x16/apps
mkdir -p $SHAREDIR/icons/hicolor/22x22/apps
mkdir -p $SHAREDIR/icons/hicolor/24x24/apps
mkdir -p $SHAREDIR/icons/hicolor/32x32/apps
mkdir -p $SHAREDIR/icons/hicolor/48x48/apps
mkdir -p $SHAREDIR/icons/hicolor/64x64/apps
mkdir -p $SHAREDIR/icons/hicolor/256x256/apps

convert forms/icon_about.png -resize 16x16 $SHAREDIR/icons/hicolor/16x16/apps/diveboard.png
convert forms/icon_about.png -resize 22x22 $SHAREDIR/icons/hicolor/22x22/apps/diveboard.png
convert forms/icon_about.png -resize 24x24 $SHAREDIR/icons/hicolor/24x24/apps/diveboard.png
convert forms/icon_about.png -resize 32x32 $SHAREDIR/icons/hicolor/32x32/apps/diveboard.png
convert forms/icon_about.png -resize 48x48 $SHAREDIR/icons/hicolor/48x48/apps/diveboard.png
convert forms/icon_about.png -resize 64x64 $SHAREDIR/icons/hicolor/64x64/apps/diveboard.png
convert forms/icon_about.png -resize 256x256 $SHAREDIR/icons/hicolor/256x256/apps/diveboard.png

echo "Build TGZ package"
./makeTgzPkg.sh
echo "Build DEB package"
./makeDebPkg.sh
echo "Build RPM package"
./makeRpmPkg.sh

echo "You may now sign the DEB Package using the following command:"
DIR=$(dirname $(readlink -f $0))
VERSION=`cat "$DIR/VERSION"`
if [ `arch` == 'x86_64' ]; then
ARCH=x86_64
else
ARCH=i386
fi
echo "fakeroot dpkg-sig -k \"Diveboard Tech\" --sign builder build/packages/diveboard-agent-$VERSION-$ARCH.deb"

