/***********************************************************************
	Easily xdk v5.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc module system call document

	@module	_if_module.c | module system call macos implement file

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

#ifdef XDK_SUPPORT_PROCESS

res_modu_t _load_library(const tchar_t *lname)
{
    return dlopen(lname, RTLD_LAZY);
}

void _free_library(res_modu_t lib)
{
    dlclose(lib);
}

void *_get_address(res_modu_t lib, const tchar_t *fname)
{
    return dlsym(lib, fname);
}

void _get_runpath(res_modu_t ins, tchar_t *buf, int max)
{
    char *p;
    int size;

    size = readlink("/proc/self/exe", buf, max);
    if (size < 0)
        return;

    buf[size] = '\0';

    p = strrchr(buf, '/');
    if (p == NULL)
        return;

    *p = '\0';
}

void _process_safe()
{
    struct sigaction sa = {0};
    
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    sigaction(SIGALRM, &sa, NULL);
    
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    sigaction(SIGPROF, &sa, NULL);
    
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    sigaction(SIGPIPE, &sa, NULL);

    sigemptyset(&sa.sa_mask);
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    sigaction(SIGIO, &sa, NULL);
    
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    sigaction(SIGSYS, &sa, NULL);
}

bool_t _create_process(const tchar_t *exename, const tchar_t *cmdline, int share, proc_info_t *ppi)
{
    int fd[2] = {0};
    char cmdname[1024] = {0};
    char *token;

    if (is_null(exename))
        return 0;

    token = (char *)exename + xslen(exename);
    while (*token != '/' && *token != '\\' && token != exename)
        token--;

    if (*token == '/' || *token == '\\')
        token++;

    xscpy(cmdname, token);

    if (share == SHARE_PIPE)
    {
        if (pipe(fd) >= 0)
        {
            ppi->pip_read = fd[0];
            ppi->std_write = fd[1];
        }

        if (pipe(fd) >= 0)
        {
            ppi->std_read = fd[0];
            ppi->pip_write = fd[1];
        }
    }
    else if (share == SHARE_SOCK)
    {
        if (socketpair(AF_UNIX, SOCK_STREAM, 0, fd) >= 0)
        {
            ppi->pip_read = fd[0];
            ppi->std_write = fd[1];
        }

        if (socketpair(AF_UNIX, SOCK_STREAM, 0, fd) >= 0)
        {
            ppi->std_read = fd[0];
            ppi->pip_write = fd[1];
        }
    }

    ppi->process_id = fork();
    if (ppi->process_id < 0) //error process
    {
        if (share)
        {
            if (ppi->pip_read)
                close(ppi->pip_read);
            if (ppi->std_write)
                close(ppi->std_write);
            if (ppi->std_read)
                close(ppi->std_read);
            if (ppi->pip_write)
                close(ppi->pip_write);
        }
        return 0;
    }
    else if (ppi->process_id > 0) //parent process
    {
        if (share)
        {
            if (ppi->std_read)
                close(ppi->std_read);
            ppi->std_read = 0;

            if (ppi->std_write)
                close(ppi->std_write);
            ppi->std_write = 0;

            if (ppi->pip_read)
                close(ppi->pip_read);
            ppi->pip_read = 0;
        }

        ppi->thread_id = ppi->process_id;
        ppi->process_handle = ppi->process_id;
        ppi->thread_handle = 0;

        //waitpid(ppi->process_id, NULL, 0);

        return 1;
    }
    else //child process
    {
        if (share)
        {
            if (ppi->pip_read)
                close(ppi->pip_read);
            ppi->pip_read = 0;

            if (ppi->pip_write)
                close(ppi->pip_write);
            ppi->pip_write = 0;

            if (ppi->std_read)
                dup2(ppi->std_read, STDIN_FILENO);

            if (ppi->std_read)
                close(ppi->std_read);
            ppi->std_read = 0;

            //if(ppi->std_write)
            //dup2(ppi->std_write, STDOUT_FILENO);

            if (ppi->std_write)
                close(ppi->std_write);
            ppi->std_write = 0;
        }

        execl(exename, cmdname, cmdline, NULL);

        return 0;
    }
}

void _release_process(proc_info_t *ppi)
{
    if (ppi->pip_read)
        close(ppi->pip_read);
    if (ppi->std_write)
        close(ppi->std_write);
    if (ppi->std_read)
        close(ppi->std_read);
    if (ppi->pip_write)
        close(ppi->pip_write);

    memset((void *)ppi, 0, sizeof(proc_info_t));
}

void _process_waitrun(res_proc_t ph)
{
    int status;

    waitpid(ph, &status, WNOHANG);
}

res_file_t _process_dupli(res_proc_t ph, res_file_t vh)
{
    return INVALID_FILE;
}

void *_process_alloc(res_proc_t ph, dword_t dw)
{
    return NULL;
}

void _process_free(res_proc_t ph, void *p)
{
    return;
}

bool_t _process_write(res_proc_t ph, void *p, void *data, dword_t dw)
{
    return 0;
}

bool_t _process_read(res_proc_t ph, void *p, void *data, dword_t dw)
{
    return 0;
}

bool_t _inherit_handle(res_proc_t hh, bool_t b)
{
    return 0;
}

void _release_handle(res_file_t hh)
{
    close(hh);
}

void _read_profile(const tchar_t *fname, const tchar_t *sec, const tchar_t *key, tchar_t *buf, int max)
{
    return;
}

void _write_profile(const tchar_t *fname, const tchar_t *sec, const tchar_t *key, const tchar_t *val)
{
    return;
}

int _get_envvar(const tchar_t *ename, tchar_t *buf, int max)
{
    char *str;
    int len;

    str = getenv(ename);
    if (!str)
        return 0;

    len = xslen(str);

    len = (len < max) ? len : max;
    xsncpy(buf, str, len);
    buf[len] = _T('\0');

    return len;
}

void _system_info(sys_info_t *psi)
{
    int n;

    n = get_nprocs();
    if (n <= 0)
        n = 1;

    psi->processor_number = n;
    psi->page_size = getpagesize();
    psi->page_gran = PAGE_GRAN;
}

#endif //XDK_SUPPORT_PROCESS