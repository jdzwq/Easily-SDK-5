/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xdk system call interface document

	@module	xdkiml.h | interface file

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

#ifndef _XDKIML_H
#define	_XDKIML_H

#include "xdkdef.h"

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef XDK_SUPPORT_MEMO
#ifdef XDK_SUPPORT_MEMO_HEAP
/*heap memory*/
res_heap_t _process_heapo(void);
res_heap_t	_heapo_create(void);
void	_heapo_destroy(res_heap_t heap);
void	_heapo_clean(res_heap_t heap);
void*	_heapo_alloc(res_heap_t heap, dword_t size);
void*	_heapo_realloc(res_heap_t heap, void* p, dword_t size);
void	_heapo_free(res_heap_t heap, void* p);
void	_heapo_zero(res_heap_t heap, void* p, dword_t size);
#endif

#ifdef XDK_SUPPORT_MEMO_GLOB
/*glob memory*/
res_glob_t	_globo_alloc(dword_t size);
res_glob_t	_globo_realloc(res_glob_t glob, dword_t size);
void	_globo_free(res_glob_t glob);
dword_t	_globo_size(res_glob_t glob);
void*	_globo_lock(res_glob_t glob);
bool_t	_globo_unlock(res_glob_t glob);
#endif

#ifdef XDK_SUPPORT_MEMO_LOCAL
/*local memory*/
void*	_local_alloc(dword_t size);
void*	_local_realloc(void* p, dword_t size);
void	_local_free(void* p);
#endif

#ifdef XDK_SUPPORT_MEMO_PAGE
/*page memory*/
void*	_paged_alloc(dword_t size);
void*	_paged_realloc(void* p, dword_t size);
void	_paged_free(void* p);
dword_t	_paged_size(void* p);
void*	_paged_lock(void* p);
void	_paged_unlock(void* p);
bool_t	_paged_protect(void* p, bool_t b);
#endif

#ifdef XDK_SUPPORT_MEMO_CACHE
void*	_cache_open(void);
void	_cache_close(void* bh);
bool_t	_cache_write(void* bh, dword_t hoff, dword_t loff, void* buf, dword_t size, dword_t* pb);
bool_t	_cache_read(void* bh, dword_t hoff, dword_t loff, void* buf, dword_t size, dword_t* pb);
#endif

#endif /*XDK_SUPPORT_MEMO*/

#ifdef XDK_SUPPORT_ERROR
/*error*/
int		_error_text(tchar_t* buf, int max);
void	_error_exit(void);
void	_error_debug(const tchar_t* str);
void	_error_print(const char* str);
#endif

#ifdef XDK_SUPPORT_MBCS
int		_gbk_to_ucs(const schar_t* gbk, int len, wchar_t* ucs, int max);
int		_ucs_to_gbk(const wchar_t* ucs, int len, schar_t* gbk, int max);
int		_utf8_to_ucs(const schar_t* utf, int len, wchar_t* ucs, int max);
int		_ucs_to_utf8(const wchar_t* ucs, int len, schar_t* utf, int max);
#endif

#ifdef XDK_SUPPORT_DATE
void	_get_loc_date(xdate_t* pxd);
bool_t	_mak_loc_date(xdate_t* pxd);
void	_get_utc_date(xdate_t* pxd);
bool_t	_mak_utc_date(xdate_t* pxd);
dword_t	_get_times(void);
clock_t _get_ticks(void);
stamp_t _get_timestamp(void);
void	_utc_date_from_times(xdate_t* pxd, dword_t ms);
void	_utc_date_from_ticks(xdate_t* pxd, clock_t ts);
void	_utc_date_from_timestamp(xdate_t* pxd, stamp_t ts);
#endif

#ifdef XDK_SUPPORT_ASYNC
async_t* _async_alloc_lapp(int type, int ms, res_file_t fd);
void	_async_free_lapp(async_t* pas);
#endif

#ifdef XDK_SUPPORT_THREAD
//those thread function must implemented by each platform
void	_thread_begin(res_thread_t* ptr_hand, PF_THREADFUNC pf_func, void* param);
void	_thread_end(void);
void	_thread_create_tls(tls_key_t* pkey);
void	_thread_destroy_tls(tls_key_t key);
void*	_thread_get_tls(tls_key_t key);
void	_thread_set_tls(tls_key_t key, void* pval);
pid_t	_thread_get_id(void);
void	_thread_sleep(int ms);
void	_thread_yield(void);
void	_thread_join(res_thread_t th);
void	_thread_safe(void);

#ifdef XDK_SUPPORT_THREAD_EVENT
res_even_t	_event_create(void);
void		_event_destroy(res_even_t ev);
wait_t		_event_wait(res_even_t ev, int milsec);
void		_event_sign(res_even_t ev, bool_t b_sign);
#endif

#ifdef XDK_SUPPORT_THREAD_CRITI
res_crit_t	_criti_create(void);
void		_criti_destroy(res_crit_t cr);
bool_t		_criti_query(res_crit_t cr);
void		_criti_enter(res_crit_t cr);
void		_criti_leave(res_crit_t cr);
#endif

