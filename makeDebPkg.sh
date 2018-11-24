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
Depends: libc6 (>= 2.11), libconfig9, libcurl3|libcurl4, libjpeg8, libpng12-0, libssl1.0.0, libtiff5, libwebkitgtk-1.0-0, zlib1g (>= 1.1.4)
Replaces: diveboard (<= 1.1.1)
Maintainer: Diveboard <support@diveboard.com>
Installed-Size: $INSTALLED_SIZE
Description: Dive Computer Agent for DiveBoard:
 http://www.diveboard.com
 .
 With the DiveBoard-Agent program you can upload your dive
 profiles direct from a supported dive computer to the web.
 This plugin links with Jef Driesen's LibDiveComputer
 http://www.libdivecomputer.org/

"

CHANGELOG="diveboard-agent ($VERSION) xenial; urgency=medium

  * diveboard-agent version $VERSION
    for more information and a detailed changelog
    see the diveboard-agent repo on github

 -- Thomas Horner <diveboard-agent-changelog@t-horner.com>  Fri, 20 Apr 2018 18:00:00 +0200

"

COPYRIGHT="Format: http://www.debian.org/doc/packaging-manuals/copyright-format/1.0/
Upstream-Name: diveboard-agent
Source: diveboard

Files: *
Copyright: Copyright 2018 TBD <tbd@diveboard.com>
License: TBD
 TBD

"

MANPAGE=".\\\"Manpages for DiveboardAgent
.\\\"http://sourceforge.net/projects/gmanedit2
.TH man 1 \"April 20, 2018\" \"0.1\" \"DiveboardAgent\"

.SH NAME
DiveboardAgent \\- Dive Computer Agent for DiveBoard: http://www.diveboard.com

.SH SYNOPSIS
.B DiveboardAgent
.RI [ -h ] [ --help ] [ --verbose ]
.br

.SH DESCRIPTION
With the
.B DiveboardAgent
program you can upload your dive
profiles direct from a supported dive computer to the web.
This plugin links with Jef Driesen's LibDiveComputer
http://www.libdivecomputer.org/

.SH OPTIONS
.B
.IP -h
shows a help message
.B
.IP --help
shows a help message
.B
.IP --verbose
generate verbose log messages

.SH BUGS
For the list of known bugs see the diveboard-agent repo on github.

.SH \"SEE ALSO\"
For more information see the diveboard-agent repo on github.

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

echo Creating required debian files
mkdir -p $BUILDDIR/usr/share/doc/diveboard-agent
echo -e "$CHANGELOG" |gzip -n -9 > $BUILDDIR/usr/share/doc/diveboard-agent/changelog.gz
echo -e "$COPYRIGHT" > $BUILDDIR/usr/share/doc/diveboard-agent/copyright
mkdir -p $BUILDDIR/usr/share/man/man1
echo -e "$MANPAGE" |gzip -n -9 > $BUILDDIR/usr/share/man/man1/DiveboardAgent.1.gz

echo Fixing permissions...
find $BUILDDIR/usr -type d -exec chmod 755 {} \;
find $BUILDDIR/usr -type f -exec chmod 644 {} \;
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
