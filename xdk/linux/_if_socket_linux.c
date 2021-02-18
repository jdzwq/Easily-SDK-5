/***********************************************************************
	Easily xdk v5.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc socket document

	@module	if_socket_win.c | socket macos implement file

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

#ifdef XDK_SUPPORT_SOCK

bool_t _host_addr(const schar_t* host, schar_t* addr)
{
    int i;
    struct hostent *hos;
    struct in_addr *addr_list;
    
    hos = gethostbyname((char*)host);
    if (!hos)
        return 0;
    
    i = 0;
    if (hos->h_addrtype == AF_INET)
    {
        while (hos->h_addr_list[i] != 0) {
            addr_list = (struct in_addr *)hos->h_addr_list[i++];
            xscpy(addr, inet_ntoa(*addr_list));
            return 1;
        }
    }
    
    return 0;
}


void _fill_addr(net_addr_t* paddr, unsigned short port, const schar_t* saddr)
{
    paddr->sin_family = AF_INET;
    paddr->sin_port = htons((u_short)port);
    if (!saddr || !saddr[0])
        paddr->sin_addr.s_addr = INADDR_ANY;
    else
        paddr->sin_addr.s_addr = inet_addr(saddr);
}

void _conv_addr(net_addr_t* paddr, unsigned short* port, schar_t* saddr)
{
    *port = ntohs(paddr->sin_port);
    xscpy(saddr, inet_ntoa(paddr->sin_addr));
}

void _socket_addr(res_file_t so, net_addr_t* paddr)
{
    int ns;
    
    ns = sizeof(net_addr_t);
    getsockname(so, (struct sockaddr *)paddr, (socklen_t*)&ns);
}

void _socket_peer(res_file_t so, net_addr_t* paddr)
{
    int ns;
    
    ns = sizeof(net_addr_t);
    getsockname(so, (struct sockaddr *)paddr, (socklen_t*)&ns);
}

bool_t _socket_startup(void)
{
    struct sigaction sa = {0};
    
    sa.sa_handler = SIG_IGN;
    sigaction(SIGPIPE, &sa, 0);
    
	return 1;
}

void _socket_cleanup(void)
{
	return;
}

int _socket_error(tchar_t* buf, int max)
{
    xsncpy(buf, strerror(errno), max);
    return (int)xslen(buf);;
}

res_file_t _socket_open(int af, int type, int protocol)
{
    res_file_t so;
    int opt = 1;
    
    so = socket(af, type, protocol);
   
    return (so < 0) ? INVALID_FILE : so;
}

res_file_t _socket_tcp(int ver, dword_t flag)
{
    res_file_t so;
    
    so = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(so < 0)
        return INVALID_FILE;
    
    if(flag & FILE_OPEN_OVERLAP)
    {
        ioctl(so, FIONBIO, 1);
    }
    
    return so;
}

res_file_t _socket_udp(int ver, dword_t flag)
{
    res_file_t so;
    
    so = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(so < 0)
        return INVALID_FILE;

    
    if(flag & FILE_OPEN_OVERLAP)
    {
        ioctl(so, FIONBIO, 1);
    }
    
    return so;
}

res_file_t _socket_icmp(int ver, dword_t flag)
{
    res_file_t so;
    
    so = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if(so < 0)
        return INVALID_FILE;
    
    
    if(flag & FILE_OPEN_OVERLAP)
    {
        ioctl(so, FIONBIO, 1);
    }
    
    return so;
}

void _socket_shutdown(res_file_t so, int how)
{
	shutdown(so, how);
}

void _socket_close(res_file_t so)
{
	close(so);
}

dword_t _socket_wait(res_file_t so, dword_t msk, int ms)
{
    fd_set fr, fw;
    struct timeval tv = { 0 };
    dword_t ev = 0;
    
    FD_ZERO(&fr);
    FD_SET(so, &fr);
    
    FD_ZERO(&fw);
    FD_SET(so, &fw);
    
    tv.tv_sec = ms / 1000;
    tv.tv_usec = (ms % 1000) * 1000;
    
    if(((msk & FD_ACCEPT) || (msk & FD_READ)) && (msk & FD_WRITE))
        select(so + 1, &fr, &fw, NULL, &tv);
    else if(((msk & FD_ACCEPT) || (msk & FD_READ)))
        select(so + 1, &fr, NULL, NULL, &tv);
    else
        return 0;
    
    if((msk & FD_ACCEPT) && FD_ISSET(so, &fr))
        ev |= FD_ACCEPT;
    else if((msk & FD_READ) && FD_ISSET(so, &fr))
        ev |= FD_READ;
    
    if((msk & FD_WRITE) && FD_ISSET(so, &fw))
        ev |= FD_WRITE;
        
    return ev;
}

bool_t _socket_bind(res_file_t so, res_addr_t saddr, int slen)
{
	return (bind(so, saddr, slen) == 0) ? 1 : 0;
}

bool_t _socket_connect(res_file_t so, res_addr_t saddr, int slen)
{
	return (connect(so, saddr, slen) == 0) ? 1 : 0;
}

bool_t _socket_sendto(res_file_t so, res_addr_t saddr, int alen, void* buf, dword_t size, async_t* pb)
{
    dword_t* pcb = (pb) ? &(pb->size) : NULL;
    struct timeval tv = {0};

    int rs, rt;
    struct epoll_event ev = {0};
    fd_set fs = {0};

    if (pb->type == ASYNC_QUEUE)
    {
        ev.events = EPOLLOUT;
        ev.data.fd = so; 

        epoll_ctl(pb->port, EPOLL_CTL_ADD, so, &ev); 
        rs = epoll_wait(pb->port, &ev, 1, (int)pb->timo);
        epoll_ctl(pb->port, EPOLL_CTL_DEL, so, &ev); 
        
        if(rs < 0)
        {
            *pcb = 0;
            return 0;
        }else if(rs == 0)
        {
            *pcb = 0;
            return 1;
        }
        
        rt = (int)size;
    }
    else if (pb->type == ASYNC_EVENT)
    {
        FD_ZERO(&fs);
        FD_SET(so, &fs);
        
        tv.tv_sec = pb->timo / 1000;
        tv.tv_usec = (pb->timo % 1000) * 1000;
        
        rs = select(so + 1, NULL, &fs, NULL, &tv);
        FD_CLR(so, &fs);

        if(rs < 0)
        {
            *pcb = 0;
            return 0;
        }else if(rs == 0)
        {
            *pcb = 0;
            return 1;
        }
        
        rt = (int)size;
    }else
    {
        rt = (int)size;
    }
    
    rt = (int)sendto(so, (char*)buf, (size_t)rt, MSG_NOSIGNAL, (struct sockaddr*)saddr, (socklen_t)alen);
    if (rt < 0)
    {
        if(errno != EINPROGRESS)
        {
            if (pcb)  *pcb = 0;
            return 0;
        }else
        {
            rt = 0;
        }
    }
    
    if (pcb) *pcb = (dword_t)rt;
    
    return (rt > 0)? 1 : 0;
}

bool_t _socket_recvfrom(res_file_t so, res_addr_t saddr, int* plen, void* buf, dword_t size, async_t* pb)
{
    dword_t* pcb = (pb) ? &(pb->size) : NULL;
    struct timeval tv = {0};

    int rs, rt;
    socklen_t len = 0;    
    struct epoll_event ev = {0};
    fd_set fs = {0};

    if (pb->type == ASYNC_QUEUE)
    {
        ev.events = EPOLLIN;
        ev.data.fd = so; 

        epoll_ctl(pb->port, EPOLL_CTL_ADD, so, &ev); 
        rs = epoll_wait(pb->port, &ev, 1, (int)pb->timo);
        epoll_ctl(pb->port, EPOLL_CTL_DEL, so, &ev); 

        if(rs < 0)
        {
            *pcb = 0;
            return 0;
        }else if(rs == 0)
        {
            *pcb = 0;
            return 1;
        }
        
        if(ioctl(so, FIONREAD, &rt) < 0)
            rt = (int)size;
        else
            rt = (rt < (int)size)? rt : (int)size;
    }
    else if (pb->type == ASYNC_EVENT)
    {
        FD_ZERO(&fs);
        FD_SET(so, &fs);
        
        tv.tv_sec = pb->timo / 1000;
        tv.tv_usec = (pb->timo % 1000) * 1000;
        
        rs = select(so + 1, &(fs), NULL, NULL, &tv);
        FD_CLR(so, &fs);

        if(rs < 0)
        {
            *pcb = 0;
            return 0;
        }else if(rs == 0)
        {
            *pcb = 0;
            return 1;
        }
        
        if(ioctl(so, FIONREAD, &rt) < 0)
            rt = (int)size;
        else
            rt = (rt < (int)size)? rt : (int)size;
    }else
    {
        rt = (int)size;
    }
    
    rt = (int)recvfrom(so, (char*)buf, (size_t)rt, MSG_NOSIGNAL, (struct sockaddr*)saddr, (socklen_t*)&len);
    *plen = (int)len;
    
    if (rt < 0)
    {
        if(errno != EINPROGRESS)
        {
            if (pcb)  *pcb = 0;
            return 0;
        }
        else
        {
            rt = 0;
        }
        
    }
    
    if (pcb) *pcb = (dword_t)rt;
    
    return (rt > 0)? 1 : 0;
}

bool_t _socket_send(res_file_t so, void* buf, dword_t size, async_t* pb)
{
    dword_t* pcb = (pb) ? &(pb->size) : NULL;
    struct timeval tv = {0};
    
    int rs, rt;
    struct epoll_event ev = {0};
    fd_set fs = {0};

    if (pb->type == ASYNC_QUEUE)
    {
        ev.events = EPOLLOUT;
        ev.data.fd = so; 

        epoll_ctl(pb->port, EPOLL_CTL_ADD, so, &ev); 
        rs = epoll_wait(pb->port, &ev, 1, (int)pb->timo);
        epoll_ctl(pb->port, EPOLL_CTL_DEL, so, &ev); 

        if(rs < 0)
        {
            *pcb = 0;
            return 0;
        }else if(rs == 0)
        {
            *pcb = 0;
            return 1;
        }
        
        rt = (int)size;
    }
    else  if (pb->type == ASYNC_EVENT)
    {
        FD_ZERO(&fs);
        FD_SET(so, &fs);
        
        tv.tv_sec = pb->timo / 1000;
        tv.tv_usec = (pb->timo % 1000) * 1000;
        
        rs = select(so + 1, NULL, &(fs), NULL, &tv);
        FD_CLR(so, &fs);

        if(rs < 0)
        {
            *pcb = 0;
            return 0;
        }else if(rs == 0)
        {
            *pcb = 0;
            return 1;
        }
        
        rt = (int)size;
    }else
    {
        rt = (int)size;
    }

    rt = (int)send(so, (char*)buf, rt, MSG_NOSIGNAL);
    if (rt < 0)
    {
        if(errno != EINPROGRESS)
        {
            if (pcb)  *pcb = 0;
            return 0;
        }
        else
        {
            rt = 0;
        }
        
    }
    
    if (pcb) *pcb = (dword_t)rt;
    
    return 1;
}

bool_t _socket_recv(res_file_t so, void* buf, dword_t size, async_t* pb)
{
    dword_t* pcb = (pb) ? &(pb->size) : NULL;
    struct timeval tv = {0};

    int rs, rt;
    struct epoll_event ev = {0};
    fd_set fs = {0};

    if (pb->type == ASYNC_QUEUE)
    {
        ev.events = EPOLLIN;
        ev.data.fd = so; 

        epoll_ctl(pb->port, EPOLL_CTL_ADD, so, &ev);      
        rs = epoll_wait(pb->port, &ev, 1, (int)pb->timo);
        epoll_ctl(pb->port, EPOLL_CTL_DEL, so, &ev); 

        if(rs < 0)
        {
            *pcb = 0;
            return 0;
        }else if(rs == 0)
        {
            *pcb = 0;
            return 1;
        }
        
        if(ioctl(so, FIONREAD, &rt) < 0)
            rt = (int)size;
        else
            rt = (rt < (int)size)? rt : (int)size;
    }
    else if (pb->type == ASYNC_EVENT)
    {
        FD_ZERO(&fs);
        FD_SET(so, &fs);
        
        tv.tv_sec = pb->timo / 1000;
        tv.tv_usec = (pb->timo % 1000) * 1000;
        
        rs = select(so + 1, &(fs), NULL, NULL, &tv);
        FD_CLR(so, &fs);

        if(rs < 0)
        {
            *pcb = 0;
            return 0;
        }else if(rs == 0)
        {
            *pcb = 0;
            return 1;
        }
        
        if(ioctl(so, FIONREAD, &rt) < 0)
            rt = (int)size;
        else
            rt = (rt < (int)size)? rt : (int)size;
    }else
    {
        rt = (int)size;
    }
    
    rt = (int)recv(so, (char*)buf, rt, MSG_NOSIGNAL);
    if (rt < 0)
    {
        if(errno != EINPROGRESS)
        {
            if (pcb)  *pcb = 0;
            return 0;
        }
        else
        {
            rt = 0;
        }
        
    }
    
    if (pcb) *pcb = (dword_t)rt;
    
    return 1;
}

int _socket_type(res_file_t so)
{
    int type = 0;
    int len = sizeof(int);
    
    getsockopt(so, SOL_SOCKET, SO_TYPE, (char*)&type, (socklen_t*)&len);
    
    return type;
}

bool_t	_socket_setopt(res_file_t so, int optname, const char* optval, int optlen)
{
	return (setsockopt(so, SOL_SOCKET, optname, optval, optlen) < 0) ? 0 : 1;
}

bool_t	_socket_getopt(res_file_t so, int optname, char* pval, int* plen)
{
	return (getsockopt(so, SOL_SOCKET, optname, pval, (socklen_t*)plen) < 0) ? 0 : 1;
}

bool_t _socket_set_sndbuf(res_file_t so, int size)
{
    return (setsockopt(so, SOL_SOCKET, SO_SNDBUF, (const char*)&size, sizeof(int)) < 0) ? 0 : 1;
}

bool_t _socket_set_rcvbuf(res_file_t so, int size)
{
    return (setsockopt(so, SOL_SOCKET, SO_RCVBUF, (const char*)&size, sizeof(int)) < 0) ? 0 : 1;
}

bool_t _socket_set_sndtmo(res_file_t so, int tmo)
{
    struct timeval tv;
    
    tv.tv_sec = tmo / 1000;
    tv.tv_usec = (tmo % 1000) * 1000;
    
    return (setsockopt(so, SOL_SOCKET, SO_SNDTIMEO, (const char*)&tv, sizeof(tv)) < 0) ? 0 : 1;
}

bool_t _socket_set_rcvtmo(res_file_t so, int tmo)
{
    struct timeval tv;
    
    tv.tv_sec = tmo / 1000;
    tv.tv_usec = (tmo % 1000) * 1000;
    
    return (setsockopt(so, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv)) < 0) ? 0 : 1;
}

bool_t _socket_set_linger(res_file_t so, bool_t wait, int sec)
{
    struct linger li;
    
    li.l_onoff = (wait) ? 1 : 0;
    li.l_linger = (unsigned short)sec;
    
    return (setsockopt(so, SOL_SOCKET, SO_LINGER, (const char*)&li, sizeof(li)) < 0) ? 0 : 1;
}

bool_t	_socket_set_nonblk(res_file_t so, bool_t none)
{
    //unsigned long dw = (none)? 1 : 0;

    //return (ioctl(so, FIONBIO, ((none)? 1 : 0)) < 0)? 0 : 1;

    int flags;
    
    flags = fcntl(so, F_GETFL, 0);

    if(!(flags & O_NONBLOCK) && none)
    {
        return (fcntl(so, F_SETFL, flags | O_NONBLOCK) < 0)? 0 : 1;
    }

    if((flags & O_NONBLOCK) && !none)
    {
        return (fcntl(so, F_SETFL, (flags & (~O_NONBLOCK))) < 0)? 0 : 1;
    }

    return (1);
}

bool_t _socket_get_nonblk(res_file_t so)
{
    int flags;
    
    flags = fcntl(so, F_GETFL, 0);

    return (flags & O_NONBLOCK)? 1 : 0;
}

bool_t _socket_listen(res_file_t so, int max)
{
	return (listen(so, max) == 0)? 1 : 0;
}

res_file_t _socket_accept(res_file_t so, res_addr_t saddr, int *plen, async_t* pb)
{
    res_file_t po;
    struct timeval tv = {0};
    
    int rs, rt;  
    socklen_t nlen = 0;
     struct epoll_event ev = {0};
    fd_set fs = {0};

    if (pb->type == ASYNC_QUEUE)
    {
        ev.events = EPOLLIN;
        ev.data.fd = so; 

        epoll_ctl(pb->port, EPOLL_CTL_ADD, so, &(ev)); 
        rs = epoll_wait(pb->port, &ev, 1, (int)pb->timo);
        epoll_ctl(pb->port, EPOLL_CTL_DEL, so, &ev); 

        if(rs <= 0)
        {
            return INVALID_FILE;
        }
    }
    else if (pb->type == ASYNC_EVENT)
    {
        FD_ZERO(&fs);
        FD_SET(so, &fs);
        
        tv.tv_sec = pb->timo / 1000;
        tv.tv_usec = (pb->timo % 1000) * 1000;
        
        rs = select(so + 1, &(fs), NULL, NULL, &tv);
        FD_CLR(so, &fs);
        
        if(rs <= 0)
        {
            return INVALID_FILE;
        }
    }
    
    po = accept(so, saddr, (socklen_t*)&nlen);
    if(po < 0)
    {
        *plen = 0;
        return INVALID_FILE;
    }
    
    ioctl(po, FIONBIO, 1);
    
    *plen = (int)nlen;
    
    return (res_file_t)po;
}

int _socket_write(void* pso, unsigned char* buf, int len)
{
    ssize_t rt;
    int pos = 0;
    
    while (pos < len)
    {
        rt = send(*((int*)pso), (char*)(buf + pos), len - pos, MSG_NOSIGNAL);
        if (rt < 0)
        {
            return (-1);
        }
        if (!rt) continue;
        
        pos += rt;
    }
    
    return pos;
}

int _socket_read(void* pso, unsigned char* buf, int len)
{
    ssize_t rt;
    int pos = 0;
    
    while (pos < len)
    {
        rt = recv(*((int*)pso), (char*)(buf + pos), len - pos, MSG_NOSIGNAL);
        if (rt < 0)
        {
            return (-1);
        }
        if (!rt) continue;
        
        pos += rt;
    }
    
    return pos;
}

bool_t _socket_share(pid_t procid, res_file_t procfd, res_file_t so, void* data, dword_t size)
{
    struct msghdr msg;
    struct cmsghdr* pcms;
    struct iovec vec;
    char nch[2] = {0};
    
    char buf[CMSG_SPACE(sizeof(int))] = {0};
    
    int rt = 0;
    fd_set fw;
    struct timeval tmo = {3, 0};
    
    FD_ZERO(&fw);
    FD_SET(procfd, &fw);
    
    if(select(procfd + 1, NULL, &fw, NULL, &tmo) <= 0)
    {
        return 0;
    }
    
    vec.iov_base = (data)? (char*)data : nch;
    vec.iov_len = (data)? size : 1;
    
    msg.msg_name = NULL;
    msg.msg_namelen = 0;
    msg.msg_iov= &vec;
    msg.msg_iovlen = 1;
    msg.msg_flags = 0;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);
    
    pcms = CMSG_FIRSTHDR(&msg);
    pcms->cmsg_level = SOL_SOCKET;
    pcms->cmsg_type = SCM_RIGHTS;
    pcms->cmsg_len = CMSG_LEN(sizeof(int));
    *((int*)CMSG_DATA(pcms)) = so;

    return (sendmsg(procfd, &msg, MSG_NOSIGNAL) > 0)? 1 : 0;
}

res_file_t _socket_dupli(res_file_t procfd, void* data, dword_t* pcb)
{
    struct msghdr msg;
    struct cmsghdr* pcms;
    struct iovec vec;
    char nch[2] = {0};
    int so;
    
    char buf[CMSG_SPACE(sizeof(int))] = {0};
    
    int rt = 0;
    fd_set fr;
    struct timeval tmo = {3, 0};
    
    FD_ZERO(&fr);
    FD_SET(procfd, &fr);
    
    if(select(procfd + 1, &fr, NULL, NULL, &tmo) <= 0)
    {
        return INVALID_FILE;
    }

    vec.iov_base = (data)? (char*)data : nch;
    vec.iov_len = (data)? *pcb : 1;

    msg.msg_name = NULL;
    msg.msg_namelen = 0;
    msg.msg_iov= &vec;
    msg.msg_iovlen = 1;
    msg.msg_flags = 0;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);

    if(recvmsg(procfd, &msg, MSG_NOSIGNAL) < 1)
        return INVALID_FILE;
    
    pcms = CMSG_FIRSTHDR(&msg);
    if(pcms->cmsg_level != SOL_SOCKET || pcms->cmsg_type != SCM_RIGHTS)
        return INVALID_FILE;
    
    so = *((int*)CMSG_DATA(pcms));

    if(!so || so == INVALID_FILE)
    {
        return INVALID_FILE;
    }
    
    ioctl(so, FIONBIO, 1);
    
    return so;
}


#endif