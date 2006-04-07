SUBDIRS = syntax po

bin_PROGRAMS = madedit
madedit_CXXFLAGS = -I$(top_srcdir)/wxFlatNotebook/include -DDATA_DIR=\"${datadir}\" 
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
	src/MadUtils.cpp \
	src/MadUtils.h \
	src/MadWordCountDialog.cpp \
	src/MadWordCountDialog.h \
	src/plugin.h \
	src/MadEdit/caret_new.cpp \
	src/MadEdit/caret_new.h \
	src/MadEdit/clipbrd_gtk.cpp \
	src/MadEdit/clipbrd_gtk.h \
	src/MadEdit/MadEdit.cpp \
	src/MadEdit/MadEdit.h \
	src/MadEdit/MadEditAdvanced.cpp \
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
	wxFlatNotebook/src/wxFlatNotebook/wxFlatNotebook.cpp \
	wxFlatNotebook/include/wx/wxFlatNotebook/wxFlatNotebook.h \
	wxFlatNotebook/include/wx/wxFlatNotebook/wxFNBDropTarget.h

pixmapsdir = $(datadir)/pixmaps
pixmaps_DATA = madedit.xpm

appdir = $(datadir)/applications
app_DATA = madedit.desktop

EXTRA_DIST = $(pixmaps_DATA) $(app_DATA)

