///////////////////////////////////////////////////////////////////////////////
// Name:        MadEditFrame.h
// Description: Main frame of MadEdit
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _MADEDITFRAME_H_
#define _MADEDITFRAME_H_

#include <wx/wxprec.h>
#ifdef __BORLANDC__
        #pragma hdrstop
#endif
#ifndef WX_PRECOMP
        #include <wx/wx.h>
#endif

//Do not add custom headers.
//wx-dvcpp designer will remove them
////Header Include Start
#include <wx/menu.h>
#include <wx/toolbar.h>
#include <wx/statusbr.h>
////Header Include End

#include <wx/frame.h>
#include <wx/docview.h>
#include <wx/fileconf.h>
#include <wx/hashset.h>
#include <wx/imaglist.h>
#include <wx/treectrl.h>

#include <manager.h> // wxAUI


////Dialog Style Start
#undef MadEditFrame_STYLE
#define MadEditFrame_STYLE wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxCLOSE_BOX
////Dialog Style End


class wxFlatNotebook;
class wxFlatNotebookEvent;
class MadEdit;

class MadEditFrame : public wxFrame
{
private:
    DECLARE_EVENT_TABLE()
public:
    MadEditFrame( wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("MadEdit"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = MadEditFrame_STYLE);
    virtual ~MadEditFrame();
public:
  //Do not add custom Control Declarations here.
  //wx-devcpp will remove them. Try adding the custom code
  //after the block.
  ////GUI Control Declaration Start
		wxMenuBar *WxMenuBar1;
		wxToolBar *WxToolBar1;
		wxStatusBar *WxStatusBar1;
  ////GUI Control Declaration End
public:
	//Note: if you receive any error with these enums, then you need to
    //change your old form code that are based on the #define control ids.
	//It may replace a numeric value in the enums names.
	enum {
////GUI Enum Control ID Start
			ID_WXTOOLBAR1 = 1005,
			ID_WXSTATUSBAR1 = 1003,
////GUI Enum Control ID End

    ID_NOTEBOOK, // for wxFlatNotebook m_Notebook
    ID_OUTPUTNOTEBOOK,
    ID_FINDINFILESRESULTS,

    ID_DUMMY_VALUE_ //Dont Delete this DummyValue
   }; //End of Enum

public:
    wxFlatNotebook *m_Notebook;
    int           m_NewFileCount;
    wxConfigBase  *m_Config;
    wxImageList   *m_ImageList;
    wxFileHistory *m_RecentFiles;
    wxFileHistory *m_RecentEncodings;
    wxFileHistory *m_RecentFonts;

    wxFrameManager m_FrameManager; // wxAUI
    wxFlatNotebook *m_InfoNotebook; //
    wxTreeCtrl *m_FindInFilesResults;

    void OnUpdateUI_MenuFile_CheckCount(wxUpdateUIEvent& event);
    void OnUpdateUI_MenuFileReload(wxUpdateUIEvent& event);
    void OnUpdateUI_MenuFileRecentFiles(wxUpdateUIEvent& event);

    void OnUpdateUI_MenuEditUndo(wxUpdateUIEvent& event);
    void OnUpdateUI_MenuEditRedo(wxUpdateUIEvent& event);
    void OnUpdateUI_MenuEdit_CheckSelection(wxUpdateUIEvent& event);
    void OnUpdateUI_MenuEdit_CheckSelSize(wxUpdateUIEvent& event);
    void OnUpdateUI_MenuEditCopy(wxUpdateUIEvent& event);
    void OnUpdateUI_MenuEditPaste(wxUpdateUIEvent& event);
    void OnUpdateUI_Menu_CheckSize(wxUpdateUIEvent& event);
    void OnUpdateUI_MenuEditDeleteLine(wxUpdateUIEvent& event);
    void OnUpdateUI_MenuEditInsertTabChar(wxUpdateUIEvent& event);
    void OnUpdateUI_MenuEditInsertDateTime(wxUpdateUIEvent& event);

    void OnUpdateUI_MenuEditCopyAsHexString(wxUpdateUIEvent& event);
    void OnUpdateUI_MenuIndent(wxUpdateUIEvent& event);
    void OnUpdateUI_MenuComment(wxUpdateUIEvent& event);

    void OnUpdateUI_MenuSearchGoTo(wxUpdateUIEvent& event);
    void OnUpdateUI_MenuSearchGoToBrace(wxUpdateUIEvent& event);

    void OnUpdateUI_MenuViewEncoding(wxUpdateUIEvent& event);
    void OnUpdateUI_MenuViewSyntax(wxUpdateUIEvent& event);
    void OnUpdateUI_MenuViewFontName(wxUpdateUIEvent& event);
    void OnUpdateUI_MenuViewFontSize(wxUpdateUIEvent& event);
    void OnUpdateUI_MenuViewSetFont(wxUpdateUIEvent& event);
    void OnUpdateUI_MenuViewFixedWidthMode(wxUpdateUIEvent& event);
    void OnUpdateUI_MenuViewTabColumn(wxUpdateUIEvent& event);
    void OnUpdateUI_MenuViewLineSpacing(wxUpdateUIEvent& event);
    void OnUpdateUI_MenuViewNoWrap(wxUpdateUIEvent& event);
    void OnUpdateUI_MenuViewWrapByWindow(wxUpdateUIEvent& event);
    void OnUpdateUI_MenuViewWrapByColumn(wxUpdateUIEvent& event);
    void OnUpdateUI_MenuViewDisplayLineNumber(wxUpdateUIEvent& event);
    void OnUpdateUI_MenuViewShowEndOfLine(wxUpdateUIEvent& event);
    void OnUpdateUI_MenuViewShowTabChar(wxUpdateUIEvent& event);
    void OnUpdateUI_MenuViewShowSpaceChar(wxUpdateUIEvent& event);
    void OnUpdateUI_MenuViewMarkActiveLine(wxUpdateUIEvent& event);
    void OnUpdateUI_MenuViewMarkBracePair(wxUpdateUIEvent& event);
    void OnUpdateUI_MenuViewTextMode(wxUpdateUIEvent& event);
    void OnUpdateUI_MenuViewColumnMode(wxUpdateUIEvent& event);
    void OnUpdateUI_MenuViewHexMode(wxUpdateUIEvent& event);

    void OnUpdateUI_MenuToolsByteOrderMark(wxUpdateUIEvent& event);
    void OnUpdateUI_MenuToolsNewLineChar(wxUpdateUIEvent& event);
    void OnUpdateUI_MenuToolsInsertNewLineChar(wxUpdateUIEvent& event);
    void OnUpdateUI_MenuToolsConvertNL(wxUpdateUIEvent& event);
    void OnUpdateUI_MenuToolsConvertEncoding(wxUpdateUIEvent& event);

    void OnUpdateUI_MenuWindow_CheckCount(wxUpdateUIEvent& event);

    void OnFileNew(wxCommandEvent& event);
    void OnFileOpen(wxCommandEvent& event);
    void OnFileSave(wxCommandEvent& event);
    void OnFileSaveAs(wxCommandEvent& event);
    void OnFileSaveAll(wxCommandEvent& event);
    void OnFileReload(wxCommandEvent& event);
    void OnFileClose(wxCommandEvent& event);
    void OnFileCloseAll(wxCommandEvent& event);
    void OnFilePageSetup(wxCommandEvent& event);
    void OnFilePrintPreview(wxCommandEvent& event);
    void OnFilePrint(wxCommandEvent& event);
    void OnFileRecentFile(wxCommandEvent& event);
    void OnFileExit(wxCommandEvent& event);

    void OnEditUndo(wxCommandEvent& event);
    void OnEditRedo(wxCommandEvent& event);
    void OnEditCut(wxCommandEvent& event);
    void OnEditCopy(wxCommandEvent& event);
    void OnEditPaste(wxCommandEvent& event);
    void OnEditDelete(wxCommandEvent& event);
    void OnEditCutLine(wxCommandEvent& event);
    void OnEditDeleteLine(wxCommandEvent& event);
    void OnEditSelectAll(wxCommandEvent& event);
    void OnEditInsertTabChar(wxCommandEvent& event);
    void OnEditInsertDateTime(wxCommandEvent& event);
    void OnEditCopyAsHexString(wxCommandEvent& event);
    void OnEditCopyAsHexStringWithSpace(wxCommandEvent& event);
    void OnEditIncIndent(wxCommandEvent& event);
    void OnEditDecIndent(wxCommandEvent& event);
    void OnEditComment(wxCommandEvent& event);
    void OnEditUncomment(wxCommandEvent& event);
    void OnEditToUpperCase(wxCommandEvent& event);
    void OnEditToLowerCase(wxCommandEvent& event);
    void OnEditInvertCase(wxCommandEvent& event);
    void OnEditToHalfWidth(wxCommandEvent& event);
    void OnEditToFullWidth(wxCommandEvent& event);

    void OnSearchFind(wxCommandEvent& event);
    void OnSearchFindNext(wxCommandEvent& event);
    void OnSearchFindPrevious(wxCommandEvent& event);
    void OnSearchReplace(wxCommandEvent& event);
    void OnSearchFindInFiles(wxCommandEvent& event);
    void OnSearchShowFindInFilesResults(wxCommandEvent& event);
    void OnSearchGoToLine(wxCommandEvent& event);
    void OnSearchGoToPosition(wxCommandEvent& event);
    void OnSearchGoToLeftBrace(wxCommandEvent& event);
    void OnSearchGoToRightBrace(wxCommandEvent& event);

    void OnViewEncoding(wxCommandEvent& event);
    void OnViewRecentEncoding(wxCommandEvent& event);
    void OnViewSyntax(wxCommandEvent& event);
    void OnViewFontName(wxCommandEvent& event);
    void OnViewRecentFont(wxCommandEvent& event);
    void OnViewFontSize(wxCommandEvent& event);
    void OnViewSetFont(wxCommandEvent& event);
    void OnViewFixedWidthMode(wxCommandEvent& event);
    void OnViewTabColumn(wxCommandEvent& event);
    void OnViewLineSpacing(wxCommandEvent& event);
    void OnViewNoWrap(wxCommandEvent& event);
    void OnViewWrapByWindow(wxCommandEvent& event);
    void OnViewWrapByColumn(wxCommandEvent& event);
    void OnViewDisplayLineNumber(wxCommandEvent& event);
    void OnViewShowEndOfLine(wxCommandEvent& event);
    void OnViewShowTabChar(wxCommandEvent& event);
    void OnViewShowSpaceChar(wxCommandEvent& event);
    void OnViewMarkActiveLine(wxCommandEvent& event);
    void OnViewMarkBracePair(wxCommandEvent& event);
    void OnViewTextMode(wxCommandEvent& event);
    void OnViewColumnMode(wxCommandEvent& event);
    void OnViewHexMode(wxCommandEvent& event);

    void OnToolsOptions(wxCommandEvent& event);
    void OnToolsHighlighting(wxCommandEvent& event);
    void OnToolsToggleBOM(wxCommandEvent& event);
    void OnToolsConvertToDOS(wxCommandEvent& event);
    void OnToolsConvertToMAC(wxCommandEvent& event);
    void OnToolsConvertToUNIX(wxCommandEvent& event);
    void OnToolsInsertDOS(wxCommandEvent& event);
    void OnToolsInsertMAC(wxCommandEvent& event);
    void OnToolsInsertUNIX(wxCommandEvent& event);
    void OnToolsConvertEncoding(wxCommandEvent& event);
    void OnToolsSimp2TradChinese(wxCommandEvent& event);
    void OnToolsTrad2SimpChinese(wxCommandEvent& event);
    void OnToolsSimp2TradClipboard(wxCommandEvent& event);
    void OnToolsTrad2SimpClipboard(wxCommandEvent& event);
    void OnToolsWordCount(wxCommandEvent& event);

    void OnWindowToggleWindow(wxCommandEvent& event);
    void OnWindowPreviousWindow(wxCommandEvent& event);
    void OnWindowNextWindow(wxCommandEvent& event);

    void OnHelpAbout(wxCommandEvent& event);

private:
    bool m_PageClosing; // prevent from reentry of CloseFile(), OnNotebookPageClosing()
public:
    int OpenedFileCount();
    void OpenFile(const wxString &filename, bool mustExist); // if filename is empty, open a new file
    void CloseAllFiles(bool force);
    void MadEditFrameKeyDown(wxKeyEvent& event);
    void SetPageFocus(int pageId);
    MadEdit *GetEditByFileName(const wxString &filename, int &id);
    void ResetAcceleratorTable();

protected:
    void MadEditFrameClose(wxCloseEvent& event);
    void CreateGUIControls(void);

#ifndef __WXMSW__
    virtual void OnInternalIdle();
#endif

    void OnNotebookPageChanged(wxFlatNotebookEvent& event);
    void OnNotebookPageClosing(wxFlatNotebookEvent& event);
    void OnNotebookPageClosed(wxFlatNotebookEvent& event);

    void OnSize(wxSizeEvent &evt);
    //void OnChar(wxKeyEvent& evt);
    void OnActivate(wxActivateEvent &evt);

    bool QueryCloseFile(int idx);
    bool QueryCloseAllFiles();

    void LoadMenuKeys(wxConfigBase *config);
    wxString GetMenuKey(const wxString &menu, const wxString &defaultkey);

    void OnInfoNotebookSize(wxSizeEvent &evt);
    void OnFindInFilesResultsDClick(wxMouseEvent& event);

#ifdef __WXMSW__
    WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam);
#endif

public:
    void ResetFindInFilesResults();
    void AddItemToFindInFilesResults(const wxString &text, size_t index, const wxString &filename, int pageid, const wxFileOffset &begpos, wxFileOffset &endpos);

};

