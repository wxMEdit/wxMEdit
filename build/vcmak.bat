rem set VC_BOOST_DIR=
rem set VC_ICU_DIR=
rem set VC_WXWIN=
rem set VC_BOOST_ADD=
rem set VC_WXAUI_DIR=
rem set VC_ICUDEBUG=
rem set VC_WXVER=
bakefile -D BOOST_DIR="%VC_BOOST_DIR%" -D ICU_DIR="%VC_ICU_DIR%" -D WXWIN="%VC_WXWIN%" -D BOOST_ADD="%VC_BOOST_ADD%" -D WXAUI_DIR="%VC_WXAUI_DIR%" -D ICUDEBUG="%VC_ICUDEBUG%" -D WXVER="%VC_WXVER%" -f msvc wxmedit.bkl -o ..\msvc\Makefile
