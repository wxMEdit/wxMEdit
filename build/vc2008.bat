rem set VC9_BOOST_DIR=
rem set VC9_ICU_DIR=
rem set VC9_WXWIN=
rem set VC9_XPRESSIVE_INC=
rem set VC9_AUI_INC=
rem set VC9_AUI_LIB=
rem set VC9_ICUDEBUG=
rem set VC9_WXVER
bakefile -D BOOST_DIR="%VC9_BOOST_DIR%" -D ICU_DIR="%VC9_ICU_DIR%" -D WXWIN="%VC9_WXWIN%" -D XPRESSIVE_INC="%VC9_XPRESSIVE_INC%" -D AUI_INC="%VC9_AUI_INC%" -D AUI_LIB="%VC9_AUI_LIB%" -D ICUDEBUG="%VC9_ICUDEBUG%" -D WXVER="%VC9_WXVER%" -f msvs2008prj wxmedit.bkl -o ..\vc2008\wxmedit
