#!/bin/bash

DIR=$(dirname $(readlink -f $0))

if [ `arch` == 'x86_64' ]; then
ARCH=x86_64
ARCHITECTURE=amd64
else
ARCH=i386
ARCHITECTURE=i386
fi

VERSION=`cat "$DIR/VERSION"`


BINDIR=$DIR/build/bin
SHAREDIR=$BINDIR/share
BUILDDIR=$BINDIR/deb
OUTDIR=$DIR/build/packages

LIBDIVE=$BINDIR/libdivecomputer.so
APPDIVEBOARD=$BINDIR/DiveboardAgent

PKGNAME=diveboard-agent-$VERSION-$ARCH.deb

INSTALLED_BYTES=`du -bsc $LIBDIVE $APPDIVEBOARD |tail -n 1 |cut -f 1`
INSTALLED_SIZE=`expr $INSTALLED_BYTES / 1024`

CONTROL="Package:diveboard-agent
Version: $VERSION
Section: web
Priority: optional
Architecture: $ARCHITECTURE
Depends: libc6 (>= 2.11), libssl1.0.0, libcurl3, libwebkitgtk-1.0-0
Replaces: diveboard (<= 1.1.1)
Maintainer: Diveboard <support@diveboard.com>
Installed-Size: $INSTALLED_SIZE
Description: Web Browser plugin for DiveBoard:
 http://www.diveboard.com
 .
 With the DiveBoard browser plugin you can upload your dive
 profiles direct from a supported dive computer to the web.
 This plugin links with Jef Driesen's LibDiveComputer http://www.divesoftware.org/libdc/

"

#
# Put the package together
#

if [ ! -f $APPDIVEBOARD ]; then
    echo Unable to find DiveBoard - did you build it?
    echo MISSING: $APPDIVEBOARD
    exit 1
fi
if [ ! -f $LIBDIVE ]; then
    echo Unable to find libdivecomputer - did you build it?
    echo MISSING: $LIBDIVE
    exit 1
fi

echo Creating build structure...
mkdir -p $OUTDIR
rm -f $OUTDIR/$PKGNAME
rm -rf $BUILDDIR
mkdir -p $BUILDDIR/usr/lib/diveboard
mkdir -p $BUILDDIR/usr/bin/

echo Copying package contents...
cp $APPDIVEBOARD $BUILDDIR/usr/bin/DiveboardAgent
cp $LIBDIVE $BUILDDIR/usr/lib/diveboard/libdivecomputer.so
cp -r $SHAREDIR $BUILDDIR/usr/

echo Stripping binaries...
strip $BUILDDIR/usr/bin/DiveboardAgent
strip $BUILDDIR/usr/lib/diveboard/libdivecomputer.so

echo Fixing permissions...
chmod 755 $BUILDDIR/usr/bin/DiveboardAgent
chmod 644 $BUILDDIR/usr/lib/diveboard/libdivecomputer.so
#chmod 644 $BUILDDIR/usr/share/* -R

echo Creating control file...
mkdir -p $BUILDDIR/DEBIAN
echo -e "$CONTROL" > $BUILDDIR/DEBIAN/control

echo Creating postinst script...
cat > $BUILDDIR/DEBIAN/postinst <<EOF
#!/bin/bash

set -e

exit 0
 EOF
EOF

chmod 755 $BUILDDIR/DEBIAN/postinst

echo Creating prerm script...
cat > $BUILDDIR/DEBIAN/prerm <<EOF
#!/bin/bash

set -e

exit 0
 EOF
EOF
chmod 755 $BUILDDIR/DEBIAN/prerm

echo Building Package...
fakeroot dpkg-deb --build $BUILDDIR $OUTDIR/$PKGNAME


if [ ! -f $OUTDIR/$PKGNAME ];then
    echo Something went wrong!
    exit 1
fi
