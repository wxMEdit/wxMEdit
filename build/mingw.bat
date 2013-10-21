rem set MGW_BOOST_DIR=
rem set MGW_ICU_DIR=
rem set MGW_WXWIN=
rem set MGW_BOOST_ADD=
rem set MGW_WXAUI_DIR=
rem set MGW_WXVER=28
bakefile -D BOOST_DIR="%MGW_BOOST_DIR%" -D ICU_DIR="%MGW_ICU_DIR%" -D WXWIN="%MGW_WXWIN%" -D BOOST_ADD="%MGW_BOOST_ADD%" -D WXAUI_DIR="%MGW_WXAUI_DIR%" -D WXVER="%MGW_WXVER%" -f mingw wxmedit.bkl -o ../mingw/GNUmakefile