enum { // menu id
    menuNew = wxID_NEW,
    menuOpen = wxID_OPEN,
    menuSave = wxID_SAVE,
    menuSaveAs = wxID_SAVEAS,
    menuClose = wxID_CLOSE,
    menuCloseAll = wxID_CLOSE_ALL,
    menuPageSetup = wxID_PRINT_SETUP,
    menuPrintPreview = wxID_PREVIEW,
    menuPrint = wxID_PRINT,
    menuExit = wxID_EXIT,
    menuUndo = wxID_UNDO,
    menuRedo = wxID_REDO,
    menuCut = wxID_CUT,
    menuCopy = wxID_COPY,
    menuPaste = wxID_PASTE,
    menuDelete = wxID_DELETE,
    menuSelectAll = wxID_SELECTALL,
    menuFind = wxID_FIND,
    menuReplace = wxID_REPLACE,
    menuAbout = wxID_ABOUT,

    // file
    menuSaveAll = 1100,
    menuReload,
    menuRecentFiles,

    // edit
    menuCutLine,
    menuDeleteLine,
    menuInsertTabChar,
    menuInsertDateTime,
    menuAdvanced,
    menuCopyAsHexString,
    menuCopyAsHexStringWithSpace,
    menuIncreaseIndent,
    menuDecreaseIndent,
    menuComment,
    menuUncomment,
    menuToUpperCase,
    menuToLowerCase,
    menuInvertCase,
    menuToHalfWidth,
    menuToFullWidth,

