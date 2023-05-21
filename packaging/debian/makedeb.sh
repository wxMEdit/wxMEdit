#!/bin/sh

export LC_ALL=C

SRCBASEDIR="../.."

VERBASE=$(sed -n '3s/^.*v//;3s/://p' "$SRCBASEDIR/ChangeLog")

SRCPARENTDIR="$SRCBASEDIR/.."
SRCVERDIR="$SRCPARENTDIR/wxmedit-$VERBASE"

mkdir -p "$SRCVERDIR"
/bin/cp -Rf "$SRCBASEDIR/"* "$SRCVERDIR"

DEBIAN_DIR="$SRCVERDIR/debian"
DEBSOURCE_DIR="$DEBIAN_DIR/source"

mkdir -p "$DEBSOURCE_DIR"

gawk -f changelog_deb.awk ../../ChangeLog > "$DEBIAN_DIR/changelog"
cp control copyright menu rules "$DEBIAN_DIR"
echo '3.0 (native)' > "$DEBSOURCE_DIR/format"
echo 10 > "$DEBIAN_DIR/compat"

cd "$SRCVERDIR" || exit 1
touch aclocal.m4 configure Makefile.in config.h.in
dpkg-buildpackage
