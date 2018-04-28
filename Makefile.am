SUBDIRS = syntax po

enc_src =	src/xm/encoding/cp20932.cpp \
	src/xm/encoding/cp20932.h \
	src/xm/encoding/doublebyte.cpp \
	src/xm/encoding/doublebyte.h \
	src/xm/encoding/encoding.cpp \
	src/xm/encoding/encoding.h \
	src/xm/encoding/encoding_def.h \
	src/xm/encoding/external.h \
	src/xm/encoding/multibyte.cpp \
	src/xm/encoding/multibyte.h \
	src/xm/encoding/gb18030.cpp \
	src/xm/encoding/gb18030.h \
	src/xm/encoding/singlebyte.cpp \
	src/xm/encoding/singlebyte.h \
	src/xm/encoding/unicode.cpp \
	src/xm/encoding/unicode.h \
	src/xm/line_enc_adapter.h

encdet_src =	src/xm/encdet.cpp \
	src/xm/encdet.h \
	src/xm/mad_encdet.cpp \
	src/xm/mad_encdet.h

bin_PROGRAMS = wxmedit
check_PROGRAMS = wxmedit_test
TESTS = wxmedit_test
wxmedit_LDADD = ${curl_LIBS}
wxmedit_CXXFLAGS = -DDATA_DIR=\"${datadir}\" ${curl_CFLAGS}
wxmedit_SOURCES =	$(enc_src) \
	$(encdet_src) \
	src/dialog/wxm_conv_enc_dialog.cpp \
	src/dialog/wxm_conv_enc_dialog.h \
	src/dialog/wxm_enumeration_dialog.cpp \
	src/dialog/wxm_enumeration_dialog.h \
	src/dialog/wxm_find_in_files_dialog.cpp \
	src/dialog/wxm_find_in_files_dialog.h \
	src/dialog/wxm_highlighting_dialog.cpp \
	src/dialog/wxm_highlighting_dialog.h \
	src/dialog/wxm_purge_histories_dialog.cpp \
	src/dialog/wxm_purge_histories_dialog.h \
	src/dialog/wxm_search_replace_dialog.cpp \
	src/dialog/wxm_search_replace_dialog.h \
	src/dialog/wxm_sort_dialog.cpp \
	src/dialog/wxm_sort_dialog.h \
	src/dialog/wxm_word_count_dialog.cpp \
	src/dialog/wxm_word_count_dialog.h \
	src/dialog/wxmedit_about_dialog.cpp \
	src/dialog/wxmedit_about_dialog.h \
	src/dialog/wxmedit_options_dialog.cpp \
	src/dialog/wxmedit_options_dialog.h \
	src/mad_utils.cpp \
	src/mad_utils.h \
	src/wxm/case_conv.cpp \
	src/wxm/case_conv.h \
	src/wxm/choice_map.hpp \
	src/wxm/def.h \
	src/wxm/edit/inframe.cpp \
	src/wxm/edit/inframe.h \
	src/wxm/edit/simple.cpp \
	src/wxm/edit/simple.h \
	src/wxm/edit/single_line.cpp \
	src/wxm/edit/single_line.h \
	src/wxm/recent_list.cpp \
	src/wxm/recent_list.h \
	src/wxm/searcher.cpp \
	src/wxm/searcher.h \
	src/wxm/status_bar.cpp \
	src/wxm/status_bar.h \
	src/wxm/update.cpp \
	src/wxm/update.h \
	src/wxm/utils.cpp \
	src/wxm/utils.h \
	src/wxm/wx_avoid_wxmsw_bug4373.h \
	src/wxm/wx_icu.cpp \
	src/wxm/wx_icu.h \
	src/wxm_command.h \
	src/wxm_plugin.cpp \
	src/wxm_plugin.h \
	src/wxm_printout.cpp \
	src/wxm_printout.h \
	src/wxmedit/caret_new.cpp \
	src/wxmedit/caret_new.h \
	src/wxmedit/clipbrd_gtk.cpp \
	src/wxmedit/clipbrd_gtk.h \
	src/wxmedit/trad_simp.cpp \
	src/wxmedit/trad_simp.h \
	src/wxmedit/wxm_lines.cpp \
	src/wxmedit/wxm_lines.h \
	src/wxmedit/wxm_syntax.cpp \
	src/wxmedit/wxm_syntax.h \
	src/wxmedit/wxm_undo.cpp \
	src/wxmedit/wxm_undo.h \
	src/wxmedit/wxmedit.cpp \
	src/wxmedit/wxmedit.h \
	src/wxmedit/wxmedit_advanced.cpp \
	src/wxmedit/wxmedit_basic.cpp \
	src/wxmedit/wxmedit_command.cpp \
	src/wxmedit/wxmedit_command.h \
	src/wxmedit/wxmedit_gtk.cpp \
	src/wxmedit_app.cpp \
	src/wxmedit_app.h \
	src/wxmedit_frame.cpp \
	src/wxmedit_frame.h \
	src/xm/cxx11.h \
	src/xm/remote.cpp \
	src/xm/remote.h \
	src/xm/ublock.cpp \
	src/xm/ublock.h \
	src/xm/ublock_des.cpp \
	src/xm/utils.hpp \
	src/xm/uutils.cpp \
	src/xm/uutils.h

