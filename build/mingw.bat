rem set MGW_BOOST_DIR=
rem set MGW_ICU_DIR=
rem set MGW_WXWIN=
rem set MGW_XPRESSIVE_INC=
rem set MGW_AUI_INC=
rem set MGW_AUI_LIB=
rem set MGW_WXVER=28
bakefile -D BOOST_DIR="%MGW_BOOST_DIR%" -D ICU_DIR="%MGW_ICU_DIR%" -D WXWIN="%MGW_WXWIN%" -D XPRESSIVE_INC="%MGW_XPRESSIVE_INC%" -D AUI_INC="%MGW_AUI_INC%" -D AUI_LIB="%MGW_AUI_LIB%" -D WXVER="%MGW_WXVER%" -f mingw wxmedit.bkl -o ../mingw/GNUmakefile
