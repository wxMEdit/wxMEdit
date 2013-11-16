#!/bin/sh

export LC_ALL=C

TOP_DIR="../.."
DEBIAN_DIR="$TOP_DIR/debian"
DEBSOURCE_DIR="$DEBIAN_DIR/source"
mkdir -p "$DEBSOURCE_DIR"
gawk -f changelog_deb.awk ../../ChangeLog > "$DEBIAN_DIR/changelog"
cp control copyright menu rules "$DEBIAN_DIR"
echo '1.0' > "$DEBSOURCE_DIR/format"
echo 8 > "$DEBIAN_DIR/compat"
cd "$TOP_DIR"
dpkg-buildpackage