    // search
    menuFindNext,
    menuFindPrevious,
    menuFindInFiles,
    menuShowFindInFilesResults,
    menuGoToLine,
    menuGoToPosition,
    menuLeftBrace,
    menuRightBrace,

    // view
    menuEncoding,
    menuAllEncodings,
    menuEncoding1,
    menuEncoding99 = menuEncoding1 + 98,

    menuRecentEncoding1,
    menuRecentEncoding9 = menuRecentEncoding1 + 8,

    menuSyntax,
    menuSyntax1,
    menuSyntax199 = menuSyntax1 + 198,

    menuFontName,
    menuFontName1,
    menuFontName999 = menuFontName1 + 998,

    menuFont0, //[@]
    menuFont1, //[ABCDE]
    menuFont2, //[FGHIJ]
    menuFont3, //[KLMNO]
    menuFont4, //[PQRST]
    menuFont5, //[UVWXYZ]
    menuFont6, //[Other]

    menuRecentFonts,
    menuRecentFont1,
    menuRecentFont9 = menuRecentFont1 + 8,

    menuFontSize,
    menuFontSize1,
    menuFontSize99 = menuFontSize1 + 98,

    menuSetFont,
    menuFixedWidthMode,

    menuTabColumn,
    menuTabColumn1,
    menuTabColumn16 = menuTabColumn1 + 15,

