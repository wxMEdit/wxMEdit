			=================================
			  MadEdit v0.2.8 Beta
			=================================
About:
-----
MadEdit is a cross-platform Text/Hex Editor written in C++ & wxWidgets.
MadEdit supports many useful functions, e.g. SyntaxHighlightings, WordWraps, 
Encodings, Column/Hex Modes, and a Plugin system(not work yet).
In HexMode, MadEdit can open large files which size is up to 32GB (INT_MAX*16).


Supported Platforms:
-------------------
A. Linux, FreeBSD, and Unix-like OS (__WXGTK__):
   a. GNU C++ 3.x/4.x:
      Required Libraries to compile:
      1. wxWidgets-2.6.1 or higher with Unicode enabled
      2. Boost-1.33.0 or higher
      3. Boost.Xpressive latest version

B. MS Windows (__WXMSW__):
   a. MinGW32/GNU C++ 3.x (wxDevCpp):
      Required Libraries to compile:
      1. wxWidgets-2.6.1 or higher with Unicode enabled
      2. Boost-1.33.0 or higher
      3. Boost.Xpressive latest version
      4. libunicows: it's optional under WinNT/XP, but required under Win98
   b. Visual C++ 7.1 (VS.Net 2003):
      Required Libraries to compile:
      1. wxWidgets-2.6.1 or higher with Unicode enabled
      2. Boost-1.33.0 or higher
      3. Boost.Xpressive latest version
      4. libunicows: it's optional under WinNT/XP, but required under Win98

P.S.: If you use CVS version of Boost, Boost.Xpressive has been included in the CVS HEAD, 
      or you must get 'xpressive.zip' at:
      http://www.boost-consulting.com/vault/index.php?directory=Strings%20-%20Text%20Processing
P.S.2: Since MadEdit v0.2.2, Xpressive is attached in MadEdit sources package.

Syntax files, Locale files and Settings:
---------------------------------------
  Syntax files: in the path $(MadEditConfigPath)/syntax/
  Locale files: in the path $(MadEditConfigPath)/locale/
  
  $(MadEditConfigPath) are the one of below paths:
    1. MadEdit execution file directory : Windows and Linux
    2. $(HOME)/.madedit/ : Linux
    3. /usr/share/madedit/ : Linux
  
  If MadEdit does not find any files in those paths, 
  MadEdit can also execute standalone by using default settings.
  
  If you are under Win98, you must get unicows.dll from MS's website:
  http://www.microsoft.com/downloads/details.aspx?FamilyId=73BA7BD7-ED06-4F0D-80A4-2A7EEAEE17E2&displaylang=en
  
  When quit MadEdit, MadEdit will save the settings to:
    1. $(HOME)/.madedit/madedit.cfg: Linux
    2. $(MadEdit execution file directory)/MadEdit.cfg: Windows
  And FontWidth.dat will also be saved to the same path.
  FontWidth.dat is Cache of Font-Width-Data, it can speed-up MadEdit a lot.


ChangeLog:
----------
v0.2.8:
1.added Japanese Kanji supporting for converting Chinese characters.
2.added the function to split window into multiple Tab Controls (by using wxAuiNotebook)
  ScreenShot: http://madedit.sourceforge.net/screenshot06.png
3.added the function to convert between WordWraps and NewLine chars.
4.added File-Type Associations Dialog under win32.
5.added Search-In-Selection function when searching and replaceing.
6.added Count function in SearchDialog.
7.added ToHalfWidthByOptions, ToFullWidthByOptions to choose the characters(ASCII,Japanese,Korean)
  you want to convert.
8.added ConvertSpaceToTab and ConvertTabToSpace functions.
9.added new syntax files for D, SQL, Verilog and VHDL.
10.fixed that cannot replace text in Hex-Mode.
11.many bug-fixes and changes.

v0.2.7:
1.fixed that ISO-8859-7 charset doesn't available under Win32.
2.fixed that incorrectly handle Japanese halfwidth characters in Shift-JIS charset.
3.supported XIM Over-The-Spot feature under Linux/FreeBSD(wxGTK).
4.added options to set Date/Time format.
5.added a option to reload files previously open on startup.
6.added a option to restore previous caret position when files are opened.
7.fixed a crashing bug that incorrectly detect MAC/DOS new-line char on HTML/PHP files.
8.changed for showing CR, LF, CRLF and EOF in different marks.

