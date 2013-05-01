SUBDIRS = syntax po

bin_PROGRAMS = madedit
INCLUDES = -I$(top_srcdir)/wxAUI/include -I$(top_srcdir)/charset-detector/include -I$(top_srcdir)/charset-detector/src -I$(top_srcdir)/charset-detector/src/tables
madedit_CXXFLAGS = -DDATA_DIR=\"${datadir}\"
madedit_SOURCES =	src/MadAboutDialog.cpp \
	src/MadAboutDialog.h \
	src/MadCommand.h \
	src/MadConvEncDialog.cpp \
	src/MadConvEncDialog.h \
	src/MadEditApp.cpp \
	src/MadEditApp.h \
	src/MadEditFrame.cpp \
	src/MadEditFrame.h \
	src/MadFindInFilesDialog.cpp \
	src/MadFindInFilesDialog.h \
	src/MadHighlightingDialog.cpp \
	src/MadHighlightingDialog.h \
	src/MadOptionsDialog.cpp \
	src/MadOptionsDialog.h \
	src/MadPlugin.cpp \
	src/MadPrintout.cpp \
	src/MadPrintout.h \
	src/MadReplaceDialog.cpp \
	src/MadReplaceDialog.h \
	src/MadSearchDialog.cpp \
	src/MadSearchDialog.h \
	src/MadSortDialog.cpp \
	src/MadSortDialog.h \
	src/MadUtils.cpp \
	src/MadUtils.h \
	src/MadWordCountDialog.cpp \
	src/MadWordCountDialog.h \
	src/plugin.h \
	src/MadEdit/caret_new.cpp \
	src/MadEdit/caret_new.h \
	src/MadEdit/clipbrd_gtk.cpp \
	src/MadEdit/clipbrd_gtk.h \
	src/MadEdit/MadDeque.hpp \
	src/MadEdit/MadEdit.cpp \
	src/MadEdit/MadEdit.h \
	src/MadEdit/MadEditAdvanced.cpp \
	src/MadEdit/MadEditBasic.cpp \
	src/MadEdit/MadEditCommand.cpp \
	src/MadEdit/MadEditCommand.h \
	src/MadEdit/MadEditSearch.cpp \
	src/MadEdit/MadEdit_gtk.cpp \
	src/MadEdit/MadEncoding.cpp \
	src/MadEdit/MadEncoding.h \
	src/MadEdit/MadLines.cpp \
	src/MadEdit/MadLines.h \
	src/MadEdit/MadSyntax.cpp \
	src/MadEdit/MadSyntax.h \
	src/MadEdit/MadUndo.cpp \
	src/MadEdit/MadUndo.h \
	src/MadEdit/TradSimp.cpp \
	src/MadEdit/TradSimp.h \
	src/MadEdit/ucs4_t.h \
	wxAUI/include/wx/aui/aui.h \
	wxAUI/include/wx/aui/auibar.h \
	wxAUI/include/wx/aui/auibook.h \
	wxAUI/include/wx/aui/dockart.h \
	wxAUI/include/wx/aui/floatpane.h \
	wxAUI/include/wx/aui/framemanager.h \
	wxAUI/include/wx/aui/tabmdi.h \
	wxAUI/src/auibar.cpp \
	wxAUI/src/auibook.cpp \
	wxAUI/src/dockart.cpp \
	wxAUI/src/floatpane.cpp \
	wxAUI/src/framemanager.cpp \
	wxAUI/src/tabmdi.cpp \
	charset-detector/src/CharDistribution.cpp \
	charset-detector/src/CharDistribution.h \
	charset-detector/src/JpCntx.cpp \
	charset-detector/src/JpCntx.h \
	charset-detector/src/LangBulgarianModel.cpp \
	charset-detector/src/LangCyrillicModel.cpp \
	charset-detector/src/LangGreekModel.cpp \
	charset-detector/src/LangHebrewModel.cpp \
	charset-detector/src/LangHungarianModel.cpp \
	charset-detector/src/LangThaiModel.cpp \
	charset-detector/src/nsBig5Prober.cpp \
	charset-detector/src/nsBig5Prober.h \
	charset-detector/src/nsCharSetProber.cpp \
	charset-detector/src/nsCharSetProber.h \
	charset-detector/src/nsCodingStateMachine.h \
	charset-detector/src/nscore.h \
	charset-detector/src/nsEscCharsetProber.cpp \
	charset-detector/src/nsEscCharsetProber.h \
	charset-detector/src/nsEscSM.cpp \
	charset-detector/src/nsEUCJPProber.cpp \
	charset-detector/src/nsEUCJPProber.h \
	charset-detector/src/nsEUCKRProber.cpp \
	charset-detector/src/nsEUCKRProber.h \
	charset-detector/src/nsEUCTWProber.cpp \
	charset-detector/src/nsEUCTWProber.h \
	charset-detector/src/nsGB2312Prober.cpp \
	charset-detector/src/nsGB2312Prober.h \
	charset-detector/src/nsHebrewProber.cpp \
	charset-detector/src/nsHebrewProber.h \
	charset-detector/src/nsLatin1Prober.cpp \
	charset-detector/src/nsLatin1Prober.h \
	charset-detector/src/nsMBCSGroupProber.cpp \
	charset-detector/src/nsMBCSGroupProber.h \
	charset-detector/src/nsMBCSSM.cpp \
	charset-detector/src/nsPkgInt.h \
	charset-detector/src/nsSBCharSetProber.cpp \
	charset-detector/src/nsSBCharSetProber.h \
	charset-detector/src/nsSBCSGroupProber.cpp \
	charset-detector/src/nsSBCSGroupProber.h \
	charset-detector/src/nsSJISProber.cpp \
	charset-detector/src/nsSJISProber.h \
	charset-detector/src/nsUniversalDetector.cpp \
	charset-detector/src/nsUniversalDetector.h \
	charset-detector/src/nsUTF8Prober.cpp \
	charset-detector/src/nsUTF8Prober.h \
	charset-detector/src/prmem.h \
	charset-detector/src/entry/impl.cpp \
	charset-detector/include/chardetect.h

