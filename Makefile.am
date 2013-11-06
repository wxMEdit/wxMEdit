SUBDIRS = syntax po

enc_src =	src/xm/wxm_encoding/cp20932.cpp \
	src/xm/wxm_encoding/cp20932.h \
	src/xm/wxm_encoding/doublebyte.cpp \
	src/xm/wxm_encoding/doublebyte.h \
	src/xm/wxm_encoding/encoding.cpp \
	src/xm/wxm_encoding/encoding.h \
	src/xm/wxm_encoding/encoding_def.h \
	src/xm/wxm_encoding/multibyte.cpp \
	src/xm/wxm_encoding/multibyte.h \
	src/xm/wxm_encoding/gb18030.cpp \
	src/xm/wxm_encoding/gb18030.h \
	src/xm/wxm_encoding/singlebyte.cpp \
	src/xm/wxm_encoding/singlebyte.h \
	src/xm/wxm_encoding/unicode.cpp \
	src/xm/wxm_encoding/unicode.h

encdet_src =	src/wxmedit/mad_encdet.cpp \
	src/wxmedit/mad_encdet.h \
	src/xm/wxm_encdet.cpp \
	src/xm/wxm_encdet.h

bin_PROGRAMS = wxmedit wxmedit_test
wxmedit_CXXFLAGS = -DDATA_DIR=\"${datadir}\"
wxmedit_SOURCES =	$(enc_src) \
	$(encdet_src) \
	src/wxmedit_about_dialog.cpp \
	src/wxmedit_about_dialog.h \
	src/wxm_command.h \
	src/wxm_conv_enc_dialog.cpp \
	src/wxm_conv_enc_dialog.h \
	src/wxmedit_app.cpp \
	src/wxmedit_app.h \
	src/wxmedit_frame.cpp \
	src/wxmedit_frame.h \
	src/wxm_find_in_files_dialog.cpp \
	src/wxm_find_in_files_dialog.h \
	src/wxm_highlighting_dialog.cpp \
	src/wxm_highlighting_dialog.h \
	src/wxm_options_dialog.cpp \
	src/wxm_options_dialog.h \
	src/wxm_plugin.cpp \
	src/wxm_printout.cpp \
	src/wxm_printout.h \
	src/wxm_replace_dialog.cpp \
	src/wxm_replace_dialog.h \
	src/wxm_search_dialog.cpp \
	src/wxm_search_dialog.h \
	src/wxm_sort_dialog.cpp \
	src/wxm_sort_dialog.h \
	src/wxm_utils.cpp \
	src/wxm_utils.h \
	src/wxm_word_count_dialog.cpp \
	src/wxm_word_count_dialog.h \
	src/plugin.h \
	src/wxmedit/caret_new.cpp \
	src/wxmedit/caret_new.h \
	src/wxmedit/clipbrd_gtk.cpp \
	src/wxmedit/clipbrd_gtk.h \
	src/wxmedit/wxm_deque.hpp \
	src/wxmedit/wxmedit.cpp \
	src/wxmedit/wxmedit.h \
	src/wxmedit/wxmedit_advanced.cpp \
	src/wxmedit/wxmedit_basic.cpp \
	src/wxmedit/wxmedit_command.cpp \
	src/wxmedit/wxmedit_command.h \
	src/wxmedit/wxmedit_search.cpp \
	src/wxmedit/wxmedit_gtk.cpp \
	src/wxmedit/wxm_lines.cpp \
	src/wxmedit/wxm_lines.h \
	src/wxmedit/wxm_syntax.cpp \
	src/wxmedit/wxm_syntax.h \
	src/wxmedit/wxm_undo.cpp \
	src/wxmedit/wxm_undo.h \
	src/xm/xm_ublock_des.cpp \
	src/xm/xm_ublock.cpp \
	src/xm/xm_ublock.h \
	src/xm/xm_uutils.cpp \
	src/xm/xm_uutils.h \
	src/xm/wx_avoid_wxmsw_bug4373.h \
	src/xm/wx_recent_list.cpp \
	src/xm/wx_recent_list.h \
	src/xm/wxm_case_conv.cpp \
	src/xm/wxm_case_conv.h \
	src/xm/wxm_def.h \
	src/xm/wxm_line_enc_adapter.cpp \
	src/xm/wxm_line_enc_adapter.h \
	src/xm/wxm_wx_icu.cpp \
	src/xm/wxm_wx_icu.h \
	src/wxmedit/trad_simp.cpp \
	src/wxmedit/trad_simp.h \
	src/wxmedit/ucs4_t.h

wxmedit_test_SOURCES =	$(enc_src) \
	$(encdet_src) \
	src/wxmedit/ucs4_t.h \
	src/wxmedit/wxm_deque.hpp \
	src/xm/wxm_line_enc_adapter.h \
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

wxmedit_test_LDFLAGS = @TESTLIBS@

pixmapsdir = $(datadir)/pixmaps
pixmaps_DATA = wxmedit.xpm

appdir = $(datadir)/applications
app_DATA = wxmedit.desktop

EXTRA_DIST = $(pixmaps_DATA) $(app_DATA)

noinst_HEADERS =	description-pak \
	README.txt \
	autogen.sh \
	wxmedit.spec \
	wxmedit-static.spec \
	debian/changelog \
	debian/control \
	debian/copyright \
	debian/menu \
	debian/rules \
	freebsd/pkg-comment \
	freebsd/pkg-descr \
	freebsd/pkg-plist \
	freebsd/pkgcreate.txt \
	src/wxmedit_app.rc \
	src/wxm_file_association_dialog.cpp \
	src/wxm_file_association_dialog.h \
	src/plugin_example/makefile.linux \
	src/plugin_example/makefile.mingw32 \
	src/plugin_example/makefile.vc \
	src/plugin_example/plugin.cpp \
	src/plugin_example/vc_cl.bat \
	src/plugin_example/wxwin16x16.xpm \
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
	images/madedit.ico \
	images/madedit.xpm \
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
	images/wxmedit.ico \
	images/wxmedit.xpm \
	images/wxmedit_16x15.xpm \
	libunicows/libunicows.a \
	libunicows/README.txt \
	libunicows/unicows.lib \
	po/wxmedit.pot \
	po/src.list \
	po/wxstd.po.cpp \
	po/xgettext.bat \
	po/xgettext_join.bat


DOMAIN = wxmedit
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
