/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc initialize document

	@module	xdlinit.c | initialize implement file

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


#include "xdlinit.h"
#include "imperr.h"
#include "impassert.h"
#include "impmbcs.h"
#include "oemacp.h"

#ifdef XDL_SUPPORT_MEMO_DUMP
#include "dblink.h"
#include "xdlstr.h"
#endif

xdl_mou_t g_xdl_mou = { 0 };

jmp_buf* thread_jump_buff(void)
{
	if_jump_t* pju;

	pju = THREAD_JUMP_INTERFACE;
	if (!pju)
		return NULL;

	return &(pju->if_buf[pju->if_index]);
}

#ifdef XDK_SUPPORT_THREAD

void xdl_thread_init()
{
	if_zone_t* pzn;
	if_jump_t* pju;
	if_memo_t* piv;
	if_thread_t* pit;
#ifdef XDK_SUPPORT_MEMO_HEAP
	void* heap = NULL;
#endif
    dword_t tid;

	XDL_ASSERT(XDL_MOUNTED);

	piv = PROCESS_MEMO_INTERFACE;
	pit = PROCESS_THREAD_INTERFACE;

    tid = (*pit->pf_thread_get_id)();

#ifdef XDK_SUPPORT_MEMO_HEAP  
	if (g_xdl_mou.if_opt & XDL_APARTMENT_THREAD)
		heap = (*piv->pf_heap_create)();
	else
		heap = (*piv->pf_process_heap)();
	
	XDL_ASSERT(heap != NULL);

	pzn = (if_zone_t*)(*piv->pf_heap_alloc)(heap, sizeof(if_zone_t));
	pzn->if_heap = heap;
#else
	pzn = (if_zone_t*)(*piv->pf_local_alloc)(sizeof(if_zone_t));
#endif /*XDK_SUPPORT_MEMO_HEAP*/

	XDL_ASSERT(g_xdl_mou.tls_thr_zone != 0);
	(*pit->pf_thread_set_tls)(g_xdl_mou.tls_thr_zone, (void*)pzn);
#ifdef XDL_SUPPORT_MEMO_DUMP
	init_root_link(&pzn->if_dump);

	(*pit->pf_criti_enter)(g_xdl_mou.dump_crit);

	pzn->if_trak.tag = (int)(*pit->pf_thread_get_id)();
	pzn->if_trak.next = pzn->if_trak.prev = NULL;
	insert_link_after(&g_xdl_mou.dump_link, LINK_LAST, &pzn->if_trak);

	(*pit->pf_criti_leave)(g_xdl_mou.dump_crit);
#endif
#ifdef XDK_SUPPORT_MEMO_HEAP
	pju = (if_jump_t*)(*piv->pf_heap_alloc)(heap, sizeof(if_jump_t));
#else
	pju = (if_jump_t*)(*piv->pf_local_alloc)(sizeof(if_jump_t));
#endif
	pju->if_buf = NULL;
	pju->if_index = -1;
	pju->if_size = 0;

	XDL_ASSERT(g_xdl_mou.tls_thr_jump != 0);
	(*pit->pf_thread_set_tls)(g_xdl_mou.tls_thr_jump, (void*)pju);
}

