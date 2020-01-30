/***********************************************************************
	Easily xdk v5.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc thread system call document

	@module	_if_thread.c | thread system call macos implement file

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

#ifdef XDK_SUPPORT_THREAD

void _thread_create_tls(tls_key_t* pkey)
{
	 pthread_key_create(pkey, NULL);
}

void _thread_destroy_tls(tls_key_t key)
{
	 pthread_key_delete(key);
}

void* _thread_get_tls(tls_key_t key)
{
    return pthread_getspecific(key);
}

void _thread_set_tls(tls_key_t key, void* pv)
{
	pthread_setspecific(key, pv);
}

pid_t _thread_get_id(void)
{
    int tid;

    //tid = syscall(SYS_gettid);
    
    tid = (int)pthread_self();
    
    return (tid < 0)? getpid() : (pid_t)tid;
}

void _thread_safe()
{
    sigset_t mask;
    
    //sigfillset(&mask);
    sigemptyset(&mask);
    sigaddset(&mask, SIGSYS);
    sigaddset(&mask, SIGPIPE);
    sigaddset(&mask, SIGALRM);
    sigaddset(&mask, SIGPROF);
    sigaddset(&mask, SIGIO);
    //sigaddset(&mask, SIGPOLL);
    
    pthread_sigmask(SIG_BLOCK, &mask, NULL);
}

void _thread_begin(res_thread_t* ph_hand, PF_THREADFUNC pf_func, void* param)
{
    pthread_t* ptt;
    pthread_t tt = {0};
    
    if(ph_hand)
        ptt = (pthread_t*)ph_hand;
    else
        ptt = &tt;
    
    pthread_create(ptt, NULL, pf_func, param);
}

void _thread_end(void)
{
    pthread_exit(NULL);
}

void _thread_sleep(int ms)
{
    struct timeval dl;
    
    dl.tv_sec = ms / 1000;
    dl.tv_usec = (ms % 1000) * 1000;
    
    select(0, NULL, NULL, NULL, ((ms < 0)? NULL : &dl));
}

void _thread_yield()
{
    sched_yield();
}

void _thread_join(res_thread_t th)
{
    pthread_join(th, NULL);
}
/**********************************************************************************/
#ifdef XDK_SUPPORT_THREAD_EVENT
typedef struct _pthread_event_t{
    pthread_mutex_t mutx;
    pthread_cond_t cond;
    bool_t sign;
}pthread_event_t;

res_even_t _event_create()
{
    pthread_event_t* pev;
    
    pev = (pthread_event_t*)calloc(1, sizeof(pthread_event_t));
    
    pthread_mutex_init(&pev->mutx, NULL);
    pthread_cond_init(&pev->cond, NULL);
    
    return (res_even_t)pev;
}

void _event_destroy(res_even_t ev)
{
    pthread_event_t* pev = (pthread_event_t*)ev;
    
    pthread_cond_destroy(&pev->cond);
    pthread_mutex_destroy(&pev->mutx);
    
    free(pev);
}

wait_t _event_wait(res_even_t ev, int milsec)
{
    pthread_event_t* pev = (pthread_event_t*)ev;
    int err = 0;
    struct timeval tv;
    struct timespec ts;
    
    pthread_mutex_lock(&pev->mutx);

    if (milsec >= 0)
    {
        gettimeofday(&tv, NULL);

        ts.tv_sec = tv.tv_sec + (milsec / 1000);
        ts.tv_nsec = tv.tv_usec * 1000 + (milsec % 1000) * 1000000;
        if (ts.tv_nsec >= 1000000000)
        {
            ts.tv_sec++;
            ts.tv_nsec -= 1000000000;
        }
        
        while ((!pev->sign) && (err == 0))
        {
            err = pthread_cond_timedwait(&pev->cond, &pev->mutx, &ts);
        }
    }
    else
    {
        while ((!pev->sign) && (err == 0))
        {
            err = pthread_cond_wait(&pev->cond, &pev->mutx);
        }
    }
    
    pthread_mutex_unlock(&pev->mutx);
    
    return (0 == err)? WAIT_RET : WAIT_TMO;
}

void _event_sign(res_even_t ev, bool_t b_sign)
{
    pthread_event_t* pev = (pthread_event_t*)ev;

    if(b_sign)
    {
        pthread_mutex_lock(&pev->mutx);
        pev->sign = 1;
        pthread_cond_broadcast(&pev->cond);
        pthread_mutex_unlock(&pev->mutx);
    }else
    {
        pthread_mutex_lock(&pev->mutx);
        pev->sign = 0;
        pthread_mutex_unlock(&pev->mutx);
    }
}
#endif
/********************************************************************************/
#ifdef XDK_SUPPORT_THREAD_CRITI
res_crit_t _criti_create()
{
    pthread_mutex_t* pmt;
    
    pmt = (pthread_mutex_t*)calloc(1, sizeof(pthread_mutex_t));
    pthread_mutex_init(pmt, NULL);
    
    return (res_crit_t)pmt;
}

