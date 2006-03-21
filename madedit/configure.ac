#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([madedit], [0.2.4], [madedit@gmail.com])
AC_CONFIG_SRCDIR([src/MadEditApp.cpp])
AC_CONFIG_HEADER([config.h])
AC_PREFIX_DEFAULT([/usr])

AM_INIT_AUTOMAKE

ALL_LINGUAS="zh_CN zh_TW"
AM_GNU_GETTEXT
AC_SUBST(ALL_LINGUAS)

# Checks for programs.
AC_PROG_CXX
AC_PROG_INSTALL
AC_LANG(C++)

#CXXFLAGS="-O2"

# Checks for MacOS
IS_MACOS=
case "${host}" in
    *-*-darwin* | powerpc-apple-macos* )
        IS_MACOS=1
    ;;
esac

# Checks for glib2, gtk2 libraries
#AM_PATH_GLIB_2_0(2.6.0)
#AM_PATH_GTK_2_0(2.6.0)

if test "x$IS_MACOS" == "x"; then
    PKG_CHECK_MODULES(glib_gtk2, [
            glib-2.0 >= 2.6.0
            gtk+-2.0 >= 2.6.0
    ])
    CXXFLAGS="$CXXFLAGS $glib_gtk2_CFLAGS"
    LIBS="$LIBS $glib_gtk2_LIBS"
fi

# option for additional Boost lib
AC_ARG_WITH(boost, 
    [  --with-boost=PATH       use additional Boost library path,
                          otherwise use system include path],
    [CXXFLAGS="$CXXFLAGS -I$withval"])

# option for additional Xpressive lib
AC_ARG_WITH(xpressive, 
    [  --with-xpressive=PATH   use additional Boost.Xpressive library path,
                          otherwise use default path "$(top_srcdir)/xpressive"],
    [CXXFLAGS="$CXXFLAGS -I$withval"],
    [CXXFLAGS="$CXXFLAGS -I\$(top_srcdir)/xpressive"])


# Checks for wxWidgets library
WX_LIBS_set="0"
if test "x$WX_LIBS" != "x"; then
    WX_LIBS_set="1"
fi

AM_OPTIONS_WXCONFIG
AM_PATH_WXCONFIG(2.6.0, wxWin=1)
if test "$wxWin" != 1; then
   AC_MSG_ERROR([
           wxWidgets must be installed on your system
           but wx-config script couldn't be found.

           Please check that wx-config is in path, the directory
           where wxWidgets libraries are installed (returned by
           'wx-config --libs' command) is in LD_LIBRARY_PATH or
           equivalent variable and wxWidgets version is 2.6.0 or above.
   ])
fi
CPPFLAGS="$CPPFLAGS $WX_CPPFLAGS"
CXXFLAGS="$CXXFLAGS $WX_CXXFLAGS_ONLY"
#CFLAGS="$CFLAGS $WX_CFLAGS_ONLY"
if test "$WX_LIBS_set" != 1; then
    LIBS="$LIBS $WX_LIBS"
fi

UNICODE_FLAG=$(test ! $($WX_CONFIG_WITH_ARGS - -unicode >/dev/null 2>&1))

AC_MSG_CHECKING([if wxWidgets was built with UNICODE enabled])
if test "$UNICODE_FLAG" != "0"; then
    AC_MSG_RESULT([yes])
else
    AC_MSG_RESULT([no])
    AC_MSG_ERROR([wxWidgets must be built with UNICODE enabled.])
fi



# Checks for header files.
AC_CHECK_HEADERS([boost/config.hpp])

# Checks for typedefs, structures, and compiler characteristics.
#AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
#AC_FUNC_MALLOC
#AC_HEADER_STDC
AC_CHECK_FUNCS([dup2])

AC_CONFIG_FILES([Makefile syntax/Makefile po/Makefile.in])

AC_OUTPUT
