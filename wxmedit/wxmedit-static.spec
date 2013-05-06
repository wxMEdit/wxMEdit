%define release 1
%define prefix /usr
%define name wxmedit
%define version 2.9.1


Summary: wxmedit : GTK+2 based text/hex editor
Name: %{name}
Version: %{version}
Release: %{release}
License: GPL
Vendor: JiaYanwei
URL: http://code.google.com/p/wxmedit/
Group: Applications/Editors
Source0: %{name}-%{version}.tar.gz
Packager: JiaYanwei <wxmedit@gmail.com>
BuildRoot: %{_builddir}/%{name}-%{version}-root

BuildRequires: gtk2-devel >= 2.6.0
Requires: gtk2 >= 2.6.0

Docdir: %{prefix}/share/doc

%description
wxMEdit is a cross-platform Text/Hex Editor.
wxMEdit is a fork of MadEdit which has been discontinued.
wxMEdit supports many useful functions, e.g. SyntaxHighlightings,
WordWrap, Encodings, Column/Hex Modes.

%prep
%setup

%build

%configure --with-wx-config=${HOME}/wxWidgets/RELEASE/wx-config LIBS="${HOME}/wxWidgets/RELEASE/lib/libwx_gtk2u_core-2.8.a ${HOME}/wxWidgets/RELEASE/lib/libwx_baseu-2.8.a -lgtk-x11-2.0 -lgthread-2.0 /usr/X11R6/lib/libXinerama.a /usr/X11R6/lib/libXxf86vm.a /usr/X11R6/lib/libSM.a /usr/X11R6/lib/libICE.a" glib_gtk2_LIBS="-lm" WX_LIBS="-lm"
make

%install
rm -rf $RPM_BUILD_ROOT

%makeinstall

%clean
rm -rf $RPM_BUILD_ROOT


%files
%defattr(-,root,root,0755)
%doc README.txt
%{_bindir}/*
%{_datadir}/wxmedit/*
%{_datadir}/pixmaps/*
%{_datadir}/applications/*
%{_datadir}/locale/zh_TW/LC_MESSAGES/wxmedit.mo
%{_datadir}/locale/zh_CN/LC_MESSAGES/wxmedit.mo
%{_datadir}/locale/it_IT/LC_MESSAGES/wxmedit.mo
%{_datadir}/locale/ja_JP/LC_MESSAGES/wxmedit.mo


%changelog
