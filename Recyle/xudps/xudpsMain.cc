

#include "xudps.h"

#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )

xudps_param_t xp = { 0 };


int main(int argc, char* argv[])
{
	xhand_t pipe = NULL;
	stream_t stm = NULL;
	
	byte_t num[INT_LEN] = { 0 };
	tchar_t addr[ADDR_LEN] = { 0 };
	byte_t pack[UDP_SEG_SIZE] = { 0 };
	dword_t n_size = 0;
	int step = 0;

	unsigned short bind = 0;
	unsigned short port = 0;

	int i, len;

	tchar_t errcode[NUM_LEN + 1] = { 0 };
	tchar_t errtext[ERR_LEN + 1] = { 0 };

	xdl_process_init(XDL_APARTMENT_PROCESS);

	TRY_CATCH;

	get_runpath(NULL, xp.sz_root, PATH_LEN);

	for (i = 1; i < argc; i++)
	{
		len = xslen(xp.sz_param);
#ifdef _UNICODE
		mbs_to_ucs((const schar_t*)argv[i], -1, xp.sz_param + len, PATH_LEN - len);
#else
		a_xsncpy(xp.sz_param + len, argv[i], PATH_LEN - len);
#endif
		xsncat(xp.sz_param, _T(" "), 1);
	}

	pipe = xpipe_cli(PIPE_STD);

	if (!pipe)
	{
		raise_user_error(_T("-1"), _T("child process create std pipe failed"));
	}

	stm = stream_alloc(pipe);

	step = 1;
	while (step)
	{
		stream_read_chunk_size(stm, &n_size);
		if (!n_size)
			break;

		switch (step)
		{
		case 1: //bind port
			stream_read_bytes(stm, num, &n_size);
			if (!n_size)
			{
				raise_user_error(_T("-1"), _T("read pipe failed"));
			}
			bind = GET_SWORD_NET(num, 0);

			step++;
			break;
		case 2: //port
			stream_read_bytes(stm, num, &n_size);
			if (!n_size)
			{
				raise_user_error(_T("-1"), _T("read pipe failed"));
			}
			port = GET_SWORD_NET(num, 0);

			step++;
			break;
		case 3: //addr
			if (n_size > ADDR_LEN)
			{
				raise_user_error(_T("-1"), _T("read pipe failed"));
			}
			stream_read_bytes(stm, num, &n_size);
			if (!n_size)
			{
				raise_user_error(_T("-1"), _T("read pipe failed"));
			}
#ifdef _UNICODE
			mbs_to_ucs((schar_t*)num, n_size, addr, ADDR_LEN);
#else
			a_xsncpy(addr, (schar_t*)num, n_size);
#endif
			step ++;
			break;
		case 4: //pack
			if (n_size > UDP_SEG_SIZE)
			{
				raise_user_error(_T("-1"), _T("read pipe failed"));
			}
			stream_read_bytes(stm, pack, &n_size);
			if (!n_size)
			{
				raise_user_error(_T("-1"), _T("read pipe failed"));
			}

			step = 0;
			break;
		}
	}

	stream_free(stm);
	stm = NULL;

	xpipe_free(pipe);
	pipe = NULL;

	_xudps_dispatch(bind, port, addr, pack, &xp);

	END_CATCH;

	xdl_process_uninit();

	return 0;

ONERROR:

	get_last_error(errcode, errtext, ERR_LEN);

	if (pipe)
		xpipe_free(pipe);

	if (stm)
		stream_free(stm);

	xdl_process_uninit();

	return -1;
}

