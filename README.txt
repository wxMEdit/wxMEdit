                      ==================================
                                wxMEdit v2.9.5-2+
                      ==================================
About:
-----
wxMEdit is a cross-platform Text/Hex Editor written in C++ & wxWidgets.
wxMEdit a fork of MadEdit which has been discontinued.
wxMEdit supports many useful functions, e.g. SyntaxHighlightings, WordWraps, 
Encodings, Column/Hex Modes, and a Plugin system(not work yet).
In HexMode, wxMEdit can open large files which size is up to 32GB (INT_MAX*16).


Supported Platforms:
-------------------
A. Linux, FreeBSD, Mac OS X, and other Unix-like OS (__WXGTK__):
   a. GNU C++ 3.x/4.x:
      Required Libraries and Tools to compile:
      1. wxWidgets-2.6.1 or higher with Unicode enabled
      2. Boost-1.33.0 or higher
      3. ICU4C 4.2 or higher
      4. autoconf 2.59 or higher
      5. automake 1.9 or higher

B. MS Windows (__WXMSW__):
   a. MinGW32/GNU C++ 3.x/4.x:
      Required Libraries and Tools to compile:
      1. wxWidgets-2.6.1 or higher with Unicode enabled
      2. Boost-1.33.0 or higher
      3. libunicows: it's optional under WinNT/XP, but required under Win98
      4. ICU4C 4.2 or higher
      5. Bakefile 0.2.9
   b. Visual C++ 2005/2008/2010/2012(by upgrading from 2008 solution and project files):
      Required Libraries and Tools to compile:
      1. wxWidgets-2.6.1 or higher with Unicode enabled
      2. Boost-1.33.0 or higher
      3. libunicows: it's optional under WinNT/XP, but required under Win98
      4. ICU4C 4.2 or higher
      5. Bakefile 0.2.9

NOTE:
   You can download Bakefile at:
      http://www.bakefile.org/download.html

   If you use Boost-1.33, you should get Boost.Xpressive and Boost.Foreach at:
      http://code.google.com/p/wxmedit/
      http://wxmedit.googlecode.com/files/xpressive_foreach.tgz
      http://wxmedit.googlecode.com/files/xpressive_foreach.zip

   If you use wxWidgets-2.6, you should get wxAUI:
      http://wxmedit.googlecode.com/files/wxAUI.tgz
      http://wxmedit.googlecode.com/files/wxAUI.zip

   The small tool image2xpm was removed. 
   You can turn to GIMP, XnView, IrfanView, etc. or get image2xpm at:
      http://wxmedit.googlecode.com/files/image2xpm.exe
      http://wxmedit.googlecode.com/files/image2xpm_src.zip

Syntax files, Locale files and Settings:
---------------------------------------
  Syntax files: in the path $(wxMEditConfigPath)/syntax/
  Locale files: in the path $(wxMEditConfigPath)/locale/
  
  $(wxMEditConfigPath) are the one of below paths:
    1. wxMEdit execution file directory : Windows and Linux
    2. $(HOME)/.wxmedit/ : Linux
    3. /usr/share/wxmedit/ : Linux
  
  If wxMEdit does not find any files in those paths, 
  wxMEdit can also execute standalone by using default settings.
  
  If you are under Win98, you must get unicows.dll from MS's website:
  http://www.microsoft.com/downloads/details.aspx?FamilyId=73BA7BD7-ED06-4F0D-80A4-2A7EEAEE17E2&displaylang=en
  
  When quit wxMEdit, wxMEdit will save the settings to:
    1. $(HOME)/.wxmedit/wxmedit.cfg: Linux
    2. $(wxMEdit execution file directory)/wxmedit.cfg: Windows
  And FontWidth.dat will also be saved to the same path.
  FontWidth.dat is Cache of Font-Width-Data, it can speed-up wxMEdit a lot.


ChangeLog:
----------
wxMEdit v2.9.5-2:
1.updated translations for "Printout" and Syntax Highlighting Settings.
2.updated Spanish translation.
3.added Spanish to language chooser of General Settings.

wxMEdit v2.9.5:
1.fixed crash when delete a character in a line which contains two or more characters with MadEdit/wxMEdit built with vc2010.
2.fixed Fedora build and packing by Huaren Zhong <huaren.zhong@gmail.com>.
3.fixed recent lists case sensitive and truncated in menu bug.
4.fixed Double-byte conversion bug.
5.changed EUC-JP implementation to meet CP20932.
6.added CP850, CP852, CP855, KOI8-U, ISO-8859-16, Windows-1258 and GB18030 support.
7.made more system encodings(e.g. Windows-1258) detectable.
8.added Spanish translation by Carlos Sánchez <carlos1994sanchez@gmail.com>.
9.updated Japanese translation by Tilt <tiltstr@gmail.com>.
10.updated Chinese translations.

wxMEdit v2.9.4:
1.changed Unicode escape to ICU code point format {U+XXXX}-{U+XXXXXX}.
2.introduced Bakefile building system and removed MSVC/MinGW building files.
3.refactored encoding implementation.
4.added test case for conversion between Unicode and Single-Byte Encodings.
5.fixed incorrect conversion between Unicode and Single-Byte Encodings.
6.added CP866 and KOI8-R support.

See file ChangeLog for more changes.


License:
-------
wxMEdit is released under the GNU General Public License Version 3.


Author:
------
Alston Chen <madedit@gmail.com>      (MadEdit creator)
JiaYanwei <wxmedit@gmail.com>        (wxMEdit current maintainer)

Links:
-----
wxMEdit: http://code.google.com/p/wxmedit/
MadEdit: http://sourceforge.net/projects/madedit/
wxWidgets: http://www.wxwidgets.org/
Boost: http://www.boost.org/
Boost.Xpressive: http://www.boost.org/doc/html/xpressive.html
libunicows: http://libunicows.sourceforge.net/
Open Clip Art Library: http://www.openclipart.org/
ICU: http://site.icu-project.org/
Bakefile: http://www.bakefile.org/
Code::Blocks: http://www.codeblocks.org/
wxSmith: http://wiki.codeblocks.org/index.php?title=WxSmith_plugin