wxmedit_test_SOURCES =	$(enc_src) \
	$(encdet_src) \
	src/xm/cxx11.h \
	src/xm/utils.hpp \
	src/xm/uutils.h \
	src/xm/uutils.cpp \
	test/encdet/data_from_icudet.cpp \
	test/encdet/data_from_icudet.h \
	test/encdet/data_from_mozdet.cpp \
	test/encdet/data_from_mozdet.h \
	test/encdet/test_detenc.cpp \
	test/encdet/test_detenc.h \
	test/encdet/test_from_icudet.cpp \
	test/encdet/test_from_mozdet.cpp \
	test/encdet/test_wxmedit_encdet.cpp \
	test/encoding/data_cp20932_conv.cpp \
	test/encoding/data_cp932_conv.cpp \
	test/encoding/data_cp936_conv.cpp \
	test/encoding/data_cp949_conv.cpp \
	test/encoding/data_cp950_conv.cpp \
	test/encoding/data_gb18030_bmp2db.cpp \
	test/encoding/data_gb18030_bmp2qb.cpp \
	test/encoding/data_gb18030_db2bmp.cpp \
	test/encoding/data_gb18030_qb2bmp.cpp \
	test/encoding/data_multibyte_conv.h \
	test/encoding/data_singlebyte_conv.cpp \
	test/encoding/data_singlebyte_conv.h \
	test/encoding/test_doublebyte_conv.cpp \
	test/encoding/test_gb18030_conv.cpp \
	test/encoding/test_singlebyte_conv.cpp \
	test/encdet_test.h \
	test/encoding_test.h \
	test/test.cpp

dist_doc_DATA = ChangeLog LICENSE README.txt

pixmapsdir = $(datadir)/pixmaps
pixmaps_DATA = wxmedit.png

wxmedit.desktop: $(top_srcdir)/wxmedit.desktop._ Makefile
	if [ x"$$XDG_DATA_DIRS" == x"" ]; then \
	  sed "s#@pixmapsdir@#$(pixmapsdir)#" $(top_srcdir)/wxmedit.desktop._ > $@; \
	else \
	  cmp $(top_srcdir)/wxmedit.desktop $@ >/dev/null 2>&1 || cp $(top_srcdir)/wxmedit.desktop $@ || echo ""; \
	fi

appdir = $(datadir)/applications
app_DATA = wxmedit.desktop

EXTRA_DIST = $(pixmaps_DATA) $(app_DATA)

noinst_HEADERS =	README.txt \
	autogen.sh \
	src/wxmedit_app.rc \
	src/dialog/wxm_file_association_dialog.cpp \
	src/dialog/wxm_file_association_dialog.h \
	src/plugin_example/makefile.linux \
	src/plugin_example/makefile.mingw32 \
	src/plugin_example/makefile.vc \
	src/plugin_example/plugin.cpp \
	src/plugin_example/vc_cl.bat \
	src/plugin_example/wxwin16x16.xpm \
	images/bmkclearall.xpm \
	images/bmknext.xpm \
	images/bmkprev.xpm \
	images/bmktoggle.xpm \
	images/closeall.xpm \
	images/columnmode.xpm \
	images/comment.xpm \
	images/copy.xpm \
	images/cut.xpm \
	images/delete.xpm \
	images/diffmode.xpm \
	images/down.xpm \
	images/fileclose.xpm \
	images/fileopen.xpm \
	images/filesaveas.xpm \
	images/filesave.xpm \
	images/findnext.xpm \
	images/findprev.xpm \
	images/find.xpm \
	images/fontname.xpm \
	images/fontsize.xpm \
	images/font.xpm \
	images/glslmode.xpm \
	images/hexmode.xpm \
	images/icons.png \
	images/icons.svg \
	images/indent.xpm \
	images/mainicon.png \
	images/mainicon.svg \
	images/new.xpm \
	images/nowrap.xpm \
	images/null.xpm \
	images/olist.xpm \
	images/options.xpm \
	images/paste.xpm \
	images/preview.xpm \
	images/print.xpm \
	images/quit.xpm \
	images/redo.xpm \
	images/replace.xpm \
	images/saveall.xpm \
	images/texmode.xpm \
	images/textmode.xpm \
	images/uncomment.xpm \
	images/undo.xpm \
	images/unindent.xpm \
	images/wrapbycol.xpm \
	images/wrapbywin.xpm \
	images/wxmedit0_24x24.xpm \
	images/wxmedit0.xpm \
	images/wxmedit1.ico \
	images/wxmedit.ico \
	images/wxmedit.xpm \
	po/wxmedit.pot \
	po/src.list \
	po/wxstd.po.cpp \
	po/xgettext.bat \
	po/xgettext_join.bat


DOMAIN = wxmedit
podir = $(top_srcdir)/po
LINGUAS = @ALL_LINGUAS@
