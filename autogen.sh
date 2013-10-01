#! /bin/sh
#./chmod_644_all_files.sh
aclocal $ACLOCAL_FLAGS
autoheader
automake --foreign --add-missing --copy
autoconf
