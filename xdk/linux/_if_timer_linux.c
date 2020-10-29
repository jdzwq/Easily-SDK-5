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

typedef struct _timer_token_t{
	int fd;
	PF_TIMERFUNC pf;
	void* pa;
}timer_token_t;

typedef struct _timer_queue_t{
	int fd;

	bool_t active;
	timer_token_t tt[MAX_TIMER_LISTEN];

	res_even_t ev;
}timer_queue_t;

void* _timer_listen(void* param)
{
	timer_queue_t* pqt = (timer_queue_t*)param;
	int fd_cnt,fd;
	struct epoll_event evs[MAX_TIMER_LISTEN] = {0}; 
	int i,j;   
	uint64_t exp = 0;

	while (pqt->active)
	{
		fd_cnt = epoll_wait(pqt->fd, evs, MAX_TIMER_LISTEN, 512);

		for (i = 0; i < fd_cnt; i++)
		{
			fd = evs[i].data.fd;
			if (evs[i].events & EPOLLIN)
			{
				read(fd, &exp, sizeof(uint64_t));

				for(j = 0;j<MAX_TIMER_LISTEN;j++)
				{
					if(fd == pqt->tt[j].fd)
					{
						(*(pqt->tt[j].pf))(pqt->tt[j].pa, 0);
					}
				}
			}
		}
	}

	_event_sign(pqt->ev, 1);

	_thread_end();
}

res_queue_t _create_timer_queue()
{
	int fd;
	timer_queue_t* pqt;
	res_thread_t th;

	fd = epoll_create(MAX_TIMER_LISTEN); 
	if(fd < 0) return (res_queue_t)0;

	pqt = (timer_queue_t*)calloc(1, sizeof(timer_queue_t));
	pqt->fd = fd;
	pqt->active = 1;
	pqt->ev = _event_create();

	_thread_begin(&th, _timer_listen, (void*)pqt);

	return (res_queue_t)pqt;
}

void _destroy_timer_queue(res_queue_t rq)
{
	timer_queue_t* pqt = (timer_queue_t*)rq;
	int i;

	pqt->active = 0;

	_event_wait(pqt->ev, -1);

	for(i = 0;i<MAX_TIMER_LISTEN;i++)
	{
		if(pqt->tt[i].fd)
		{
			close(pqt->tt[i].fd);
		}
	}

	if(pqt->fd)
		close((int)pqt->fd);

	free(pqt);
}

res_timer_t _create_timer(res_queue_t rq, dword_t duetime, dword_t period, PF_TIMERFUNC pf, void* pa)
{
	timer_queue_t* pqt = (timer_queue_t*)rq;
	int fd;
	struct itimerspec its = {0};
	struct epoll_event event = {0};
	int i;

	its.it_value.tv_sec = duetime / 1000;
	its.it_value.tv_nsec = 0;
	its.it_interval.tv_sec = period / 1000;
	its.it_interval.tv_nsec = 0;
   
	fd = timerfd_create(CLOCK_MONOTONIC, 0);
	if (fd < 0) return (res_timer_t)0;

	if (timerfd_settime(fd, 0, &its, NULL) < 0)
	{
		close(fd);
		return (res_timer_t)0;
	}

	event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    if (epoll_ctl(pqt->fd, EPOLL_CTL_ADD, fd, &event) < 0)
    {
        close(fd);
		return (res_timer_t)0;
    }

	for(i = 0;i<MAX_TIMER_LISTEN;i++)
	{
		if(pqt->tt[i].fd == 0)
		{
			pqt->tt[i].fd = fd;
			pqt->tt[i].pf = pf;
			pqt->tt[i].pa = pa;
			
			break;
		}
	}

	return (res_timer_t)(&(pqt->tt[i]));
}

void _destroy_timer(res_queue_t rq, res_timer_t rt)
{
	timer_queue_t* pqt = (timer_queue_t*)rq;
	timer_token_t* ptt = (timer_token_t*)rt;
	struct epoll_event event = {0};
	int i;

	event.data.fd = rt;
	event.events = EPOLLIN;
    epoll_ctl(pqt->fd, EPOLL_CTL_DEL, ptt->fd, &event);

	for(i = 0;i<MAX_TIMER_LISTEN;i++)
	{
		if(ptt->fd == pqt->tt[i].fd)
		{
			pqt->tt[i].fd = 0;
			pqt->tt[i].pf = NULL;
			pqt->tt[i].pa = NULL;
		}
	}

	close(ptt->fd);
}

bool_t _alter_timer(res_queue_t rq, res_timer_t rt, dword_t duetime, dword_t period)
{
	timer_token_t* ptt = (timer_token_t*)rt;
	struct itimerspec its = {0};

	its.it_value.tv_sec = duetime / 1000;
	its.it_value.tv_nsec = 0;
	its.it_interval.tv_sec = period / 1000;
	its.it_interval.tv_nsec = 0;

	return (timerfd_settime((int)(ptt->fd), 0, &its, NULL) < 0)? 0 : 1;
}

#endif //XDK_SUPPORT_TIMER
