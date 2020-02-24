
#include "xportm.h"

typedef struct _TMP_CTRL{
	res_crit_t cr;
	xhand_t pp;
	stream_t stm;
}TMP_CTRL;

void* STDCALL read_thread(void* param)
{
	TMP_CTRL* prt = (TMP_CTRL*)param;
	byte_t* buf = NULL;
	dword_t size = 0;
	stream_t stm = NULL;

	xdl_thread_init();

	TRY_CATCH;

	stm = stream_alloc(prt->pp);
	if (!stm)
	{
		raise_user_error(NULL, NULL);
	}

	stream_set_mode(stm, CHUNK_OPERA);

	while (stream_read_chunk_size(stm, &size))
	{
		if (!size)
			break;

		buf = (byte_t*)xmem_alloc(size);
		stream_read_bytes(stm, buf, &size);

		if (size)
		{
			if (prt->cr)
			{
				xcriti_enter(prt->cr);
			}

			stream_write_bytes(prt->stm, buf, size);

			if (prt->cr)
			{
				xcriti_leave(prt->cr);
			}
		}

		xmem_free(buf);
		buf = NULL;
	}

	stream_free(stm);
	stm = NULL;

	xpipe_free(prt->pp);
	prt->pp = NULL;

	xmem_free(prt);
	prt = NULL;

	END_CATCH;

	xdl_thread_uninit(0);

	xthread_end();

	return 0;
ONERROR:

	if (buf)
		xmem_free(buf);

	if (stm)
		stream_free(stm);

	if (prt->pp)
		xpipe_free(prt->pp);

	if (prt)
		xmem_free(prt);

	xdl_thread_uninit(0);

	xthread_end();

	return 0;
}

void* STDCALL wait_thread(void* param)
{
	xportm_param_t* pxm = (xportm_param_t*)param;
	TMP_CTRL *prt;
	xhand_t pipe;

	xdl_thread_init();

	while (pxm->act)
	{
		pipe = xpipe_srv(pxm->pname);
		if (pipe)
		{
			prt = (TMP_CTRL*)xmem_alloc(sizeof(TMP_CTRL));
			prt->cr = pxm->cr;
			prt->stm = pxm->stm;
			prt->pp = pipe;

			xthread_begin(read_thread, (void*)prt);
		}
#ifdef _DEBUG
		xthread_sleep(10);
#endif
	}

	xevent_sign(pxm->ev, 1);

	xdl_thread_uninit(0);

	xthread_end();

	return 0;
}

void _xportm_start(xportm_param_t* pxm)
{
	dword_t dw;

	XDL_ASSERT(pxm != NULL);

	TRY_CATCH;

	if (pxm->stm)
	{
		raise_user_error(_T("-1"), _T("pipe service already started"));
	}

	pxm->std = xcons_alloc();
	if (!pxm->std)
	{
		raise_user_error(_T("-1"), _T("pipe service alloc console failed"));
	}

	pxm->stm = stream_alloc(pxm->std);
	if (pxm->stm)
	{
		stream_set_encode(pxm->stm, DEF_MBS);
	}

	pxm->ev = xevent_create(NULL);
	pxm->cr = xcriti_create();

	pxm->act = 1;
	xthread_begin(wait_thread, (void*)pxm);

	if (pxm->stm)
	{
		stream_write(pxm->stm, _T("xportm start monitoring...\n"), -1, &dw);
	}

	END_CATCH;

	return;

ONERROR:

	if (pxm->std)
		xcons_free(pxm->std);
	pxm->std = NULL;

	if (pxm->stm)
		stream_free(pxm->stm);
	pxm->stm = NULL;

	if (pxm->ev)
		xevent_destroy(pxm->ev);
	pxm->ev = NULL;

	if (pxm->cr)
		xcriti_destroy(pxm->cr);
	pxm->ev = NULL;

	return;
}

void _xportm_stop(xportm_param_t* pxm)
{
	if (pxm->stm)
	{
		if (pxm->cr)
		{
			xcriti_enter(pxm->cr);
		}

		stream_write(pxm->stm, _T("xportm try to stop...\n"), -1, NULL);

		if (pxm->cr)
		{
			xcriti_leave(pxm->cr);
		}
	}

	pxm->act = 0;

	if (pxm->ev)
	{
		xevent_wait(pxm->ev, -1);
	}

	if (pxm->std)
		xcons_free(pxm->std);
	pxm->std = NULL;

	if (pxm->stm)
		stream_free(pxm->stm);
	pxm->stm = NULL;

	if (pxm->ev)
		xevent_destroy(pxm->ev);
	pxm->ev = NULL;

	if (pxm->cr)
		xcriti_destroy(pxm->cr);
	pxm->cr = NULL;
}
