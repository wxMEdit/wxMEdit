#! /bin/sh
aclocal $ACLOCAL_AMFLAGS
autoheader
automake --foreign --add-missing --copy
autoconf
