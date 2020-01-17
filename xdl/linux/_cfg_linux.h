/***********************************************************************
	Easily xdl v5.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc config document

	@module	xdlconf.h | platform config interface file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
	@devnote 张文权 2017.01 - 2017.12	v5.0
***********************************************************************/

/**********************************************************************
This program is free software : you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
LICENSE.GPL3 for more details.
***********************************************************************/

#ifndef _XDLCFG_H
#define	_XDLCFG_H

#if defined(XDK_SUPPORT_FILE) && (defined(_DEBUG) || defined(DEBUG))
#define XDL_SUPPORT_MEMO_DUMP
#endif

#define GPL_SUPPORT_ACP
#define GPL_SUPPORT_PNG
#define GPL_SUPPORT_JPG
#define GPL_SUPPORT_ZIP

#define XDL_SUPPORT_PDG
#define XDL_SUPPORT_BMP
#define XDL_SUPPORT_BAR
#define XDL_SUPPORT_MPI
#define XDL_SUPPORT_DHM
#define XDL_SUPPORT_RAND
#define XDL_SUPPORT_CRYPT
#define XDL_SUPPORT_CERT
#define XDL_SUPPORT_HLL
#define XDL_SUPPORT_LZF
#define XDL_SUPPORT_SIP
#define XDL_SUPPORT_MUR
#define XDL_SUPPORT_ASN
#define XDL_SUPPORT_SSL

#define XDL_SUPPORT_DOC
#define XDL_SUPPORT_VIEW

#if defined(XDK_SUPPORT_FILE)
#define XDL_SUPPORT_BIO
#endif

/*CHINA LANGUAGE*/
#define LANG_CN


#endif	/* _XDLCFG_H */

