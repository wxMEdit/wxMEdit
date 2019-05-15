rem set MGW_BOOST_DIR=
rem set MGW_ICU_DIR=
rem set MGW_WXWIN=
rem set MGW_BOOST_ADD=
rem set MGW_WXAUI_DIR=
rem set MGW_WXVER=28
rem set MGW_MGW_VER=48
rem set MGW_MSLU=
rem set MGW_CURL_DIR=
bakefile -D BOOST_DIR="%MGW_BOOST_DIR%" -D ICU_DIR="%MGW_ICU_DIR%" -D WXWIN="%MGW_WXWIN%" -D BOOST_ADD="%MGW_BOOST_ADD%" -D WXAUI_DIR="%MGW_WXAUI_DIR%" -D WXVER="%MGW_WXVER%" -D MGW_VER="%MGW_MGW_VER%" -D MSLU="%MGW_MSLU%" -D CURL_DIR="%MGW_CURL_DIR%" -f mingw wxmedit.bkl -o ../mingw/GNUmakefile
