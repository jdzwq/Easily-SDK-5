/***********************************************************************
	Easily xdk v5.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc console system call document

	@module	_if_console.c | console system call macos implement file

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

#ifdef XDK_SUPPORT_CONS

res_file_t _cons_alloc(tchar_t* cname, int max)
{
    int ttym;
    char tty_name[PATH_LEN] = {0};
    int len;
    pid_t pid;
    
    /*ttym = posix_openpt(O_RDWR);
    if(ttym < 0)
        return INVALID_FILE;
    
    strcpy(tty_name, ptsname(ttym));
    
    if(grantpt(ttym) < 0)
    {
        close(ttym);
        return INVALID_FILE;
    }
    
    if(unlockpt(ttym) < 0)
    {
        close(ttym);
        return INVALID_FILE;
    }
    
    ttys = open(tty_name,O_RDWR);
    if(ttys < 0)
    {
        close(ttym);
        return INVALID_FILE;
    }*/
    
    /*if(openpty(&ttym, &ttys, tty_name, NULL, NULL) < 0)
        return INVALID_FILE;*/
    
    pid = forkpty(&ttym, tty_name, NULL, NULL);
    if(pid < 0)
    {
        return INVALID_FILE;
    }
    else if(pid == 0)
    {
        close(ttym);
        return INVALID_FILE;
    }else
    {
        len = (int)strlen(tty_name);
        len = (len < max)? len : max;
        strncpy(cname, tty_name, len);
        cname[len] = '\0';

        return ttym;
    }
}

void _cons_free(res_file_t ch)
{
    close(ch);
}

bool_t _cons_sigaction(res_file_t con, PF_SIGHANDLER pf)
{
    struct sigaction sa = {0};
    
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = pf;
    sa.sa_flags = SA_SIGINFO | SA_RESETHAND;
    
    return (sigaction(SIGTERM, &sa, 0) < 0)? 0 : 1;
}

res_file_t _cons_stdin(res_file_t ch)
{
    return (res_file_t)STDIN_FILENO;
}

res_file_t _cons_stdout(res_file_t ch)
{
    return (res_file_t)STDOUT_FILENO;
}

bool_t _cons_write(res_file_t ch, void* buf, size_t len, size_t* pb)
{
    int ttym;
    
    ttym = STDOUT_FILENO;
    if(!ttym)
    {
        if(pb)
            *pb = 0;
        return 0;
    }
    
    if(write(ttym, buf, len) != len)
    {
        if(pb)
            *pb = 0;
        return 0;
    }else
    {
        if(pb)
            *pb = len;
        return 1;
    }
}

bool_t _cons_flush(res_file_t ch)
{
    return 1;
}

bool_t _cons_read(res_file_t ch, void* buf, size_t size, size_t* pb)
{
    int ttys;
    int rt;
    
    ttys = STDIN_FILENO;
    if(!ttys)
    {
        if(pb)
            *pb = 0;
        return 0;
    }
    
    //rt = fcntl(ttys, F_GETFL, 0);
    //rt |= O_NONBLOCK;
    //rt = fcntl(ttys, F_SETFL, rt);
    
    rt = (int)read(ttys, buf, size);
    if(rt < 0)
    {
        if(pb)
            *pb = 0;
        return 0;
    }else
    {
        if(pb)
            *pb = rt;
        return 1;
    }
}

#endif //XDK_SUPPORT_CONS