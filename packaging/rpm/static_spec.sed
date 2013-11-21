/%configure%/s##& --with-wx-config=${HOME}/wxWidgets/release/bin/wx-config LIBS="${HOME}/wxWidgets/release/lib/libwx_gtk2u_aui-2.8.a ${HOME}/wxWidgets/release/lib/libwx_gtk2u_core-2.8.a ${HOME}/wxWidgets/release/lib/libwx_baseu-2.8.a -lgtk-x11-2.0 -lgthread-2.0 -lX11" glib_gtk2_LIBS="-lm" WX_LIBS="-lm"#
/BuildRequires:/s#^.*$#BuildRequires: gtk2-devel >= 2.6.0, libicu-devel >= 4.2.1#
/Requires:/s#^.*$#Requires: gtk2 >= 2.6.0, libicu >= 4.2.1#
