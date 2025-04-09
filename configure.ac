#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.63)
AC_INIT([wxmedit], [3.2], [wxmedit@gmail.com])
AC_CONFIG_SRCDIR([src/wxmedit_app.cpp])
AC_CONFIG_HEADER([config.h])
AC_PREFIX_DEFAULT([/usr])

AM_INIT_AUTOMAKE([subdir-objects])

AC_USE_SYSTEM_EXTENSIONS
ALL_LINGUAS="de_DE es fr_FR it_IT ja_JP pl_PL ru_RU ta zh_CN zh_TW"
AM_GNU_GETTEXT([external])
AC_SUBST(ALL_LINGUAS)

# Checks for programs.
AC_PROG_CXX
AC_PROG_INSTALL
AC_LANG(C++)

#CXXFLAGS="-O2"
my_cxxflags_bak="$CXXFLAGS"
CXXFLAGS="$CXXFLAGS -std=c++17"
AC_MSG_CHECKING([whether -std=c++17 is supported])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])],
	[AC_MSG_RESULT([yes])]
	[my_cxx17_suported="x"],
	[AC_MSG_RESULT([no])]
	[CXXFLAGS="$my_cxxflags_bak"]
)

if test "x$my_cxx17_suported" == "x"; then
my_cxxflags_bak="$CXXFLAGS"
CXXFLAGS="$CXXFLAGS -std=c++11"
AC_MSG_CHECKING([whether -std=c++11 is supported])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])],
	[AC_MSG_RESULT([yes])],
	[AC_MSG_RESULT([no])]
	[CXXFLAGS="$my_cxxflags_bak"]
	[AC_MSG_ERROR([C++11 support is required])]
)
fi


# Checks for MacOS
IS_MACOS=
case "${host}" in
    *-*-darwin* | powerpc-apple-macos* )
        IS_MACOS=1
    ;;
esac

PKG_CHECK_MODULES(curl, [libcurl >= 7.15.5])

PKG_CHECK_MODULES(icu, [icu-i18n >= 50])
CPPFLAGS="$CPPFLAGS $icu_CFLAGS"
LIBS="$LIBS $icu_LIBS"


# option for additional Boost lib
AC_ARG_WITH(boost, 
    [  --with-boost=PATH       use additional Boost library path,
                          otherwise use system include path],
    [CXXFLAGS="$CXXFLAGS -I$withval"])


# Checks for wxWidgets library
WX_LIBS_set="0"
if test "x$WX_LIBS" != "x"; then
    WX_LIBS_set="1"
fi

AM_OPTIONS_WXCONFIG
WX_CONFIG_CHECK(2.8.0, wxWin=1, ,[core,aui])
if test "$wxWin" != 1; then
    AC_MSG_ERROR([
            wxWidgets must be installed on your system
            but wx-config script couldn''t be found.

            Please check that wx-config is in path, the directory
            where wxWidgets libraries are installed (returned by
            'wx-config --libs' command) is in LD_LIBRARY_PATH or
            equivalent variable and wxWidgets version is 2.8.0.
    ])
fi
CPPFLAGS="$CPPFLAGS $WX_CPPFLAGS"
CXXFLAGS="$CXXFLAGS $WX_CXXFLAGS_ONLY"
#CFLAGS="$CFLAGS $WX_CFLAGS_ONLY"
if test "$WX_LIBS_set" != 1; then
    LIBS="$LIBS $WX_LIBS"
fi

UNICODE_FLAG=$(test ! $($WX_CONFIG_WITH_ARGS --unicode=yes >/dev/null 2>&1))

AC_MSG_CHECKING([if wxWidgets was built with UNICODE enabled])
if test "$UNICODE_FLAG" != "0"; then
    AC_MSG_RESULT([yes])
else
    AC_MSG_RESULT([no])
    AC_MSG_ERROR([wxWidgets must be built with UNICODE enabled.])
fi


# Checks for glib2, gtk2/3 libraries
WX_BASENAME=$($WX_CONFIG_WITH_ARGS --basename)
if test "$WX_BASENAME" = "wx_gtk2u"; then
    PKG_CHECK_MODULES(glib_gtk, [
            glib-2.0 >= 2.28.0
            gtk+-2.0 >= 2.24.0
    ], [
        AC_MSG_RESULT([... GTK+ 2.x])
    ])
else
    PKG_CHECK_MODULES(glib_gtk, [
            glib-2.0 >= 2.28.0
            gtk+-3.0 >= 3.0.0
    ], [
        AC_MSG_RESULT([... GTK+ 3.x])
    ])
fi
CPPFLAGS="$CPPFLAGS $glib_gtk_CFLAGS"
LIBS="$LIBS $glib_gtk_LIBS"

WX_RELEASE=$($WX_CONFIG_WITH_ARGS --release)
if test "$WX_RELEASE" = "2.8"; then
    LIBS="$LIBS -lX11"
fi


# Checks for header files.
AC_CHECK_HEADERS([boost/config.hpp], [], AC_MSG_ERROR([cannot find required header boost/config.hpp]))
AC_CHECK_HEADERS([boost/xpressive/xpressive_dynamic.hpp], [], AC_MSG_ERROR([cannot find required header boost/xpressive/xpressive_dynamic.hpp]))
AC_CHECK_HEADERS([boost/foreach.hpp], [], AC_MSG_ERROR([cannot find required header boost/foreach.hpp]))
AC_CHECK_HEADERS([unicode/uchar.h], [], AC_MSG_ERROR([cannot find required header unicode/uchar.h]))
AC_CHECK_HEADERS([unicode/unistr.h], [], AC_MSG_ERROR([cannot find required header unicode/unistr.h]))
AC_CHECK_HEADERS([wx/aui/aui.h], [], AC_MSG_ERROR([cannot find required header wx/aui/aui.h]))

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