void _criti_destroy(res_crit_t cr)
{
    pthread_mutex_t* pmt = (pthread_mutex_t*)cr;

    pthread_mutex_destroy(pmt);
    free(pmt);
}

void _criti_enter(res_crit_t cr)
{
    pthread_mutex_t* pmt = (pthread_mutex_t*)cr;
    
    pthread_mutex_lock(pmt);
}

void _criti_leave(res_crit_t cr)
{
    pthread_mutex_t* pmt = (pthread_mutex_t*)cr;
    
    pthread_mutex_unlock(pmt);
}

bool_t _criti_query(res_crit_t cr)
{
    pthread_mutex_t* pmt = (pthread_mutex_t*)cr;
    
    return (0 == pthread_mutex_trylock(pmt))? 1 : 0;
}
#endif
/**********************************************************************************/
#ifdef XDK_SUPPORT_THREAD_MUTEX
res_mutx_t _mutex_create(const tchar_t* mname)
{
    sem_t* psem;
    
    psem = sem_open(mname, O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH, 1);
    if(psem == SEM_FAILED)
        return (res_sema_t)NULL;
    
    return (res_mutx_t)psem;
}

void _mutex_destroy(const tchar_t* mname, res_mutx_t mtx)
{
    sem_t* psem = (sem_t*)mtx;
    
    sem_close(psem);
    sem_unlink(mname);
}

res_mutx_t _mutex_open(const tchar_t* mname)
{
    sem_t* psem;
    
    psem = sem_open(mname, O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH, 0);
    if(psem == SEM_FAILED)
        return (res_sema_t)NULL;
    
    return (res_sema_t)psem;
}

wait_t _mutex_lock(res_mutx_t mtx, int milsec)
{
    sem_t* psem = (sem_t*)mtx;
    
    return (sem_wait(psem) < 0)? WAIT_TMO : WAIT_RET;
}

void _mutex_unlock(res_mutx_t mtx)
{
    sem_t* psem = (sem_t*)mtx;
    
    sem_post(psem);
}
#endif

/**********************************************************************************/
#ifdef XDK_SUPPORT_THREAD_SEMAP
res_sema_t _semap_create(const tchar_t* sname, int max)
{
    sem_t* psem;
    
	psem = sem_open(sname, O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH, max);
    if(psem == SEM_FAILED)
        return (res_sema_t)NULL;
    
    return (res_sema_t)psem;
}

void _semap_destroy(const tchar_t* sname, res_sema_t sem)
{
    sem_t* psem = (sem_t*)sem;
    
    sem_close(psem);
    sem_unlink(sname);
}

res_sema_t _semap_open(const tchar_t* sname)
{
    sem_t* psem;
    
    psem = sem_open(sname, O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH, 0);
    if(psem == SEM_FAILED)
        return (res_sema_t)NULL;
    
    return (res_sema_t)psem;
}

wait_t _semap_lock(res_sema_t sem, int milsec)
{
    sem_t* psem = (sem_t*)sem;
    
    return (sem_wait(psem) < 0)? WAIT_TMO : WAIT_RET;
}

void _semap_unlock(res_sema_t sem)
{
    sem_t* psem = (sem_t*)sem;
    
    sem_post(psem);
}
#endif

/**********************************************************************************/
#ifdef XDK_SUPPORT_THREAD_QUEUE
res_queue_t _queue_create(res_queue_t ep, res_file_t fd, int max)
{
    int kq;
    
    kq = kqueue();
    
    return (kq < 0)? 0 : (res_queue_t)kq;
}

void _queue_destroy(res_queue_t ep)
{
    close(ep);
}

wait_t _queue_wait(res_queue_t ep, int ms)
{
    struct kevent src[2];
    struct kevent dst[2];
    int rt, n = 0;
    
    EV_SET(&(src[n++]), ep, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
    EV_SET(&(src[n++]), ep, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
    
    rt = kevent(ep, src, n, dst, n, NULL);
    if(rt < 0)
        return WAIT_ERR;
    else if(!rt)
        return WAIT_TMO;
    else
        return WAIT_RET;
}
#endif

#endif //XDK_SUPPORT_THREAD