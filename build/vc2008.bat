rem set VC9_BOOST_DIR=
rem set VC9_ICU_DIR=
rem set VC9_WXWIN=
rem set VC9_BOOST_ADD=
rem set VC9_WXAUI_DIR=
rem set VC9_ICUDEBUG=
rem set VC9_WXVER=28
rem set VC9_MSLU=
bakefile -D BOOST_DIR="%VC9_BOOST_DIR%" -D ICU_DIR="%VC9_ICU_DIR%" -D WXWIN="%VC9_WXWIN%" -D BOOST_ADD="%VC9_BOOST_ADD%" -D WXAUI_DIR="%VC9_WXAUI_DIR%" -D ICUDEBUG="%VC9_ICUDEBUG%" -D WXVER="%VC9_WXVER%" -D MSLU="%VC9_MSLU%" -f msvs2008prj wxmedit.bkl -o ..\vc2008\wxmedit
