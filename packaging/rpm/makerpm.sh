#!/bin/sh

#======================================================================
# 1. copy files
#----------------------------------------------------------------------

export LC_ALL=C

SRCBASEDIR="../.."

ver=`sed -n '3s/^.*v//;3s/://p' "$SRCBASEDIR/ChangeLog"`
echo "$ver" | grep '-' > /dev/null
[ $? -eq 0 ] || ver="$ver"-1

verbase=`echo "$ver" | sed 's/-.*$//'`
release=`echo "$ver" | sed 's/^.*-//'`

sedcmd_ver="s/@VER@/$verbase/;s/@REL@/$release/"

SRCPARENTDIR="$SRCBASEDIR/.."
SRCVERDIR="$SRCPARENTDIR/wxmedit-$verbase"

mkdir -p "$SRCVERDIR"
/bin/cp -Rf "$SRCBASEDIR/"* "$SRCVERDIR"

#======================================================================
# 2. generate .spec file
#----------------------------------------------------------------------

cd "$SRCVERDIR/packaging/rpm"

NEW_SRCBASEDIR="../.."
NEW_SRCPARENTDIR="$NEW_SRCBASEDIR/.."

gawk -f changelog_rpm.awk "$NEW_SRCBASEDIR/ChangeLog" > changelog

cat wxmedit.spec._ changelog | sed "$sedcmd_ver" > wxmedit.spec

#======================================================================
# 3. create .tar.gz
#----------------------------------------------------------------------

cd "$NEW_SRCPARENTDIR"

SRCPACKDIR="wxmedit-$verbase"
SRCPACK="$SRCPACKDIR.tar.gz"

tar -czvf "$SRCPACK" "$SRCPACKDIR"

#======================================================================
# 4. build rpm
#----------------------------------------------------------------------

mv "$SRCPACK" ~/rpmbuild/SOURCES/

RPMBUILDOPT=""
uname -r | grep 'fc[0-9]\+\.i686' > /dev/null && RPMBUILDOPT="--target=i686"
rpmbuild $RPMBUILDOPT -bb "$SRCPACKDIR/packaging/rpm/wxmedit.spec"

