// xdk_lib_test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

void test_page()
{
	if_memo_t if_memo = { 0 };

	xdk_impl_memo_page(&if_memo);

	void* p = (*if_memo.pf_page_alloc)(1);
	dword_t n = (*if_memo.pf_page_size)(p);

	p = (*if_memo.pf_page_realloc)(p, 10000);
	p = (*if_memo.pf_page_realloc)(p, 1);

	(*if_memo.pf_page_free)(p);

	if_process_t if_proc = { 0 };

	xdk_impl_process(&if_proc);

	sys_info_t si = { 0 };
	(*if_proc.pf_system_info)(&si);
}



void test_comm()
{
	if_comm_t if_comm = { 0 };

	xdk_impl_comm(&if_comm);

	while (1)
	{
		res_file_t com = (*if_comm.pf_comm_open)(_T("COM4"));
		if (!com)
		{
			Sleep(100);
			continue;
		}

		char buf[1024] = { 0 };

		dword_t msk = 0;
		dword_t dw;
		sword_t sw;

		INPUT input[2] = { 0 };

		while (msk = (*if_comm.pf_comm_listen)(com, &dw))
		{
			if (msk &  COMM_EVNET_ERROR)
			{
				printf("comm ERR:~\n");
				break;
			}

			if (msk & COMM_EVNET_BREAK)
			{
				printf("comm BREAK:~\n");
			}

			if (msk & COMM_EVENT_IDLE)
			{
				printf("comm IDLE:~\n");
			}

			if (msk & COMM_EVNET_RING)
			{
				if (dw & COMM_STATUS_RING_ON)
					printf("comm RING: on\n");
				else
				{
					printf("comm RING: off\n");
					break;
				}
			}

			if (msk & COMM_EVNET_RLSD)
			{
				if (dw & COMM_STATUS_RLSD_ON)
					printf("comm RLSD: on\n");
				else
					printf("comm RLSD: off\n");
			}

			if (msk & COMM_EVNET_CTS)
			{
				if (dw & COMM_STATUS_CTS_ON)
					printf("comm CTS: on\n");
				else
					printf("comm CTS: off\n");
			}

			if (msk & COMM_EVNET_DSR)
			{
				if (dw & COMM_STATUS_DSR_ON)
					printf("comm DSR: on\n");
				else
					printf("comm DSR: off\n");
			}

			if (msk & COMM_EVNET_RXCHAR)
			{
				(*if_comm.pf_comm_read)(com, (char*)buf, dw, &dw);
				buf[dw] = 0;

				float df = buf[1] * 255 + buf[2];
				df = df - 273.15;
				
				float f2 = ((buf[2] << 8 | buf[1]) * 625) / 1000;

				if (dw)
				{
					printf("comm DATA: %s\n", (char*)buf);

					/*sw = MAKESWORD(buf[0], buf[1]);
					input[0].type = INPUT_KEYBOARD;
					input[0].ki.wVk = 0;
					input[0].ki.wScan = sw;
					input[0].ki.dwFlags = KEYEVENTF_UNICODE;
					SendInput(1, input, sizeof(INPUT));

					input[1].type = INPUT_KEYBOARD;
					input[1].ki.wVk = 0;
					input[1].ki.wScan = sw;
					input[1].ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
					SendInput(1, input + 1, sizeof(INPUT));

					sw = MAKESWORD(buf[2], buf[3]);
					input[0].type = INPUT_KEYBOARD;
					input[0].ki.wVk = 0;
					input[0].ki.wScan = sw;
					input[0].ki.dwFlags = KEYEVENTF_UNICODE;
					SendInput(1, input, sizeof(INPUT));

					input[1].type = INPUT_KEYBOARD;
					input[1].ki.wVk = 0;
					input[1].ki.wScan = sw;
					input[1].ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
					SendInput(1, input + 1, sizeof(INPUT));*/
				}
			}
		}

		(*if_comm.pf_comm_close)(com);
	}
}


void test_semap()
{
	res_sema_t sem = xsemap_create(NULL, 1);

	xsemap_lock(sem, -1);

	xsemap_lock(sem, 3000);

	xsemap_unlock(sem);

	xsemap_lock(sem, -1);

	xsemap_destroy(NULL, sem);
}

int _tmain(int argc, _TCHAR* argv[])
{
	test_page();

	//test_comm();

	getch();
	return 0;
}