pixmapsdir = $(datadir)/pixmaps
pixmaps_DATA = madedit.xpm

appdir = $(datadir)/applications
app_DATA = madedit.desktop

EXTRA_DIST = m4/ChangeLog  $(pixmaps_DATA) $(app_DATA)

noinst_HEADERS =	description-pak \
	README.txt \
	autogen.sh \
	madedit.spec \
	madedit-static.spec \
	debian/changelog \
	debian/control \
	debian/copyright \
	debian/menu \
	debian/rules \
	freebsd/pkg-comment \
	freebsd/pkg-descr \
	freebsd/pkg-plist \
	freebsd/pkgcreate.txt \
	m4/gettext.m4 \
	m4/iconv.m4 \
	m4/lib-ld.m4 \
	m4/lib-link.m4 \
	m4/lib-prefix.m4 \
	m4/nls.m4 \
	m4/po.m4 \
	m4/progtest.m4 \
	m4/ChangeLog \
	src/MadEditApp.rc \
	src/MadAboutDialog.wxform \
	src/MadConvEncDialog.wxform \
	src/MadEditFrame.wxform \
	src/MadFileAssociationDialog.cpp \
	src/MadFileAssociationDialog.h \
	src/MadFileAssociationDialog.wxform \
	src/MadFindInFilesDialog.wxform \
	src/MadHighlightingDialog.wxform \
	src/MadOptionsDialog.wxform \
	src/MadReplaceDialog.wxform \
	src/MadSearchDialog.wxform \
	src/MadSortDialog.wxform \
	src/MadWordCountDialog.wxform \
	src/plugin_example/makefile.linux \
	src/plugin_example/makefile.mingw32 \
	src/plugin_example/makefile.vc \
	src/plugin_example/plugin.cpp \
	src/plugin_example/vc_cl.bat \
	src/plugin_example/wxwin16x16.xpm \
	wxAUI/COPYING.LIB \
	wxAUI/LICENCE.txt \
	wxAUI/README.txt \
	devcpp/MadEdit.dev \
	devcpp/MadEdit.layout \
	devcpp/MadEdit_private.h \
	devcpp/MadEdit_private.rc \
	image2xpm/image2xpm.dev \
	image2xpm/image2xpm.layout \
	image2xpm/image2xpmApp.cpp \
	image2xpm/image2xpmApp.h \
	image2xpm/image2xpmApp.rc \
	image2xpm/image2xpmFrm.cpp \
	image2xpm/image2xpmFrm.h \
	image2xpm/image2xpmFrm.wxform \
	image2xpm/image2xpmFrm.xml \
	image2xpm/image2xpm_private.h \
	image2xpm/image2xpm_private.rc \
	images/closeall.xpm \
	images/columnmode.xpm \
	images/comment.xpm \
	images/copy.xpm \
	images/cut.xpm \
	images/down.xpm \
	images/fileclose.xpm \
	images/fileopen.xpm \
	images/filesave.xpm \
	images/filesaveas.xpm \
	images/find.xpm \
	images/findnext.xpm \
	images/findprev.xpm \
	images/font.xpm \
	images/fontname.xpm \
	images/fontsize.xpm \
	images/hexmode.xpm \
	images/indent.xpm \
	images/Mad.ico \
	images/Mad2.ico \
	images/Mad.xpm \
	images/Mad2.xpm \
	images/Mad_16x15.xpm \
	images/new.xpm \
	images/nowrap.xpm \
	images/null.xpm \
	images/paste.xpm \
	images/preview.xpm \
	images/print.xpm \
	images/quit.xpm \
	images/redo.xpm \
	images/replace.xpm \
	images/saveall.xpm \
	images/textmode.xpm \
	images/uncomment.xpm \
	images/undo.xpm \
	images/unindent.xpm \
	images/wrapbycol.xpm \
	images/wrapbywin.xpm \
	libunicows/libunicows.a \
	libunicows/README.txt \
	libunicows/unicows.lib \
	makefiles/Makefile.mac \
	makefiles/Makefile.mingw \
	makefiles/Makefile.install \
	makefiles/zcfg_wx_debug.sh \
	makefiles/zcfg_wx_release.sh \
	po/madedit.pot \
	po/src.list \
	po/wxstd.po.txt \
	po/wxstd.po.cpp \
	po/xgettext.bat \
	po/xgettext_join.bat \
	vc71/build_debug.bat \
	vc71/build_release.bat \
	vc71/MadEdit.sln \
	vc71/MadEdit.vcproj \
	vc80/MadEdit.sln \
	vc80/MadEdit.vcproj \
	charset-detector/Makefile \
	charset-detector/MPL-1.1.txt \
	charset-detector/README \
	charset-detector/test/Makefile \
	charset-detector/test/test-chardetect.c \
	charset-detector/src/Makefile \
	charset-detector/src/tables/Big5Freq.tab \
	charset-detector/src/tables/EUCKRFreq.tab \
	charset-detector/src/tables/EUCTWFreq.tab \
	charset-detector/src/tables/GB2312Freq.tab \
	charset-detector/src/tables/JISFreq.tab \
	charset-detector/src/entry/export-symbol.list


DOMAIN = madedit
podir = $(top_srcdir)/po
LINGUAS = @ALL_LINGUAS@

loc:
	@linguas='$(LINGUAS)'; \
	for lang in $$linguas; do \
		dir=./locale/$$lang; \
		$(mkinstalldirs) $$dir; \
		if test -r $(podir)/$$lang.gmo; then \
			cp $(podir)/$$lang.gmo $$dir/$(DOMAIN).mo; \
			echo "installing $(podir)/$$lang.gmo as $$dir/$(DOMAIN).mo"; \
		fi; \
	done


#ACLOCAL_AMFLAGS = -I m4
