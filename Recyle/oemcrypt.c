/***********************************************************************
	Easily xdl v5.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc encrypt document

	@module	oemcrypt.c | encrypt implement file

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

#include "oemcrypt.h"
#include "imperr.h"
#include "impmem.h"

#ifdef XDL_SUPPORT_CRYPT

#include "../md5/md5.h"
#include "../sha/sha1.h"
#include "../sha/sha2.h"
#include "../sha/sha4.h"
#include "../crc/crc.h"
#include "../aes/aes.h"
#include "../des/des.h"
#include "../arc/arc4.h"
#include "../rsa/rsa.h"


#ifdef _OS_WINDOWS
#pragma comment(lib,"md5.lib")
#pragma comment(lib,"sha.lib")
#pragma comment(lib,"crc.lib")
#pragma comment(lib,"aes.lib")
#pragma comment(lib,"des.lib")
#pragma comment(lib,"arc.lib")
#pragma comment(lib, "rsa.lib")
#endif


/********************************************************************************************************************/

/***********************************************************************************************************************/


#endif