# Created by: JiaYanwei <wxmedit@gmail.com>
# $FreeBSD$

PORTNAME=	wxmedit
PORTVERSION=	@VER
CATEGORIES=	editors
MASTER_SITES=	SF/${PORTNAME}/${PORTVERSION}
DISTNAME=	wxMEdit-${PORTVERSION}

MAINTAINER=	JiaYanwei <wxmedit@gmail.com>
COMMENT=	wxMEdit: Cross-platform Text/Hex Editor, Improved Version of MadEdit

LICENSE=	GPLv3

USES=		gettext pkgconfig desktop-file-utils
USE_WX=		3.2+
WANT_UNICODE=	yes

LIB_DEPENDS+=	libboost_system.so:devel/boost-libs \
		libcurl.so:ftp/curl \
		libicudata.so:devel/icu \
		libicui18n.so:devel/icu \
		libicuuc.so:devel/icu

GNU_CONFIGURE=	yes
CONFIGURE_ARGS+=--with-wx-config=${WX_CONFIG}
CONFIGURE_ENV+=	PKG_CONFIG="${PKG_CONFIG}" \
		glib_gtk_CFLAGS="${glib_gtk_CFLAGS}" \
		glib_gtk_LIBS="${glib_gtk_LIBS}"

CPPFLAGS+=	-I/usr/local/include
CFLAGS+=	-I/usr/local/include
CXXFLAGS+=	-I/usr/local/include
LDFLAGS+=	-L/usr/local/lib

PKG_CONFIG?=	${LOCALBASE}/bin/pkgconf
glib_gtk_CFLAGS=`${PKG_CONFIG} --cflags glib-2.0 gtk+-3.0`
glib_gtk_LIBS=	`${PKG_CONFIG} --libs glib-2.0 gtk+-3.0`

BROKEN_sparc64=	yes

.include <bsd.port.mk>
