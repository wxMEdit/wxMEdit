#!/bin/sh

(
  cd /usr/ports/editors/
  [ -e wxmedit_bak ] && sudo rm -rf wxmedit_bak
  [ -e wxmedit ] && sudo mv -f wxmedit wxmedit_bak
  sudo mkdir wxmedit
)

sudo cp -f Makefile pkg-descr /usr/ports/editors/wxmedit/

sudo sh -c "sed -e 's#share/doc/wxmedit#%%DOCSDIR%%#' \
                -e 's#share/wxmedit#%%DATADIR%%#' \
                -e 's/@dirrm /@dirrmtry /' pkg-plist_old > \
                /usr/ports/editors/wxmedit/pkg-plist"

cd /usr/ports/editors/wxmedit
sudo make makesum
sudo make package
