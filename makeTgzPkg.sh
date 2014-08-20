#!/bin/bash

DIR=$(dirname $(readlink -f $0))

if [ `arch` == 'x86_64' ]; then
ARCH=x86_64
else
ARCH=i386
fi

VERSION=`cat "$DIR/VERSION"`


BUILDDIR=$DIR/build/bin/diveboard-agent-$VERSION-$ARCH
OUTDIR=$DIR/build/packages
SHAREDIR=$DIR/build/bin/share

LIBDIVE=$DIR/build/bin/libdivecomputer.so
APPDIVEBOARD=$DIR/build/bin/DiveboardAgent

PKGNAME=diveboard-plugin-$VERSION-$ARCH.tgz

#
# Put the package together
#

if [ ! -f $APPDIVEBOARD ]; then
    echo Unable to find DiveBoard - did you build it?
    echo MISSING: $APPDIVEDBOARD
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
mkdir -p $BUILDDIR/usr/bin

echo Copying package contents...
cp $APPDIVEBOARD $BUILDDIR/usr/bin/DiveboardAgent
cp $LIBDIVE $BUILDDIR/usr/lib/diveboard/libdivecomputer.so
cp -r $SHAREDIR $BUILDDIR/usr/

echo Stripping binaries...
strip $BUILDDIR/usr/bin/DiveboardAgent
strip $BUILDDIR/usr/lib/diveboard/*.so

echo Fixing permissions...
chmod 755 $BUILDDIR/usr/bin/DiveboardAgent
chmod 644 $BUILDDIR/usr/lib/diveboard/*.so
#chmod -R 644 $BUILDDIR/usr/share/*

echo Creating install script...
cat > "$BUILDDIR/install.sh" <<EOF
#!/bin/bash

set -e

SELF=\$(which "\$0")
BASEDIR=\$(dirname "\$SELF")

if [ ! -w /usr/lib ]; then
    echo "You need to be root to install the plugin"
    exit 1
fi

cp -a "\$BASEDIR/usr" /
exit 0
 EOF
EOF
chmod 755 $BUILDDIR/install.sh

echo Creating uninstall script...
cat > $BUILDDIR/uninstall.sh <<EOF
#!/bin/bash

set -e

if [ ! -d /usr/lib/diveboard ]; then
    echo "The plugin for Diveboard does not seem to be installed. Aborting uninstallation."
    exit 2
fi

if [ ! -w /usr/lib ]; then
    echo "You need to be root to uninstall the plugin"
    exit 1
fi


rm -fr /usr/lib/diveboard
rm -fr /usr/bin/DiveboardAgent

exit 0
 EOF
EOF
chmod 755 $BUILDDIR/uninstall.sh

echo Building Package...
cd $BUILDDIR/..
tar zcf $OUTDIR/$PKGNAME --owner=root --group=root $(basename "$BUILDDIR")

if [ ! -f $OUTDIR/$PKGNAME ];then
    echo Something went wrong!
    exit 1
fi
