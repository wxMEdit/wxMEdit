#! /bin/sh
#./chmod_644_all_files.sh
aclocal
autoheader
automake --foreign --add-missing --copy
autoconf
