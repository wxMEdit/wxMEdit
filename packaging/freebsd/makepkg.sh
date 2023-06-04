#!/bin/sh

(
  cd /usr/ports/editors/ || exit 1
  [ -e wxmedit_bak ] && sudo rm -rf wxmedit_bak
  [ -e wxmedit ] && sudo mv -f wxmedit wxmedit_bak
  sudo mkdir wxmedit
)

TMPFILE="/tmp/wxMEdit-Makefile-$$"
VERBASE=$(sed -n '3s/^.*v//;3s/://p' ../../ChangeLog)
VERDOTS=$(echo "$VERBASE" | sed 's/[^.]//g')
PREREL_SEDCMD=''
# shellcheck disable=SC2016
[ "$VERDOTS" = "..." ] && PREREL_SEDCMD='/^MASTER_SITES=/s#^.*$#MASTER_SITES=	SF/${PORTNAME}/prerelease/${PORTVERSION}#'
sed -e 's#libboost_system.so:devel/boost-libs##' -e "s/@VER/$VERBASE/" -e "$PREREL_SEDCMD" Makefile > "$TMPFILE"
sudo cp -f pkg-descr pkg-plist /usr/ports/editors/wxmedit/
sudo mv -f "$TMPFILE" /usr/ports/editors/wxmedit/Makefile

cd /usr/ports/editors/wxmedit || exit 2
sudo make makesum
sudo make package
