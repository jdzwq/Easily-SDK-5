/***********************************************************************
	Easily xdk v5.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc memory file system call document

	@module	_if_block.c | memory file system call macos implement file

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

#include "xdkiml.h"

#ifdef XDK_SUPPORT_FILE_BLOCK

res_file_t _block_open(const tchar_t* fname, u32_t fmode)
{
    int fd = 0;
    int flag = 0;
    
    if (fmode & FILE_OPEN_APPEND)
        flag = O_CREAT | O_RDWR | O_APPEND;
    else if(fmode & FILE_OPEN_CREATE)
        flag = O_CREAT | O_RDWR | O_TRUNC;
    else if(fmode & FILE_OPEN_WRITE)
        flag = O_RDWR;
    else
        flag = O_RDONLY;
    
    if(fmode & FILE_OPEN_OVERLAP)
        flag |= O_NONBLOCK;
    
    fd = open(fname, flag, S_IRWXU | S_IXGRP | S_IROTH | S_IXOTH);
    if(fd < 0)
        return INVALID_FILE;
    
    return (res_file_t)fd;
}

bool_t _block_size(res_file_t fh, u32_t* ph, u32_t* pl)
{
    return _file_size(fh, ph, pl);
}

void _block_close(res_file_t fh)
{
    close(fh);
}

bool_t _block_write(res_file_t fh, u32_t hoff, u32_t loff, void* buf, size_t size, size_t* pcb)
{
    return _file_write_range(fh, hoff, loff, buf, size, pcb);
}

bool_t _block_read(res_file_t fh, u32_t hoff, u32_t loff, void* buf, size_t size, size_t* pcb)
{
    return _file_read_range(fh, hoff, loff, buf, size, pcb);
}

bool_t _block_truncate(res_file_t fh, u32_t hoff, u32_t loff)
{
    size_t len;
    
    len = MAKESIZE(loff, hoff);
    
    if (ftruncate(fh, len) < 0)
        return 0;
    
    lseek(fh, len, SEEK_SET);
    return 1;
}

#endif //XDK_SUPPORT_BLOCK