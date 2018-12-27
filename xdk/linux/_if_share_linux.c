/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc memory file system call document

	@module	_if_share.c | memory file system call macos implement file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
	@devnote 张文权 2017.01 - 2017.12	v5.0
	@devnote 张文权 2018.01 - 2018.12	v5.5
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

#ifdef XDK_SUPPORT_SHARE

res_file_t _share_srv(const tchar_t* sname, const tchar_t* fpath, size_t size)
{
    int sd = 0, fd = 0;
    void* buf = NULL;
    size_t len;
    struct stat st = {0};
    void *p = NULL;
    
    fd = open(fpath, O_RDONLY, S_IRWXU | S_IXGRP | S_IROTH | S_IXOTH);
    if(fd < 0)
        goto ERRRET;
    
    fstat(fd, &st);
    len =st.st_size;
    buf = calloc(1, len);
    
    if(read(fd, buf, len) != len)
        goto ERRRET;
    
    close(fd);
    fd = 0;
    
    sd = shm_open(sname, O_CREAT | O_RDWR, S_IRWXU | S_IXGRP | S_IROTH | S_IXOTH);
    if(sd < 0)
        goto ERRRET;
    
    if(ftruncate(sd, size)< 0)
        goto ERRRET;
    
    p = mmap(NULL, size, PROT_WRITE | PROT_READ, MAP_SHARED, sd, 0);
    if(p == MAP_FAILED)
        goto ERRRET;
    
    memcpy(p, buf, len);
    free(buf);
    buf = NULL;
    
    munmap(p, size);
    p = NULL;
    
    return (res_file_t)sd;
ERRRET:
    if(fd)
        close(fd);
    
    if(buf)
        free(buf);
    
    if(p)
        munmap(p, size);
    
    if(sd)
    {
        close(sd);
        shm_unlink(sname);
    }
    
    return INVALID_FILE;
}

void _share_close(const tchar_t* sname, res_file_t fh)
{
    close(fh);
    shm_unlink(sname);
}

res_file_t _share_cli(const tchar_t* sname, size_t size)
{
    int sd = 0;
    struct stat st = {0};
    
    sd = shm_open(sname, O_RDWR, S_IRWXU | S_IXGRP | S_IROTH | S_IXOTH);
    
    if(fstat(sd, &st) < 0)
    {
        close(sd);
        shm_unlink(sname);
        
        return INVALID_FILE;
    }
    
    if(st.st_size < (size_t)size)
    {
        if(ftruncate(sd, size) < 0)
        {
            close(sd);
            shm_unlink(sname);
            
            return INVALID_FILE;
        }
    }

    return (res_file_t)sd;
}

bool_t _share_write(res_file_t fh, size_t off, void* buf, size_t size, size_t* pcb)
{
    void* p = NULL;
    size_t len;
    struct stat st = {0};
    
    u32_t loff, hoff, poff;
    size_t dlen;
    
    hoff = GETSIZEH(off);
    loff = GETSIZEL(off);
    
    poff = (loff % PAGE_GRAN);
    loff = (loff / PAGE_GRAN) * PAGE_GRAN;
    dlen = poff + size;

    if(fstat(fh, &st) < 0)
    {
        if (pcb) *pcb = 0;
        return 0;
    }

    len = off + size;
    if(st.st_size < len)
    {
        if(ftruncate(fh, len) < 0)
        {
            if (pcb) *pcb = 0;
            return 0;
        }
    }
    
    p = mmap(NULL, dlen, PROT_WRITE | PROT_READ, MAP_SHARED, fh, MAKESIZE(loff, hoff));
    if(p == MAP_FAILED)
    {
        if (pcb) *pcb = 0;
        return 0;
    }
    
    memcpy((void*)((char*)p + poff), buf, size);
    
    msync(p, dlen, MS_SYNC);
    
    munmap(p, dlen);
    
    if (pcb) *pcb = size;
    
    return 1;
}

bool_t _share_read(res_file_t fh, size_t off, void* buf, size_t size, size_t* pcb)
{
    void* p = NULL;
    
    u32_t loff, hoff, poff;
    size_t dlen;
    
    hoff = GETSIZEH(off);
    loff = GETSIZEL(off);
    
    poff = (loff % PAGE_GRAN);
    loff = (loff / PAGE_GRAN) * PAGE_GRAN;
    dlen = poff + size;
    
    p = mmap(NULL, dlen, PROT_READ, MAP_SHARED, fh, MAKESIZE(loff, hoff));
    if(p == MAP_FAILED)
    {
        if (pcb) *pcb = 0;
        return 0;
    }
    
    memcpy(buf, (void*)((char*)p + poff), size);
    
    munmap(p, dlen);
    
    if (pcb) *pcb = size;

    return 1;
}

void* _share_lock(res_file_t fh, size_t off, size_t size)
{
    void* p = NULL;
    
    u32_t loff, hoff, poff;
    size_t dlen;
    
    hoff = GETSIZEH(off);
    loff = GETSIZEL(off);
    
    poff = (loff % PAGE_GRAN);
    loff = (loff / PAGE_GRAN) * PAGE_GRAN;
    dlen = poff + size;
    
    p = mmap(NULL, dlen, PROT_READ, MAP_SHARED, fh, MAKESIZE(loff, hoff));
    if(p == MAP_FAILED)
        return NULL;

    return (void*)((char*)p + poff);
}

void _share_unlock(res_file_t fh, size_t off, size_t size, void* p)
{
    u32_t loff, hoff, poff;
    size_t dlen;
    
    hoff = GETSIZEH(off);
    loff = GETSIZEL(off);
    
    poff = (loff % PAGE_GRAN);
    loff = (loff / PAGE_GRAN) * PAGE_GRAN;
    dlen = poff + size;

    munmap((void*)((char*)p - poff), dlen);
}

#endif //XDK_SUPPORT_FILE_SHARE