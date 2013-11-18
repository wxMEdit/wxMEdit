#!/bin/sh

PREFIX=/usr/local
if [ $# -ge 1 -a -d "$1"]
then
	PREFIX="$1"
fi

(
	cd ../..
	touch aclocal.m4 configure Makefile.in config.h.in
	./configure --prefix="$PREFIX" --with-wx-config=wxgtk2u-2.8-config --with-boost-lib-suffix=''
	make
	sudo make install
	DOCDIR="$PREFIX/share/doc/wxmedit"
	sudo mkdir -p "$DOCDIR"
	sudo cp ChangeLog LICENSE README.txt "$DOCDIR"
)

pkg_create -p "$PREFIX" -c -"wxMEdit: Cross-platform Text/Hex Editor, a fork of MadEdit" -d pkg-descr -f pkg-plist "wxmedit-2.9.5-2"