void xdl_thread_uninit(int error)
{
	if_zone_t* pzn;
	if_jump_t* pju;
	if_memo_t* piv;
	if_thread_t* pit;
#ifdef XDK_SUPPORT_MEMO_HEAP
	void* heap = NULL;
#endif

	if (!error)
	{
		XDL_ASSERT(XDL_MOUNTED);
	}

	if (!XDL_MOUNTED)
		return;

	piv = PROCESS_MEMO_INTERFACE;
	pit = PROCESS_THREAD_INTERFACE;

	if (!error)
	{
		XDL_ASSERT(g_xdl_mou.tls_thr_zone != 0);
	}

	if (!g_xdl_mou.tls_thr_zone)
		return;

	pzn = (if_zone_t*)(*pit->pf_thread_get_tls)(g_xdl_mou.tls_thr_zone);
	
	if (!error)
	{
		XDL_ASSERT(pzn != NULL);
	}

	if (!pzn)
		return;

#ifdef XDL_SUPPORT_MEMO_DUMP
	(*pit->pf_criti_enter)(g_xdl_mou.dump_crit);

	xmem_dump();

	delete_link(&g_xdl_mou.dump_link, &pzn->if_trak);

	(*pit->pf_criti_leave)(g_xdl_mou.dump_crit);
#endif

#ifdef XDK_SUPPORT_MEMO_HEAP
	heap = pzn->if_heap;

	if (!error)
	{
		XDL_ASSERT(heap != NULL);
	}

	if (!heap)
		return;
#endif /*XDK_SUPPORT_MEMO_HEAP*/

	if (g_xdl_mou.tls_thr_jump != 0)
	{
		pju = (if_jump_t*)(*pit->pf_thread_get_tls)(g_xdl_mou.tls_thr_jump);

		if (!error)
		{
			XDL_ASSERT(pju->if_buf == NULL);
		}
#ifdef XDK_SUPPORT_MEMO_HEAP
		(*piv->pf_heap_free)(heap, (void*)pju);
#else
		(*piv->pf_local_free)((void*)pju);
#endif
		(*pit->pf_thread_set_tls)(g_xdl_mou.tls_thr_jump, 0);
	}

#ifdef XDK_SUPPORT_MEMO_HEAP
	(*piv->pf_heap_free)(heap, (void*)pzn);

	if (g_xdl_mou.if_opt & XDL_APARTMENT_THREAD)
		(*piv->pf_heap_destroy)(heap);
	else
		(*piv->pf_heap_clean)(heap);
#endif /*XDK_SUPPORT_MEMO_HEAP*/

	(*pit->pf_thread_set_tls)(g_xdl_mou.tls_thr_zone, 0);
}

#endif //XDK_SUPPORT_THREAD

static int _is_big_endian()
{
	union cu
	{
		int i;
		char j;
	} n;

	n.i = 1;

	return (n.j == 1) ? 0 : 1;
}

