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

SEDCMD_FILE=/dev/null
grep -wE "SUSE|openSUSE" /etc/os-release > /dev/null && SEDCMD_FILE=suse_deps.sed
cat wxmedit.spec._ changelog | sed -e "$SEDCMD_VER" -f "$SEDCMD_FILE" > wxmedit.spec

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
rpmbuild $RPMBUILDOPT --define "_topdir $HOME/rpmbuild" -bb "$SRCPACKDIR/packaging/rpm/wxmedit.spec"
