rem set VC8_BOOST_DIR=
rem set VC8_ICU_DIR=
rem set VC8_WXWIN=
rem set VC8_BOOST_ADD=
rem set VC8_WXAUI_DIR=
rem set VC8_ICUDEBUG=
rem set VC8_WXVER=
bakefile -D BOOST_DIR="%VC8_BOOST_DIR%" -D ICU_DIR="%VC8_ICU_DIR%" -D WXWIN="%VC8_WXWIN%" -D BOOST_ADD="%VC8_BOOST_ADD%" -D WXAUI_DIR="%VC8_WXAUI_DIR%" -D ICUDEBUG="%VC8_ICUDEBUG%" -D WXVER="%VC8_WXVER%" -f msvs2005prj wxmedit.bkl -o ..\vc2005\wxmedit
