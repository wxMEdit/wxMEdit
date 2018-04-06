# Created by: JiaYanwei <wxmedit@gmail.com>
# $FreeBSD$

PORTNAME=	wxmedit
PORTVERSION=	3.1
CATEGORIES=	editors
MASTER_SITES=	SF/${PORTNAME}/${PORTVERSION}
DISTNAME=	wxMEdit-${PORTVERSION}

MAINTAINER=	JiaYanwei <wxmedit@gmail.com>
COMMENT=	wxMEdit: Cross-platform Text/Hex Editor, Improved Version of MadEdit

LICENSE=	GPLv3

USES=		gettext pkgconfig desktop-file-utils
USE_WX=		3.0+
WANT_UNICODE=	yes

LIB_DEPENDS+=	libboost_system.so:${PORTSDIR}/devel/boost-libs \
		libcurl.so:${PORTSDIR}/ftp/curl \
		libicudata.so:${PORTSDIR}/devel/icu \
		libicui18n.so:${PORTSDIR}/devel/icu \
		libicuuc.so:${PORTSDIR}/devel/icu

GNU_CONFIGURE=	yes
CONFIGURE_ARGS+=--with-wx-config=${WX_CONFIG}
CONFIGURE_ENV+=	PKG_CONFIG="${PKG_CONFIG}" \
		glib_gtk2_CFLAGS="${glib_gtk2_CFLAGS}" \
		glib_gtk2_LIBS="${glib_gtk2_LIBS}"

CPPFLAGS+=	-I/usr/local/include
CFLAGS+=	-I/usr/local/include
CXXFLAGS+=	-I/usr/local/include
LDFLAGS+=	-L/usr/local/lib

PKG_CONFIG?=	${LOCALBASE}/bin/pkgconf
glib_gtk2_CFLAGS=`${PKG_CONFIG} --cflags gtk+-2.0`
glib_gtk2_LIBS=	`${PKG_CONFIG} --libs gtk+-2.0`

BROKEN_sparc64=	yes

.include <bsd.port.mk>
