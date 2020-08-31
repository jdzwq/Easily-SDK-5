/***********************************************************************
	Easily xdk v5.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc timer system call document

	@module	_if_timer.c | timer system call macos implement file

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

#ifdef XDK_SUPPORT_TIMER

#define MAX_TIMER_LISTEN	64

/*void* _timer_listen(void* param)
{
	int i,fd_cnt,fd;
	struct epoll_event events[MAX_TIMER_LISTEN];    

	memset(events, 0, sizeof(events));
	while(1) 
	 {   
		fd_cnt = epoll_wait(g_epollfd, events, MAX_TIMER_LISTEN, 512); 
		for(i = 0; i < fd_cnt; i++) 
		{   
			fd = events[i].data.fd;
			if(events[i].events & EPOLLIN) 
			{   
				if (fd == g_timerfd) 
				{
					uint64_t exp = 0;
101     
102     read(fd, &exp, sizeof(uint64_t)); 
				}   
			}   
		} 
	 }  
}*/

res_queue_t _create_timer_queue()
{
	int fd;

	fd = epoll_create(MAX_TIMER_LISTEN); 

	return (fd < 0)? (res_queue_t)0 : (res_queue_t)fd;
}

void _destroy_timer_queue(res_queue_t rq)
{
	if(rq)
		close((int)rq);
}

res_timer_t _create_timer(res_queue_t rq, dword_t duetime, dword_t period, PF_TIMERFUNC pf, void* pa)
{
	int fd;
	struct itimerspec its;
	struct epoll_event event;

	its.it_value.tv_sec = 0;
	its.it_value.tv_nsec = duetime * 1000000;
	its.it_interval.tv_sec = 0;
	its.it_interval.tv_nsec = period * 1000000;
   
	fd = timerfd_create(CLOCK_MONOTONIC, 0);
	if (fd < 0) return (res_timer_t)0;

	if (timerfd_settime(fd, 0, &its, NULL) < 0)
	{
		close(fd);
		return (res_timer_t)0;
	}

	event.data.fd = fd;
    event.events = EPOLLIN;
    if (epoll_ctl(rq, EPOLL_CTL_ADD, fd, &event) < 0)
    {
        close(fd);
		return (res_timer_t)0;
    }

	return (res_timer_t)fd;
}

void _destroy_timer(res_queue_t rq, res_timer_t rt)
{
	struct epoll_event event;

	if(rq && rt)
	{
		event.data.fd = rt;
		event.events = EPOLLIN;
    	epoll_ctl(rq, EPOLL_CTL_DEL, rt, &event);
	}

	if(rt)
	{
		close(rt);
	}
}

bool_t _alter_timer(res_queue_t rq, res_timer_t rt, dword_t duetime, dword_t period)
{
    return 0;
}

#endif //XDK_SUPPORT_TIMER
