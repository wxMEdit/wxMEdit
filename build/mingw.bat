rem set BOOST_DIR=
rem set ICU_DIR=
rem set WXWIN=
rem set XPRESSIVE_INC=
rem set AUI_INC=
rem set AUI_LIB=
bakefile -D BOOST_DIR=%BOOST_DIR% -D ICU_DIR=%ICU_DIR% -D WXWIN=%WXWIN% -D XPRESSIVE_INC=%XPRESSIVE_INC% -D AUI_INC=%AUI_INC% -D AUI_LIB=%AUI_LIB% -f mingw wxmedit.bkl -o ../mingw/GNUmakefile
