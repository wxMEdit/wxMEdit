#!/bin/sh

(
  cd /usr/ports/editors/
  [ -e wxmedit_bak ] && sudo rm -rf wxmedit_bak
  [ -e wxmedit ] && sudo mv -f wxmedit wxmedit_bak
  sudo mkdir wxmedit
)

sudo cp -f Makefile pkg-descr pkg-plist /usr/ports/editors/wxmedit/

cd /usr/ports/editors/wxmedit
sudo make makesum
sudo make package
