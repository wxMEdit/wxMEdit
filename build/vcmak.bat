rem set VC_BOOST_DIR=
rem set VC_ICU_DIR=
rem set VC_WXWIN=
rem set VC_XPRESSIVE_INC=
rem set VC_AUI_INC=
rem set VC_AUI_LIB=
rem set VC_ICUDEBUG=
rem set VC_WXVER=
bakefile -D BOOST_DIR="%VC_BOOST_DIR%" -D ICU_DIR="%VC_ICU_DIR%" -D WXWIN="%VC_WXWIN%" -D XPRESSIVE_INC="%VC_XPRESSIVE_INC%" -D AUI_INC="%VC_AUI_INC%" -D AUI_LIB="%VC_AUI_LIB%" -D ICUDEBUG="%VC_ICUDEBUG%" -D WXVER="%VC_WXVER%" -f msvc wxmedit.bkl -o ..\msvc\Makefile
