#!/bin/sh

#======================================================================
# 1. copy files
#----------------------------------------------------------------------

export LC_ALL=C

SRCBASEDIR="../.."

VERBASE=$(sed -n '3s/^.*v//;3s/://p' "$SRCBASEDIR/ChangeLog")
RELEASE=1

SEDCMD_VER="s/@VER@/$VERBASE/;s/@REL@/$RELEASE/"

SRCPARENTDIR="$SRCBASEDIR/.."
SRCVERDIR="$SRCPARENTDIR/wxmedit-$VERBASE"

mkdir -p "$SRCVERDIR"
/bin/cp -Rf "$SRCBASEDIR/"* "$SRCVERDIR"

#======================================================================
# 2. generate .spec file
#----------------------------------------------------------------------

cd "$SRCVERDIR/packaging/rpm" || exit 2

NEW_SRCBASEDIR="../.."
NEW_SRCPARENTDIR="$NEW_SRCBASEDIR/.."

gawk -f changelog_rpm.awk "$NEW_SRCBASEDIR/ChangeLog" > changelog

cat wxmedit.spec._ changelog | sed "$SEDCMD_VER" > wxmedit.spec

#======================================================================
# 3. create .tar.gz
#----------------------------------------------------------------------

cd "$NEW_SRCPARENTDIR" || exit 3

SRCPACKDIR="wxmedit-$VERBASE"
SRCPACK="$SRCPACKDIR.tar.gz"

tar -czvf "$SRCPACK" "$SRCPACKDIR"

#======================================================================
# 4. build rpm
#----------------------------------------------------------------------

mv "$SRCPACK" ~/rpmbuild/SOURCES/

RPMBUILDOPT=""
uname -r | grep 'fc[0-9]\+\.i686' > /dev/null && RPMBUILDOPT="--target=i686"
rpmbuild $RPMBUILDOPT -bb "$SRCPACKDIR/packaging/rpm/wxmedit.spec"