noinst_HEADERS =	description-pak \
	README.txt \
	autogen.sh \
	chmod_644_all_files.sh \
	debian/changelog \
	debian/control \
	debian/copyright \
	debian/menu \
	debian/rules \
	src/MadEditApp.rc \
	src/MadAboutDialog.wxform \
	src/MadConvEncDialog.wxform \
	src/MadEditFrame.wxform \
	src/MadFindInFilesDialog.wxform \
	src/MadHighlightingDialog.wxform \
	src/MadOptionsDialog.wxform \
	src/MadReplaceDialog.wxform \
	src/MadSearchDialog.wxform \
	src/MadWordCountDialog.wxform \
	src/plugin_example/makefile.linux \
	src/plugin_example/makefile.mingw32 \
	src/plugin_example/makefile.vc \
	src/plugin_example/plugin.cpp \
	src/plugin_example/vc_cl.bat \
	src/plugin_example/wxwin16x16.xpm \
	wxFlatNotebook/README \
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
	images/Mad.xpm \
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
	makefiles/Makefile.debug \
	makefiles/Makefile.linux \
	makefiles/Makefile.mac \
	makefiles/Makefile.mingw \
	makefiles/Makefile.install \
	makefiles/zcfg_wx_debug.sh \
	makefiles/zcfg_wx_release.sh \
	makefiles/zcfg_madedit_release.sh \
    po/madedit.pot \
	po/src.list \
	po/wxstd.po.txt \
	po/wxstd.po.cpp \
	po/xgettext.bat \
	po/xgettext_join.bat \
	vs.net2003/build_debug.bat \
	vs.net2003/build_release.bat \
	vs.net2003/MadEdit.sln \
	vs.net2003/MadEdit.vcproj \
	xpressive/boost/xpressive/basic_regex.hpp \
	xpressive/boost/xpressive/detail/core/access.hpp \
	xpressive/boost/xpressive/detail/core/action.hpp \
	xpressive/boost/xpressive/detail/core/action_state.hpp \
	xpressive/boost/xpressive/detail/core/adaptor.hpp \
	xpressive/boost/xpressive/detail/core/finder.hpp \
	xpressive/boost/xpressive/detail/core/flow_control.hpp \
	xpressive/boost/xpressive/detail/core/icase.hpp \
	xpressive/boost/xpressive/detail/core/linker.hpp \
	xpressive/boost/xpressive/detail/core/matcher/action_matcher.hpp \
	xpressive/boost/xpressive/detail/core/matcher/alternate_end_matcher.hpp \
	xpressive/boost/xpressive/detail/core/matcher/alternate_matcher.hpp \
	xpressive/boost/xpressive/detail/core/matcher/any_matcher.hpp \
	xpressive/boost/xpressive/detail/core/matcher/assert_bol_matcher.hpp \
	xpressive/boost/xpressive/detail/core/matcher/assert_bos_matcher.hpp \
	xpressive/boost/xpressive/detail/core/matcher/assert_eol_matcher.hpp \
	xpressive/boost/xpressive/detail/core/matcher/assert_eos_matcher.hpp \
	xpressive/boost/xpressive/detail/core/matcher/assert_line_base.hpp \
	xpressive/boost/xpressive/detail/core/matcher/assert_word_matcher.hpp \
	xpressive/boost/xpressive/detail/core/matcher/charset_matcher.hpp \
	xpressive/boost/xpressive/detail/core/matcher/end_matcher.hpp \
	xpressive/boost/xpressive/detail/core/matcher/epsilon_matcher.hpp \
	xpressive/boost/xpressive/detail/core/matcher/keeper_matcher.hpp \
	xpressive/boost/xpressive/detail/core/matcher/literal_matcher.hpp \
	xpressive/boost/xpressive/detail/core/matcher/logical_newline_matcher.hpp \
	xpressive/boost/xpressive/detail/core/matcher/lookahead_matcher.hpp \
	xpressive/boost/xpressive/detail/core/matcher/lookbehind_matcher.hpp \
	xpressive/boost/xpressive/detail/core/matcher/mark_begin_matcher.hpp \
	xpressive/boost/xpressive/detail/core/matcher/mark_end_matcher.hpp \
	xpressive/boost/xpressive/detail/core/matcher/mark_matcher.hpp \
	xpressive/boost/xpressive/detail/core/matcher/posix_charset_matcher.hpp \
	xpressive/boost/xpressive/detail/core/matcher/range_matcher.hpp \
	xpressive/boost/xpressive/detail/core/matcher/regex_byref_matcher.hpp \
	xpressive/boost/xpressive/detail/core/matcher/regex_matcher.hpp \
	xpressive/boost/xpressive/detail/core/matcher/repeat_begin_matcher.hpp \
	xpressive/boost/xpressive/detail/core/matcher/repeat_end_matcher.hpp \
	xpressive/boost/xpressive/detail/core/matcher/set_matcher.hpp \
	xpressive/boost/xpressive/detail/core/matcher/simple_repeat_matcher.hpp \
	xpressive/boost/xpressive/detail/core/matcher/string_matcher.hpp \
	xpressive/boost/xpressive/detail/core/matcher/true_matcher.hpp \
	xpressive/boost/xpressive/detail/core/matchers.hpp \
	xpressive/boost/xpressive/detail/core/optimize.hpp \
	xpressive/boost/xpressive/detail/core/peeker.hpp \
	xpressive/boost/xpressive/detail/core/quant_style.hpp \
	xpressive/boost/xpressive/detail/core/regex_impl.hpp \
	xpressive/boost/xpressive/detail/core/results_cache.hpp \
	xpressive/boost/xpressive/detail/core/state.hpp \
	xpressive/boost/xpressive/detail/core/sub_match_impl.hpp \
	xpressive/boost/xpressive/detail/core/sub_match_vector.hpp \
	xpressive/boost/xpressive/detail/detail_fwd.hpp \
	xpressive/boost/xpressive/detail/dynamic/dynamic.hpp \
	xpressive/boost/xpressive/detail/dynamic/matchable.hpp \
	xpressive/boost/xpressive/detail/dynamic/parser.hpp \
	xpressive/boost/xpressive/detail/dynamic/parser_enum.hpp \
	xpressive/boost/xpressive/detail/dynamic/parser_traits.hpp \
	xpressive/boost/xpressive/detail/dynamic/parse_charset.hpp \
	xpressive/boost/xpressive/detail/static/as_xpr.hpp \
	xpressive/boost/xpressive/detail/static/compile.hpp \
	xpressive/boost/xpressive/detail/static/is_pure.hpp \
	xpressive/boost/xpressive/detail/static/modifier.hpp \
	xpressive/boost/xpressive/detail/static/placeholders.hpp \
	xpressive/boost/xpressive/detail/static/productions/alt_compilers.hpp \
	xpressive/boost/xpressive/detail/static/productions/charset_transforms.hpp \
	xpressive/boost/xpressive/detail/static/productions/complement_compiler.hpp \
	xpressive/boost/xpressive/detail/static/productions/domain_tags.hpp \
	xpressive/boost/xpressive/detail/static/productions/independent_compiler.hpp \
	xpressive/boost/xpressive/detail/static/productions/marker_compiler.hpp \
	xpressive/boost/xpressive/detail/static/productions/marker_transform.hpp \
	xpressive/boost/xpressive/detail/static/productions/modify_compiler.hpp \
	xpressive/boost/xpressive/detail/static/productions/noop_compiler.hpp \
	xpressive/boost/xpressive/detail/static/productions/productions.hpp \
	xpressive/boost/xpressive/detail/static/productions/quant_compilers.hpp \
	xpressive/boost/xpressive/detail/static/productions/quant_traits.hpp \
	xpressive/boost/xpressive/detail/static/productions/quant_transforms.hpp \
	xpressive/boost/xpressive/detail/static/productions/set_compilers.hpp \
	xpressive/boost/xpressive/detail/static/productions/transmogrify.hpp \
	xpressive/boost/xpressive/detail/static/productions/visitor.hpp \
	xpressive/boost/xpressive/detail/static/regex_operators.hpp \
	xpressive/boost/xpressive/detail/static/static.hpp \
	xpressive/boost/xpressive/detail/static/type_traits.hpp \
	xpressive/boost/xpressive/detail/static/width_of.hpp \
	xpressive/boost/xpressive/detail/utility/algorithm.hpp \
	xpressive/boost/xpressive/detail/utility/any.hpp \
	xpressive/boost/xpressive/detail/utility/boyer_moore.hpp \
	xpressive/boost/xpressive/detail/utility/chset/basic_chset.hpp \
	xpressive/boost/xpressive/detail/utility/chset/basic_chset.ipp \
	xpressive/boost/xpressive/detail/utility/chset/chset.hpp \
	xpressive/boost/xpressive/detail/utility/chset/range_run.hpp \
	xpressive/boost/xpressive/detail/utility/chset/range_run.ipp \
	xpressive/boost/xpressive/detail/utility/cons.hpp \
	xpressive/boost/xpressive/detail/utility/dont_care.hpp \
	xpressive/boost/xpressive/detail/utility/hash_peek_bitset.hpp \
	xpressive/boost/xpressive/detail/utility/ignore_unused.hpp \
	xpressive/boost/xpressive/detail/utility/literals.hpp \
	xpressive/boost/xpressive/detail/utility/never_true.hpp \
	xpressive/boost/xpressive/detail/utility/save_restore.hpp \
	xpressive/boost/xpressive/detail/utility/sequence_stack.hpp \
	xpressive/boost/xpressive/detail/utility/tracking_ptr.hpp \
	xpressive/boost/xpressive/detail/utility/traits_utils.hpp \
	xpressive/boost/xpressive/match_results.hpp \
	xpressive/boost/xpressive/proto/arg_traits.hpp \
	xpressive/boost/xpressive/proto/compiler/branch.hpp \
	xpressive/boost/xpressive/proto/compiler/conditional.hpp \
	xpressive/boost/xpressive/proto/compiler/error.hpp \
	xpressive/boost/xpressive/proto/compiler/fold.hpp \
	xpressive/boost/xpressive/proto/compiler/switch.hpp \
	xpressive/boost/xpressive/proto/compiler/transform.hpp \
	xpressive/boost/xpressive/proto/operators.hpp \
	xpressive/boost/xpressive/proto/op_base.hpp \
	xpressive/boost/xpressive/proto/op_tags.hpp \
	xpressive/boost/xpressive/proto/proto.hpp \
	xpressive/boost/xpressive/proto/proto_fwd.hpp \
	xpressive/boost/xpressive/proto/proto_typeof.hpp \
	xpressive/boost/xpressive/regex_algorithms.hpp \
	xpressive/boost/xpressive/regex_compiler.hpp \
	xpressive/boost/xpressive/regex_constants.hpp \
	xpressive/boost/xpressive/regex_error.hpp \
	xpressive/boost/xpressive/regex_iterator.hpp \
	xpressive/boost/xpressive/regex_primitives.hpp \
	xpressive/boost/xpressive/regex_token_iterator.hpp \
	xpressive/boost/xpressive/regex_traits.hpp \
	xpressive/boost/xpressive/sub_match.hpp \
	xpressive/boost/xpressive/traits/cpp_regex_traits.hpp \
	xpressive/boost/xpressive/traits/c_regex_traits.hpp \
	xpressive/boost/xpressive/traits/detail/c_ctype.hpp \
	xpressive/boost/xpressive/traits/null_regex_traits.hpp \
	xpressive/boost/xpressive/xpressive.hpp \
	xpressive/boost/xpressive/xpressive_dynamic.hpp \
	xpressive/boost/xpressive/xpressive_fwd.hpp \
	xpressive/boost/xpressive/xpressive_static.hpp \
	xpressive/boost/xpressive/xpressive_typeof.hpp \
	xpressive/LICENSE_1_0.txt \
	xpressive/README.TXT


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
