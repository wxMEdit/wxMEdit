rem set VC8_BOOST_DIR=
rem set VC8_ICU_DIR=
rem set VC8_WXWIN=
rem set VC8_XPRESSIVE_INC=
rem set VC8_AUI_INC=
rem set VC8_AUI_LIB=
rem set VC8_ICUDEBUG=
rem set VC8_WXVER=
bakefile -D BOOST_DIR="%VC8_BOOST_DIR%" -D ICU_DIR="%VC8_ICU_DIR%" -D WXWIN="%VC8_WXWIN%" -D XPRESSIVE_INC="%VC8_XPRESSIVE_INC%" -D AUI_INC="%VC8_AUI_INC%" -D AUI_LIB="%VC8_AUI_LIB%" -D ICUDEBUG="%VC8_ICUDEBUG%" -D WXVER="%VC8_WXVER%" -f msvs2005prj wxmedit.bkl -o ..\vc2005\wxmedit
