
#include "xportm.h"

void* STDCALL wait_thread(void* param)
{
	xportm_param_t* pxm = (xportm_param_t*)param;
	stream_t stm;
	if_bio_t bio = { 0 };
	stream_t stm_std;
	if_bio_t bio_std = { 0 };
	dword_t size;
	byte_t* buf;

	xdl_thread_init(0);

	get_bio_interface(pxm->std, &bio_std);

	stm_std = stream_alloc(&bio_std);

	while (pxm->act)
	{
        size = 0;
		if(!xpipe_listen(pxm->pip))
		{
            thread_sleep(THREAD_BASE_TMO);
			continue;
		}

		get_bio_interface(pxm->pip, &bio);

		stm = stream_alloc(&bio);
		if (!stm)
		{
			break;
		}

		stream_set_mode(stm, CHUNK_OPERA);

        size = 0;
		while (stream_read_chunk_size(stm, &size))
		{
			if (!size)
				break;

			buf = (byte_t*)xmem_alloc(size);
			stream_read_bytes(stm, buf, &size);

			if (size)
			{
				if (pxm->cr)
				{
					criti_enter(pxm->cr);
				}

				if (stm_std)
				{
					stream_write_bytes(stm_std, buf, size);
				}

				if (pxm->cr)
				{
					criti_leave(pxm->cr);
				}
			}

			xmem_free(buf);
			buf = NULL;
		}

		stream_free(stm);
		stm = NULL;

		xpipe_stop(pxm->pip);
	}

	stream_free(stm_std);

	event_sign(pxm->ev, 1);

	xdl_thread_uninit(0);

	thread_stop();

	return 0;
}

void _xportm_start(xportm_param_t* pxm)
{
	dword_t dw;
	stream_t stm;
	if_bio_t bio = { 0 };

	XDL_ASSERT(pxm != NULL);

	TRY_CATCH;

	if (pxm->pip)
	{
		raise_user_error(_T("-1"), _T("service already started"));
	}
    
    pxm->std = xcons_alloc();
    if (!pxm->std)
    {
        raise_user_error(_T("-1"), _T("service alloc console failed"));
    }
    
	get_bio_interface(pxm->std, &bio);

    stm = stream_alloc(&bio);
    if (stm)
    {
        stream_set_encode(stm, DEF_MBS);
    }

	pxm->pip = xpipe_srv(pxm->pname, FILE_OPEN_READ | FILE_OPEN_OVERLAP);
	if (!pxm->pip)
	{
		raise_user_error(_T("-1"), _T("service create pipe failed"));
	}

	pxm->ev = event_create();
	pxm->cr = criti_create();

	pxm->act = 1;
	thread_start(NULL, (PF_THREADFUNC)wait_thread, (void*)pxm);

	if (stm)
	{
		stream_write(stm, _T("xportm start monitoring...\n"), -1, &dw);

		stream_free(stm);
		stm = NULL;
	}

	END_CATCH;

	return;

ONERROR:

	if (stm)
		stream_free(stm);

	if (pxm->ev)
	{
		event_sign(pxm->ev, 1);
	}

    if (pxm->ev)
		event_destroy(pxm->ev);
	pxm->ev = NULL;

	if (pxm->cr)
		criti_destroy(pxm->cr);
	pxm->ev = NULL;

	if (pxm->pip)
		xpipe_free(pxm->pip);
    
    if (pxm->std)
        xcons_free(pxm->std);
    pxm->std = NULL;

	return;
}

void _xportm_stop(xportm_param_t* pxm)
{
	stream_t stm;
	if_bio_t bio = { 0 };

	if (pxm->std)
	{
		get_bio_interface(pxm->std, &bio);

		stm = stream_alloc(&bio);

		stream_set_encode(stm, DEF_MBS);

		if (pxm->cr)
		{
			criti_enter(pxm->cr);
		}

		stream_write(stm, _T("xportm try to stop...\n"), -1, NULL);

		if (pxm->cr)
		{
			criti_leave(pxm->cr);
		}

		stream_free(stm);
	}

	pxm->act = 0;

	if (pxm->ev)
	{
		event_wait(pxm->ev, -1);
	}

	if (pxm->std)
		xcons_free(pxm->std);
	pxm->std = NULL;

	if (pxm->ev)
		event_destroy(pxm->ev);
	pxm->ev = NULL;

	if (pxm->cr)
		criti_destroy(pxm->cr);
	pxm->cr = NULL;

	if (pxm->pip)
		xpipe_free(pxm->pip);
	pxm->pip = NULL;
}
