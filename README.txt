                      ==================================
                                wxMEdit v2.9.7.2
                      ==================================
About:
-----
wxMEdit is a cross-platform Text/Hex Editor written in C++ & wxWidgets.
wxMEdit a fork of MadEdit which has been discontinued.
wxMEdit supports many useful functions, e.g. Bookmark, Syntax Highlightings, 
Word Wraps, Encodings, Column/Hex Modes, Updates checking, and a 
Plugin system(not work yet).
In HexMode, wxMEdit can open large files which size is up to 32GB (INT_MAX*16).


Supported Platforms:
-------------------
A. Linux, FreeBSD, Mac OS X, and other Unix-like OS (__WXGTK__):
   a. GNU C++ 3.x/4.x OR clang++ 2.7 or later:
      Required Tools and Libraries to compile:
      1. autoconf 2.59 or higher
      2. automake 1.10 or higher
      3. wxWidgets 2.8 with Unicode enabled
      4. Boost 1.33.0 or higher
      5. ICU4C 4.2 or higher
      6. libcurl 7.15.5 or later

B. MS Windows (__WXMSW__):
   a. MinGW32/GNU C++ 3.x/4.x:
      Required Tools and Libraries to compile:
      1. Bakefile 0.2.9
      2. wxWidgets 2.6/2.8 with Unicode enabled
      3. Boost 1.33.0 or higher
      4. libunicows: it's optional but required under Win98
      5. ICU4C 4.2 or higher
      6. libcurl 7.15.5 or later
   b. Visual C++ 2005/2008/2010/2012(by upgrading from 2008 solution and project files):
      Required Tools and Libraries to compile:
      1. Bakefile 0.2.9
      2. wxWidgets 2.6/2.8 with Unicode enabled
      3. Boost 1.33.0 or higher
      4. libunicows: it's optional but required under Win98
      5. ICU4C 4.2 or higher
      6. libcurl 7.15.5 or later

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
wxMEdit v2.9.7.2:
  1. made manual update checking asynchronously as same as automatic update checking.
  2. made default window size bigger.
  3. added more choices for whether pasting as hex-string in hex-area when editing in hex-mode.
  4. added new feature: Paste with Overwriting in Hex Area.
  5. updated FreeBSD packaging that switched to pkgng via ports.
  6. updated Chinese translations.
  7. other minor changes and bug fixes.

wxMEdit v2.9.7:
  1. added new encoding "CP437 for ASCII-Art" to support ASCII-Art better.
  2. added automatically checking for updates.
  3. added bookmark support [mainly by Gospodin Gyurov (aka Gogo) <ggurov@yahoo.com>].
  4. added new commands: delete previous/next word (by Gogo).
  5. added search option "Wrap around".
  6. added Credits tab in About Dialog.
  7. added Misc tab in Options Dialog.
  8. added selecting a line by triple click (mainly by cfreeer <cfreeer@users.sf.net>).
  9. added updating syntax highlighting on saving file (mainly by cfreeer).
 10. added FreeBASIC syntax file.
 11. added grouping of encodings.
 12. added purging histories support.
 13. added more choices for copying in hex-area in Options Dialog.
 14. added Russian translation (by Shamil Bikineyev).
 15. improved maximized behavior under Windows (by Gogo).
 16. improved system integration under Windows.
 17. improved display effect of the Word-Count information.
 18. changed behavior of selecting next word as other editors.
 19. changed notification messages of Search/Replace Dialog showing in the bottom bar of the dialog.
 20. fixed always not wrap around when replacing.
 21. fixed that the files "a b" and "aÀb" cannot be opened simultaneously under Windows Chinese Edition.
 22. fixed assertion failed with debug version of MadEdit/wxMEdit when mouse capture lost.
 23. fixed incorrect behavior when switch out from wxMEdit/MadEit on selecting text.
 24. fixed incorrect behavior when update an opened file and then click the editing area.
 25. updated Simplified Chinese, Japanese, Spanish translations.
 26. other minor changes and bug fixes.

wxMEdit v2.9.6
  1. improved Mac OS X support.
  2. redesigned all dialogs with wxSmith and fixed dialog size problems.
  3. refactored Encoding Detector and fixed encoding detection problems.
  4. fixed hex mode repaint problem under windows.
  5. fixed crash under Unix like OS when open Find In Files Result panel if the caret was in top several lines of text editing area.
  6. fixed wxWidgets error when remove file association.
  7. updated RPM, debian and FreeBSD packaging files.
  8. fixed crash on restart if "Max line length before Line-Wrap" was set too large.
  9. improved build configurations.
 10. updated Spanish translation by Carlos Sánchez <carlos1994sanchez@gmail.com>.
 11. updated Japanese translation by Tilt <tiltstr@gmail.com>.
 12. updated Simplified Chinese translation.
 13. updated Traditional Chinese translation by Denir Li <denir.li@gmail.com>.
 14. other minor changes.

See file ChangeLog for more changes.


License:
-------
wxMEdit is released under the GNU General Public License Version 3
or (at your option) any later version.


Authors:
------
JiaYanwei <wxmedit@gmail.com>        (current maintainer of wxMEdit)
Alston Chen <madedit@gmail.com>      (creator of MadEdit)


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
cURL: http://curl.haxx.se/