//mount system call
void xdl_process_init(dword_t opt)
{
#if defined(XDK_SUPPORT_THREAD)
	if_thread_t* pit;
#endif

#ifdef XDK_SUPPORT_MEMO_HEAP
	void* heap = NULL;
#endif

	if (g_xdl_mou.if_ok)
		return ;

	g_xdl_mou.if_ok = 1;

	g_xdl_mou.if_opt = opt;

	//g_xdl_mou.if_big = _is_big_endian();

#ifdef XDK_SUPPORT_MEMO_HEAP
	xdk_impl_memo_heap(&g_xdl_mou.if_memo);
#endif

#ifdef XDK_SUPPORT_MEMO_GLOB
	xdk_impl_memo_glob(&g_xdl_mou.if_memo);
#endif

#ifdef XDK_SUPPORT_MEMO_LOCAL
	xdk_impl_memo_local(&g_xdl_mou.if_memo);
#endif

#ifdef XDK_SUPPORT_MEMO_PAGE
	xdk_impl_memo_page(&g_xdl_mou.if_memo);
#endif

#ifdef XDK_SUPPORT_MEMO_CACHE
	xdk_impl_memo_cache(&g_xdl_mou.if_memo);
#endif

#ifdef XDK_SUPPORT_ERROR
	xdk_impl_error(&g_xdl_mou.if_error);
#endif

#ifdef XDK_SUPPORT_MBCS
	xdk_impl_mbcs(&g_xdl_mou.if_mbcs);
#endif

#ifdef XDK_SUPPORT_THREAD
	xdk_impl_thread(&g_xdl_mou.if_thread);

#ifdef XDK_SUPPORT_THREAD_EVENT
	xdk_impl_thread_event(&g_xdl_mou.if_thread);
#endif

#ifdef XDK_SUPPORT_THREAD_CRITI
	xdk_impl_thread_criti(&g_xdl_mou.if_thread);
#endif

#ifdef XDK_SUPPORT_THREAD_MUTEX
	xdk_impl_thread_mutex(&g_xdl_mou.if_thread);
#endif

#ifdef XDK_SUPPORT_THREAD_QUEUE
	xdk_impl_thread_queue(&g_xdl_mou.if_thread);
#endif

#endif //XDK_SUPPORT_THREAD

#ifdef XDK_SUPPORT_TIMER
	xdk_impl_timer(&g_xdl_mou.if_timer);
#endif

#ifdef XDK_SUPPORT_FILE
	xdk_impl_file(&g_xdl_mou.if_file);
#endif

#ifdef XDK_SUPPORT_FILE_FIND
	xdk_impl_file_find(&g_xdl_mou.if_file);
#endif

#ifdef XDK_SUPPORT_FILE_BLOCK
	xdk_impl_file_block(&g_xdl_mou.if_file);
#endif

#ifdef XDK_SUPPORT_SHARE
	xdk_impl_share(&g_xdl_mou.if_share);
#endif

#ifdef XDK_SUPPORT_PIPE
	xdk_impl_pipe(&g_xdl_mou.if_pipe);
#endif

#ifdef XDK_SUPPORT_COMM
	xdk_impl_comm(&g_xdl_mou.if_comm);
#endif

#ifdef XDK_SUPPORT_CONS
	xdk_impl_cons(&g_xdl_mou.if_cons);
#endif

#ifdef XDK_SUPPORT_SOCK
	xdk_impl_socket(&g_xdl_mou.if_socket);
#endif 

#ifdef XDK_SUPPORT_DATE
	xdk_impl_date(&g_xdl_mou.if_date);
#endif

#ifdef XDK_SUPPORT_PROCESS
	xdk_impl_process(&g_xdl_mou.if_process);
#endif


#ifdef XDK_SUPPORT_THREAD
	pit = PROCESS_THREAD_INTERFACE;
	g_xdl_mou.thread_id = (*pit->pf_thread_get_id)();

	//create thread id, heap, jump local storage index
	(*pit->pf_thread_create_tls)(&g_xdl_mou.tls_thr_zero);
	(*pit->pf_thread_create_tls)(&g_xdl_mou.tls_thr_zone);
	(*pit->pf_thread_create_tls)(&g_xdl_mou.tls_thr_jump);
#else
	pim = PROCESS_MEMO_INTERFACE;

#ifdef XDK_SUPPORT_MEMO_HEAP
	heap = (*pim->pf_process_heap)();

	g_xdl_mou.pif_zone = (if_zone_t*)(*pim->pf_heap_alloc)(heap, sizeof(if_zone_t));
	g_xdl_mou.pif_zone->if_heap = heap;

	g_xdl_mou.pif_jump = (if_jump_t*)(*pim->pf_heap_alloc)(heap, sizeof(if_jump_t));
#else
	g_xdl_mou.pif_zone = (if_zone_t*)(*pim->pf_local_alloc)(sizeof(if_heap_t));

	g_xdl_mou.pif_jump = (if_jump_t*)(*pim->pf_local_alloc)(sizeof(if_jump_t));
#endif //XDK_SUPPORT_MEMO_HEAP

	g_xdl_mou.pif_jump->if_buf = NULL;
	g_xdl_mou.pif_jump->if_index = -1;
	g_xdl_mou.pif_jump->if_size = 0;

#endif //XDL_SUPPORT_TRHEAD

#if defined(XDL_SUPPORT_MEMO_DUMP) && defined(XDK_SUPPORT_THREAD)
	pit = PROCESS_THREAD_INTERFACE;
	g_xdl_mou.dump_crit = (*pit->pf_criti_create)();
	init_root_link(&g_xdl_mou.dump_link);
#endif

#ifdef XDK_SUPPORT_THREAD
	//init the master thread local heap, error handler
	xdl_thread_init();
#endif

#if defined(GPL_SUPPORT_ACP) && !defined(XDK_SUPPORT_MBCS)
    g_xdl_mou.cp_gbk = create_gbk_codepage();
    g_xdl_mou.cp_ucs = create_ucs_codepage();
#endif
    
#ifdef XDK_SUPPORT_SOCK
	if (g_xdl_mou.if_socket.pf_socket_startup)
	{
		(*g_xdl_mou.if_socket.pf_socket_startup)();
	}
#endif

	if (g_xdl_mou.if_opt & XDL_INITIALIZE_DESKTOP)
	{
#ifdef XDK_SUPPORT_CONTEXT
		xdk_impl_context(&g_xdl_mou.if_context);

		if (g_xdl_mou.if_context.pf_context_startup)
		{
			g_xdl_mou.ctx_ver = (*g_xdl_mou.if_context.pf_context_startup)();
		}

#ifdef XDK_SUPPORT_CONTEXT_BITMAP
		xdk_impl_context_bitmap(&g_xdl_mou.if_context);
#endif //XDK_SUPPORT_CONTEXT_BITMAP

#ifdef XDK_SUPPORT_CONTEXT_GRAPHIC
		xdk_impl_context_graphic(&g_xdl_mou.if_context);
#endif /*XDK_SUPPORT_CONTEXT_GRAPHIC*/

#ifdef XDK_SUPPORT_CONTEXT_PRINTER
		xdk_impl_context_printer(&g_xdl_mou.if_context);
#endif

#endif //XDK_SUPPORT_CONTEXT

#ifdef XDK_SUPPORT_CLIPBOARD
		xdk_impl_clipboard(&g_xdl_mou.if_clipboard);
#endif //XDK_SUPPORT_CLIPBOARD

#ifdef XDK_SUPPORT_SHELL
		xdk_impl_shell(&g_xdl_mou.if_shell);
#ifdef XDK_SUPPORT_SHELL_DIALOG
		xdk_impl_shell_dialog(&g_xdl_mou.if_shell);
#endif
#endif //XDK_SUPPORT_SHELL


#ifdef XDK_SUPPORT_WIDGET
		xdk_impl_widget(&g_xdl_mou.if_widget);
#ifdef XDK_SUPPORT_WIDGET_EX
		xdk_impl_widget_ex(&g_xdl_mou.if_widget);
#endif
		//start widget context
		if (g_xdl_mou.if_widget.pf_widget_startup)
		{
			(*(g_xdl_mou.if_widget.pf_widget_startup))(g_xdl_mou.ctx_ver);
		}
#endif //XDK_SUPPORT_WIDGET
	}
}