    menuLineSpacing,
    menuLineSpacing100,
    menuLineSpacing250 = menuLineSpacing100 + 30,

    menuNoWrap,
    menuWrapByWindow,
    menuWrapByColumn,
    menuDisplayLineNumber,
    menuShowEndOfLine,
    menuShowTabChar,
    menuShowSpaceChar,
    menuMarkActiveLine,
    menuMarkBracePair,
    menuTextMode,
    menuColumnMode,
    menuHexMode,

    // tools
    menuOptions,
    menuHighlighting,
    menuPlugins,
    menuByteOrderMark,
    menuToggleBOM,
    menuNewLineChar,
    menuConvertToDOS,
    menuConvertToMAC,
    menuConvertToUNIX,
    menuInsertNewLineChar,
    menuInsertDOS,
    menuInsertMAC,
    menuInsertUNIX,
    menuConvertEncoding,
    menuConvertChineseChar,
    menuSimp2TradChinese,
    menuTrad2SimpChinese,
    menuSimp2TradClipboard,
    menuTrad2SimpClipboard,
    menuWordCount,

    // window
    menuToggleWindow,
    menuPreviousWindow,
    menuNextWindow
};

extern MadEditFrame *g_MainFrame;
extern void OnReceiveMessage(const wchar_t *msg, size_t size);

#endif