#ifdef XDK_SUPPORT_THREAD_MUTEX
res_mutx_t	_mutex_create(const tchar_t* mname);
void		_mutex_destroy(const tchar_t* mname, res_mutx_t mtx);
res_mutx_t	_mutex_open(const tchar_t* mname);
wait_t		_mutex_lock(res_mutx_t mtx, int milsec);
void		_mutex_unlock(res_mutx_t mtx);
#endif

#ifdef XDK_SUPPORT_THREAD_SEMAP
res_sema_t	_semap_create(const tchar_t* sname, int max);
void		_semap_destroy(const tchar_t* sname, res_sema_t sem);
res_sema_t	_semap_open(const tchar_t* sname);
wait_t		_semap_lock(res_sema_t sem, int milsec);
void		_semap_unlock(res_sema_t sem);
#endif

#ifdef XDK_SUPPORT_THREAD_QUEUE
res_queue_t	_queue_create(res_queue_t ep, res_file_t fd, int max);
void		_queue_destroy(res_queue_t ep);
wait_t		_queue_wait(res_queue_t ep, int ms);
#endif

#endif /*XDK_SUPPORT_THREAD*/

#ifdef XDK_SUPPORT_TIMER
res_queue_t _create_timer_queue(void);
void		_destroy_timer_queue(res_queue_t rq);
res_timer_t _create_timer(res_queue_t rq, clock_t duetime, clock_t period, PF_TIMERFUNC pf, void* pa);
void		_destroy_timer(res_queue_t rq, res_timer_t rt);
bool_t		_alter_timer(res_queue_t rq, res_timer_t rt, clock_t duetime, clock_t period);
#endif

#ifdef XDK_SUPPORT_SOCK
bool_t		_socket_startup(void);
void		_socket_cleanup(void);
res_file_t	_socket_tcp(int ver, dword_t flag);
res_file_t	_socket_udp(int ver, dword_t flag);
res_file_t	_socket_icmp(int ver, dword_t flag);
void	_socket_shutdown(res_file_t so, int how);
void	_socket_close(res_file_t so);
dword_t	_socket_wait(res_file_t so, dword_t msk, int ms);
bool_t	_socket_bind(res_file_t so, res_addr_t saddr, int slen);
bool_t	_socket_listen(res_file_t so, int max);
bool_t	_socket_connect(res_file_t so, res_addr_t saddr, int slen);
res_file_t _socket_accept(res_file_t so, res_addr_t saddr, int *slen, async_t* pb);
bool_t	_socket_sendto(res_file_t so, res_addr_t saddr, int alen, void* buf, dword_t len, async_t* pb);
bool_t	_socket_recvfrom(res_file_t so, res_addr_t saddr, int *plen, void* buf, dword_t len, async_t* pb);
bool_t	_socket_send(res_file_t so, void* buf, dword_t len, async_t* pb);
bool_t	_socket_recv(res_file_t so, void* buf, dword_t len, async_t* pb);
int		_socket_write(void* pso, unsigned char* buf, int len);
int		_socket_read(void* pso, unsigned char* buf, int len);
bool_t	_socket_setopt(res_file_t so, int optname, const char* optval, int optlen);
bool_t	_socket_getopt(res_file_t so, int optname, char* pval, int* plen);
bool_t	_socket_set_nonblk(res_file_t so, bool_t none);
bool_t	_socket_get_nonblk(res_file_t so);
bool_t	_socket_set_linger(res_file_t so, bool_t wait, int sec);
bool_t	_socket_set_sndbuf(res_file_t so, int size);
bool_t	_socket_set_rcvbuf(res_file_t so, int size);
bool_t	_host_addr(const char* host, char* addr);
void	_fill_addr(net_addr_t* paddr, unsigned short port, const char* saddr);
void	_conv_addr(net_addr_t* paddr, unsigned short* port, char* addr);
void	_socket_addr(res_file_t so, net_addr_t* paddr);
void	_socket_peer(res_file_t so, net_addr_t* paddr);
bool_t	_socket_share(pid_t procid, res_file_t procfd, res_file_t so, void* data, dword_t size);
res_file_t _socket_dupli(res_file_t procfd, void* data, dword_t* pcb);
int		_socket_error(tchar_t* buf, int max);
#endif

#ifdef XDK_SUPPORT_FILE
res_file_t	_file_open(const tchar_t* fname, dword_t fmode);
void	_file_close(res_file_t fl);
bool_t	_file_size(res_file_t fh, dword_t* ph, dword_t* pl);
bool_t	_file_write(res_file_t fl, void* buf, dword_t size, async_t* pb);
bool_t	_file_flush(res_file_t fl);
bool_t	_file_read(res_file_t fl, void* buf, dword_t size, async_t* pb);
bool_t	_file_read_range(res_file_t fh, dword_t hoff, dword_t loff, void* buf, dword_t size);
bool_t	_file_write_range(res_file_t fh, dword_t hoff, dword_t loff, void* buf, dword_t size);
bool_t	_file_truncate(res_file_t fh, dword_t hoff, dword_t loff);
bool_t	_file_delete(const tchar_t* fname);
bool_t	_file_rename(const tchar_t* fname, const tchar_t* nname);
bool_t	_file_info(const tchar_t* fname, file_info_t* pxf);
bool_t	_file_settime(res_file_t fh, const xdate_t* pdt);
bool_t	_file_gettime(res_file_t fh, xdate_t* pdt);
bool_t	_directory_open(const tchar_t* path, dword_t mode);
bool_t	_directory_create(const tchar_t* path);
bool_t	_directory_remove(const tchar_t* path);
#ifdef XDK_SUPPORT_FILE_FIND
res_find_t	_file_find_first(const tchar_t* fpath, file_info_t* pfi);
bool_t	_file_find_next(res_find_t ff, file_info_t* pfi);
void _file_find_close(res_find_t ff);
#endif
#endif