//unmount system call
void xdl_process_uninit()
{
#ifdef XDK_SUPPORT_THREAD
	if_thread_t* pit;
#endif

	if (!g_xdl_mou.if_ok)
		return;

	if (g_xdl_mou.if_opt & XDL_INITIALIZE_DESKTOP)
	{
#ifdef XDK_SUPPORT_CONTEXT
		//stop gdi context
		(*g_xdl_mou.if_context.pf_context_cleanup)();
#endif

#ifdef XDK_SUPPORT_WIDGET
		//stop widget context
		(*g_xdl_mou.if_widget.pf_widget_cleanup)();
#endif
	}

#ifdef XDK_SUPPORT_SOCK
	(*g_xdl_mou.if_socket.pf_socket_cleanup)();
#endif

#if defined(GPL_SUPPORT_ACP) && !defined(XDK_SUPPORT_MBCS)
	if(g_xdl_mou.cp_gbk)
		destroy_codepage(g_xdl_mou.cp_gbk);
	if(g_xdl_mou.cp_ucs)
		destroy_codepage(g_xdl_mou.cp_ucs);
#endif

#ifdef XDK_SUPPORT_THREAD
	pit = PROCESS_THREAD_INTERFACE;

	//uninit the master thread local heap, error handler
	xdl_thread_uninit(0);

	//destroy thread id, heap, jump local storage index
	(*pit->pf_thread_destroy_tls)(g_xdl_mou.tls_thr_zero);
	(*pit->pf_thread_destroy_tls)(g_xdl_mou.tls_thr_zone);
	(*pit->pf_thread_destroy_tls)(g_xdl_mou.tls_thr_jump);

#ifdef XDL_SUPPORT_MEMO_DUMP
	(*pit->pf_criti_enter)(g_xdl_mou.dump_crit);

	thread_dump();

	(*pit->pf_criti_leave)(g_xdl_mou.dump_crit);

	(*pit->pf_criti_destroy)(g_xdl_mou.dump_crit);
#endif

#else

	pim = PROCESS_MEMO_INTERFACE;

#ifdef XDK_SUPPORT_MEMO_HEAP
	(*pim->pf_heap_free)(g_xdl_mou.pif_zone->if_heap, g_xdl_mou.pif_jump);
	(*pim->pf_heap_free)(g_xdl_mou.pif_zone->if_heap, g_xdl_mou.pif_zone);
	g_xdl_mou.pif_jump = NULL;
	g_xdl_mou.pif_zone = NULL;
#else
	(*pim->pf_local_free)(g_xdl_mou.pif_jump);
	(*pim->pf_local_free)(g_xdl_mou.pif_zone);
	g_xdl_mou.pif_jump = NULL;
	g_xdl_mou.pif_heap = NULL;
#endif //XDK_SUPPORT_MEMO_HEAP

#endif //XDK_SUPPORT_THREAD

	memset((void*)&g_xdl_mou, 0, sizeof(xdl_mou_t));
}