v0.2.6:
1.improved the redraw speed under Linux (wxGTK).
2.added charset-detector, a port to C of "universalchardet", for detecting the encoding of files.
  Thanks Jserv (http://blog.linux.org.tw/~jserv/archives/001672.html).
3.improved the speed of FindInFiles function.
4.fixed crashing bug when opening a file begins with a 0x0b/0x0c character.
5.added new syntax files for Fortran, TeX/LaTeX, Squirrel, C#, Visual Basic, ASP(VBScript).
6.many misc changes and bug-fixes.

v0.2.5:
1.fixed crashes when copying and pasting data between TextMode and ColumnMode under Linux.
2.fixed sometimes crashes when using FindInFiles function under Linux.
3.added a dockable Information Window for listing the results of FindInFiles.
4.added Exclude Filters in FindInFiles dialog.
5.added a button ("<<") to get the path of the editing document in FindInFiles dialog.
6.added support for multiple shortcuts on all menu and editor commands.
7.added support for Mouse-Over-Get-Word of StarDict under Win32.
8.added Trim Trialing Spaces function.
9.added Sort Lines function.
10.added a new option to temporary disable or enable MouseSelectToCopy function when pressing Ctrl key.
11.added a new option to choose the Language of UI.
12.added Italian language. Thank Contardi Angelo for the translations.

v0.2.4-2:
1.fixed that lost the last byte of clipboard data (Linux version only).

v0.2.4:
1.added custom Syntax Highlighting settings.
2.added Find/Replace In Files function.
3.some minor changes.

v0.2.3:
1.improved the loading speed for the files contain U+10000~U+10FFFF widechars.
2.improved the repaint speed when the lines are very long(>1MB).
3.added ToHalfWidth, ToFullWidth functions.
4.added WordCount function.
5.added InsertDataTime function.
6.added ecReturnNoIndent(Shift-Enter) to insert NewLine char without indentation.
7.now 'ESC' key can be used in custom keybindings.
8.some minor changes.

v0.2.2:
1.fixed that menu icons will cause crash under Win98.
2.fixed that input incorrect CJK chars under Win98.
3.fixed that copy and then paste data in Column/Hex modes will cause crash under Win98.
4.added a messagebox to notify that madedit cannot find the matched string.
5.added ToUpperCase, ToLowerCase, InvertCase functions.
6.some misc changes.

v0.2.1-2:
1.fixed that it will crash when close ConvertEncodingDialog by pressing top-right 'X' button.
2.fixed the bug of AutoCompletePair function.

v0.2.1:
1.fixed copy/paste text to/from PrimarySelection when use mouse under Linux.
2.added ConvertEncoding function.
3.added ConvertChineseChar(Simplified<=>Traditional) in both of file and clipboard.
4.added AutoCompletePair function. The AutoCompletePair must be defined in syntax files.
5.improved AutoIndent function. The IndentChar and UnindentChar must be defined in syntax files.

v0.2.0:
1.fixed syntax parsing error in non-global range(ex: PHP, JavaScript).
2.fixed that reopen the same file under Win32 because compare filename with case sensitive.
3.added new option of custom KeyBindings.
4.added MouseSelectToCopy and MiddleMouseToPaste functions.
5.added new syntax files for CSS, Windows Registry File, XML.

v0.1.9.9:
1.fixed the incorrect searching result on some search patterns.
2.added SaveAll function.
3.some misc changes.

v0.1.9:
1.replaced Boost.Regex with Boost.Xpressive for searching & replacing Regular Expressions.
  now we can use wxDevCpp with the precompiled wxWidgets library(*.devpak, Unicode version)
  on www.devpaks.org to build MadEdit under win32.
2.added a new function for Multi-Line Editing in Column Mode.
3.several misc changes & bug-fixs.

v0.1.8:
1.fixed that randomly crash when quit MadEdit for Linux version.
2.fixed EscapeChar syntax checking.
3.fixed that Print/Preview incorrectly when the position of Horizontal-ScrollBar is not 0.
4.added a new option of 'Default Encoding'.
5.added automatically detect the encoding of FileName under Linux by current locale.
  When save a file, MadEdit will use UTF8 as default encoding of the FileName.
6.added new syntax files for diff/patch, JavaScript and Ruby.

v0.1.7:
1.fixed several misc bugs.
2.fixed MultiByte Encoding <==> Unicode converting error in v0.1.6 under Linux.
3.removed the linking to the unnecessary libs(e.g. libtiff, libjpeg and libpng) for Linux version.
4.added Print&PrintPreview functions. (Known bug: cannot print/preview correctly on the papers with landscape orientation.)
5.added Comment(Ctrl-M)/Uncomment(Ctrl-Shift-M) functions. MadEdit will add/remove LineComment string to the selected lines, the LineComment must be defined in the syntax files.

v0.1.6:
1.fixed that cannot search U+10000~U+10FFFF ucs4 char under win32.
2.added Increase&Decrease Indent functions. 
When select multi-lines and press 'Tab' will Inc-Indent, and press 'Shift-Tab' will Dec-Indent.

v0.1.5:
1.fixed several bugs.
2.now MadEdit will open the same files once only.
3.improved Reload() function, and MadEdit will reload the files which were changed by another app.

v0.1.4:
1.fixed the bug of Mark Brace incorrect position when use WordWrap.
2.fixed several misc. bugs.
3.used a new approach to determine file types:
  (a)by file extension (b)by the context of first line (c)by file name.
4.added a setting of "Do Not Save Settings to MadEdit.cfg after MadEdit Finish",
  it's useful when you edit 'MadEdit.cfg' by MadEdit.

v0.1.3:
1.fixed several bugs.
2.added Mark Brace Pair.
3.added Go To Left/Right Brace (Ctrl-[ and Ctrl-], press Shift to select).
  the Brace-Pair must be defined in syntax_*.syn files.
4.added Cut Line (Ctrl-Shift-L).
5.added Insert Tab Char (Ctrl-~ or Ctrl-`).
6.supported Drop Files from window-manager under Linux.
7.improved the speed of repaint in HexMode.

v0.1.2:
1.fixed several bugs.
2.added Replace Text/Hex functions.
3.added some fine improvements.

v0.1.1:
  fixed some bugs.
  added RightClick Popup Menu.
  added new syntax files.
  added FindPrevious & FindHexNext/Previous functions.
  changed ShortCuts of Text/Column/HexMode to Alt-1, Alt-2, Alt-3.

v0.1:
  added Edit & Search functions.

v0.0.9:
  Preview & AlphaTest version.


License:
-------
MadEdit is released under the GNU General Public License.


Author:
------
Alston Chen <madedit@gmail.com>


Links:
-----
MadEdit: http://sourceforge.net/projects/madedit/
wxWidgets: http://www.wxwidgets.org
wxDevCpp: http://wxdsgn.sourceforge.net
Boost: http://www.boost.org
Boost.Xpressive: http://boost-sandbox.sourceforge.net/libs/xpressive/
libunicows: http://libunicows.sourceforge.net
Open Clip Art Library: http://www.openclipart.org