#ifdef XDK_SUPPORT_SHARE
res_file_t _share_srv(const tchar_t* sname, const tchar_t* fpath, dword_t hoff, dword_t loff, dword_t size);
void	_share_close(const tchar_t* sname, res_file_t bh);
res_file_t	_share_cli(const tchar_t* fname, dword_t size);
bool_t	_share_write(res_file_t bh, dword_t off, void* buf, dword_t size, dword_t* pcb);
bool_t	_share_read(res_file_t bh, dword_t off, void* buf, dword_t size, dword_t* pcb);
void*	_share_lock(res_file_t fh, dword_t off, dword_t size);
void	_share_unlock(res_file_t fh, dword_t off, dword_t size, void* p);
#endif

#ifdef XDK_SUPPORT_PIPE
res_file_t _pipe_srv(const tchar_t* pname, dword_t fmode);
bool_t	_pipe_listen(res_file_t pip, async_t* pb);
void	_pipe_stop(res_file_t pip);
res_file_t _pipe_cli(const tchar_t* pname, dword_t fmode);
void	_pipe_close(const tchar_t* pname, res_file_t pip);
wait_t	_pipe_wait(const tchar_t* pname, int milsec);
bool_t	_pipe_write(res_file_t pipe, void* buf, dword_t len, async_t* pb);
bool_t	_pipe_flush(res_file_t pipe);
bool_t	_pipe_read(res_file_t pipe, void* buf, dword_t len, async_t* pb);
#endif

#ifdef XDK_SUPPORT_CONS
res_file_t	_cons_alloc(tchar_t* cname, int max);
void	_cons_free(res_file_t ch);
bool_t	_cons_sigaction(res_file_t con, PF_SIGHANDLER pf);
bool_t	_cons_write(res_file_t ch, void* buf, dword_t len, dword_t* pcb);
bool_t	_cons_flush(res_file_t ch);
bool_t	_cons_read(res_file_t ch, void* buf, dword_t len, dword_t* pcb);
res_file_t _cons_stdout(res_file_t ch);
res_file_t _cons_stdin(res_file_t ch);
#endif

#ifdef XDK_SUPPORT_COMM
void	_default_comm_mode(dev_com_t* pmod);
bool_t	_get_comm_mode(res_file_t fh, dev_com_t* pmod);
bool_t	_set_comm_mode(res_file_t fh, const dev_com_t* pmod);
res_file_t _comm_open(const tchar_t* pname, dword_t fmode);
void	_comm_close(res_file_t fh);
bool_t	_comm_read(res_file_t fh, void* buf, dword_t size, async_t* pb);
bool_t	_comm_write(res_file_t fh, void* buf, dword_t size, async_t* pb);
bool_t	_comm_flush(res_file_t fh);
dword_t	_comm_listen(res_file_t fh, async_t* pb);
#endif


#ifdef XDK_SUPPORT_PROCESS
res_modu_t _load_library(const tchar_t* lname);
void	_free_library(res_modu_t lib);
void*	_get_address(res_modu_t lib, const schar_t* fname);
void	_get_runpath(res_modu_t ins, tchar_t* buf, int max);
bool_t	_create_process(const tchar_t* exename, const tchar_t* cmdline, int share, proc_info_t* ppi);
void	_release_process(proc_info_t* ppi);
void	_process_safe(void);
void	_process_waitrun(res_proc_t child);
res_file_t _process_dupli(res_proc_t ph, res_file_t fh);
void*	_process_alloc(res_proc_t ph, dword_t dw);
void	_process_free(res_proc_t ph, void* p);
bool_t	_process_write(res_proc_t ph, void* p, void* data, dword_t size);
bool_t	_process_read(res_proc_t ph, void* p, void* data, dword_t size);
void	_release_handle(res_file_t fh);
bool_t	_inherit_handle(res_file_t fh, bool_t b);
void	_read_profile(const tchar_t* fname, const tchar_t* sec, const tchar_t* key, tchar_t* buf, int max);
void	_write_profile(const tchar_t* fname, const tchar_t* sec, const tchar_t* key, const tchar_t* val);
int		_get_envvar(const tchar_t* ename, tchar_t* buf, int max);
void	_system_info(sys_info_t* psi);
#endif

/***************************************************************************************************************************/

#ifdef	__cplusplus
}
#endif


#endif	/* _XDKIML_H */