#ifdef XDK_SUPPORT_MEMO_HEAP
void xdl_process_clean()
{
	if_memo_t* piv;
	res_heap_t heap;

	piv = PROCESS_MEMO_INTERFACE;

	XDL_ASSERT(piv != NULL);

	heap = (*piv->pf_process_heap)();
	(*piv->pf_heap_clean)(heap);
}
#else
void xdl_process_clean()
{
	return;
}
#endif //XDK_SUPPORT_MEMO_HEAP


#if defined(XDL_SUPPORT_MEMO_DUMP) && defined(XDK_SUPPORT_THREAD)

void thread_dump()
{
	link_t_ptr plk, nlk;
	schar_t token[ERR_LEN];
	dword_t dw;

	if_error_t* pie;
#ifdef XDK_SUPPORT_ERROR
	pie = PROCESS_ERROR_INTERFACE;
	XDL_ASSERT(pie != NULL);
#endif

	plk = get_first_link(&g_xdl_mou.dump_link);
	while (plk)
	{
		nlk = get_next_link(plk);

#ifdef XDK_SUPPORT_ERROR
		dw = a_xsprintf(token, "thread dump:[thread id: %d]", plk->tag);
		(*pie->pf_error_print)(token);
#endif

		delete_link(&g_xdl_mou.dump_link, plk);
		plk = nlk;
	}
}

void xmem_dump()
{
	link_t_ptr plk, nlk;
	dword_t size,tid,len;
	schar_t token[ERR_LEN];

	if_thread_t* pit;
	if_error_t* pie;
	if_zone_t* pih;

	pih = THREAD_ZONE_INTERFACE;
	pit = PROCESS_THREAD_INTERFACE;

#ifdef XDK_SUPPORT_ERROR
	pie = PROCESS_ERROR_INTERFACE;
	XDL_ASSERT(pie != NULL);
#endif

	XDL_ASSERT(pih != NULL && pit != NULL);

	plk = get_first_link(&pih->if_dump);
	while (plk)
	{
		nlk = get_next_link(plk);

		size = *((dword_t*)((byte_t*)plk + sizeof(link_t)));
		
		tid = (*pit->pf_thread_get_id)();

#ifdef XDK_SUPPORT_ERROR
		len = a_xsprintf(token, "memory dump:[thread id: %d, bytes: %d]", tid, size);
		(*pie->pf_error_print)(token);
#endif

		size = (size < (ERR_LEN-2)) ? size : ((ERR_LEN-2));

		memset((void*)token, 0, ERR_LEN);
		memcpy((void*)token, (void*)((byte_t*)plk + sizeof(link_t) + sizeof(dword_t)), size);

#ifdef XDK_SUPPORT_ERROR
		(*pie->pf_error_print)(token);
#endif

		delete_link(&pih->if_dump, plk);
		plk = nlk;
	}
}

#else //XDL_SUPPORT_MEMO_DUMP

void thread_dump()
{
	return;
}

void xmem_dump()
{
	return;
}

#endif //XDK_SUPPORT_THREAD

