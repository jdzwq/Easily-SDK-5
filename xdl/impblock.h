/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc memory block document

	@module	impblock.h | interface file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
	@devnote 张文权 2017.01 - 2017.12	v5.0
	@devnote 张文权 2018.01 - 2018.12	v5.5
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

#ifndef _IMPBLOCK_H
#define	_IMPBLOCK_H

#include "xdldef.h"

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION xblock_open: open a memory block.
@INPUT byte_t** pp: the variant bytes object.
@RETURN xhand_t: if succeeds return block handle, fails return NULL.
*/
EXP_API xhand_t xblock_open(byte_t** pp);

/*
@FUNCTION xblock_close: close a memory block.
@INPUT xhand_t block: the block handle.
@RETURN byte_t**: the variant bytes object.
*/
EXP_API byte_t** xblock_close(xhand_t blk);

/*
@FUNCTION xblock_handle: get a block handle, the handle is buffer pointer.
@INPUT xhand_t blk: the block handle.
@RETURN byte_t**: if succeeds return buffer pointer, fails return NULL.
*/
EXP_API byte_t** xblock_handle(xhand_t blk);

/*
@FUNCTION xblock_write: write data into block.
@INPUT xhand_t blk: the block handle.
@INPUT const byte_t* data: the data buffer pointer.
@INOUTPUT dword_t* pb: the integer buffer pointer holding byte count for writing, and return the actually byte count writed.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xblock_write(xhand_t blk, const byte_t* data, dword_t* pb);

/*
@FUNCTION xblock_read: read data from block.
@INPUT xhand_t blk: the block handle.
@INPUT byte_t* data: the data buffer pointer.
@INOUTPUT dword_t* pb: the integer buffer pointer for holding byte count for reading, and return the actually byte count readed.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
EXP_API bool_t xblock_read(xhand_t blk, byte_t* buf, dword_t* pb);


#ifdef	__cplusplus
}
#endif

#endif	/*XDLBLOCK_H */

