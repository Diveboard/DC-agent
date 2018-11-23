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

SPECFILE=/tmp/diveboard.spec

cat > "$SPECFILE" <<EOF
Name: diveboard-agent
Version: $VERSION
Release: 1%{?dist}
Group: Applications/System
Summary: diveboard.com browser plugin for Linux
License: Unknown
URL: http://labs.vmware.com/flings/ioblazer
Source0: http://www.diveboard.com/plugin/latest/linux/diveboard.tgz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root

%description
Explore scuba diving, log and share your experiences.

www.diveboard.com

%prep
%ifarch x86_64
%setup -n diveboard-agent-$VERSION-x86_64
%else
%setup -n diveboard-agent-$VERSION-i386
%endif

%install
%{__rm} -rf %{buildroot}
%{__mkdir_p} %{buildroot}/usr/bin/
%{__mkdir_p} %{buildroot}/usr/lib/diveboard/
%{__mkdir_p} %{buildroot}/usr/share
%{__install} -m 0755 usr/bin/DiveboardAgent %{buildroot}/usr/bin/
%{__install} -m 0755 usr/lib/diveboard/libdivecomputer.so %{buildroot}/usr/lib/diveboard/
%{__cp} -r usr/share/* %{buildroot}/usr/share/ 

%clean
%{__rm} -rf %{buildroot}

%files
%defattr(-, root, root, 0755)
/usr/lib/*
/usr/bin/DiveboardAgent
/usr/share/*

%changelog
* Fri Sep 30 2011 Pascal Manchon <pascal@diveboard.com> - 1.1.0-alpha2
- memory and cpu optimisations
* Fri Sep 16 2011 Sergio Rubio <rubiojr@frameos.org> - 1.1.0alpha1-1
- Initial Release

 EOF
EOF

mkdir -p ~/rpmbuild/SOURCES
cp $DIR/build/packages/diveboard-agent-$VERSION-$ARCH.tgz ~/rpmbuild/SOURCES/diveboard.tgz

rpmbuild --clean "$SPECFILE"

rpmbuild -bp "$SPECFILE"
rpmbuild -bc --short-circuit "$SPECFILE"
rpmbuild -bi --short-circuit "$SPECFILE"
rpmbuild -ba --short-circuit "$SPECFILE" --sign

mkdir -p $DIR/build/packages
cp ~/rpmbuild/RPMS/*/diveboard-agent-$VERSION*$ARCH.rpm $DIR/build/packages

rm -f "$SPECFILE"
