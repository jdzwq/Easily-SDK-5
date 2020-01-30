/***********************************************************************
	Easily xdk v5.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc pipe system call document

	@module	_if_pipe.c | pipe system call macos implement file

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

#ifdef XDK_SUPPORT_PIPE

#define PIPE_HEAD		_T("/tmp/")

res_file_t _pipe_srv(const tchar_t* pname, dword_t fmode)
{
    int flag, rt;
    char ppath[1024] = {0};
    
    if (_tstrnull(pname))
    {
        if(fmode & FILE_OPEN_WRITE)
             return STDOUT_FILENO;
         else
             return STDIN_FILENO;
    }
    
    sprintf(ppath, "%s%s", PIPE_HEAD, pname);
    
    if (0 == access(ppath, F_OK))
    {
        unlink(ppath);
    }
    
    if(0 != mkfifo(ppath, 0666))
        return INVALID_FILE;
    
    if(fmode & FILE_OPEN_WRITE)
        flag = O_WRONLY;
    else
        flag = O_RDONLY;
    
    if(fmode & FILE_OPEN_OVERLAP)
        flag |= O_NONBLOCK;
    
    rt = open(ppath, flag);
    
    return (rt < 0)? INVALID_FILE : (res_file_t)rt;
}

bool_t _pipe_listen(res_file_t pip, async_t* pb)
{
    LPOVERLAPPED pov = (pb)? (LPOVERLAPPED)pb->lapp : NULL;
    
    int rs;
    struct epoll_event ev = {0};

    if (pb->type == ASYNC_QUEUE)
    {
        pov->ev.events = EPOLLIN;
        pov->ev.data.fd = pip; 
        epoll_ctl(pb->port, EPOLL_CTL_MOD, pip, &(pov->ev)); 
        
        rs = epoll_wait(pb->port, &ev, 1, (int)pb->timo);
        if(rs <= 0)
        {
            return 0;
        }
    }
    else if (pb->type == ASYNC_EVENT)
    {
        FD_ZERO(&(pov->fd[0]));
        FD_SET(pip, &(pov->fd[0]));
        
        pov->tv.tv_sec = pb->timo / 1000;
        pov->tv.tv_usec = (pb->timo % 1000) * 1000;
        
        rs = select(pip + 1, &(pov->fd[0]), NULL, NULL, &(pov->tv));
        if(rs <= 0)
        {
            return 0;
        }
    }
    
    return 1;
}

void _pipe_stop(res_file_t pip)
{
    
}

void _pipe_close(const tchar_t* pname, res_file_t pip)
{
    char ppath[1024] = {0};
    
    if(!_tstrnull(pname))
        sprintf(ppath, "%s%s", PIPE_HEAD, pname);
    
    close(pip);
    
    if(!_tstrnull(pname))
        unlink(ppath);
}

res_file_t _pipe_cli(const tchar_t* pname, dword_t fmode)
{
    int flag, rt;
    char ppath[1024] = {0};
    
     if(_tstrnull(pname))
     {
         if(fmode & FILE_OPEN_WRITE)
             return STDOUT_FILENO;
         else
             return STDIN_FILENO;
     }
    
    sprintf(ppath, "%s%s", PIPE_HEAD, pname);
    
    if (access(ppath, F_OK) != 0)
        return INVALID_FILE;
    
    if(fmode & FILE_OPEN_WRITE)
        flag = O_WRONLY;
    else
        flag = O_RDONLY;
    
    if(fmode & FILE_OPEN_OVERLAP)
        flag |= O_NONBLOCK;

    rt = open(ppath, flag);
    
    return (rt < 0)? INVALID_FILE : (res_file_t)rt;
}

wait_t _pipe_wait(const tchar_t* pname, int ms)
{
    char ppath[1024] = {0};
    
    if(_tstrnull(pname))
    {
        return WAIT_RET;
    }

    sprintf(ppath, "%s%s", PIPE_HEAD, pname);
 
    while (ms && access(ppath, F_OK) != 0)
    {
        ms--;
        usleep(1000);
    }
    return (ms)? WAIT_RET : WAIT_TMO;
}

bool_t _pipe_write(res_file_t pipe, void* buf, dword_t len, async_t* pb)
{
    return _file_write(pipe, buf, len, pb);
}

bool_t _pipe_flush(res_file_t pipe)
{
    return _file_flush(pipe);
}

bool_t _pipe_read(res_file_t pipe, void* buf, dword_t size, async_t* pb)
{
    return _file_read(pipe, buf, size, pb);
}

#endif //XDK_SUPPORT_PIPE