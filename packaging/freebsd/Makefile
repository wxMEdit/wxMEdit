# Created by: JiaYanwei <wxmedit@gmail.com>
# $FreeBSD$

PORTNAME=	wxmedit
PORTVERSION=	2.9.9.3
CATEGORIES=	editors
MASTER_SITES=	SF/${PORTNAME}/${PORTVERSION}
DISTNAME=	wxMEdit-${PORTVERSION}

MAINTAINER=	JiaYanwei <wxmedit@gmail.com>
COMMENT=	wxMEdit: Cross-platform Text/Hex Editor, Improved Version of MadEdit

LICENSE=	GPLv3

USES=		gettext pkgconfig desktop-file-utils
USE_WX=		2.8+
WANT_UNICODE=	yes

LIB_DEPENDS+=	libboost_system.so:${PORTSDIR}/devel/boost-libs \
		libcurl.so:${PORTSDIR}/ftp/curl \
		libicudata.so:${PORTSDIR}/devel/icu \
		libicui18n.so:${PORTSDIR}/devel/icu \
		libicuuc.so:${PORTSDIR}/devel/icu

GNU_CONFIGURE=	yes
CONFIGURE_ARGS+=--with-wx-config=${WX_CONFIG}
CONFIGURE_ENV+=	PKG_CONFIG="${PKG_CONFIG}" glib_gtk2_CFLAGS="${glib_gtk2_CFLAGS}" glib_gtk2_LIBS="${glib_gtk2_LIBS}"
CXXFLAGS+=	${glib_gtk2_CFLAGS}

PKG_CONFIG?=	${LOCALBASE}/bin/pkgconf
glib_gtk2_CFLAGS=`${PKG_CONFIG} --cflags gtk+-2.0`
glib_gtk2_LIBS=	`${PKG_CONFIG} --libs gtk+-2.0`

BROKEN_sparc64=	yes

.include <bsd.port.mk>
