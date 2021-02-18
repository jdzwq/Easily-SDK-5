
#include <xdl.h>

#ifdef _OS_WINDOWS
#include <conio.h>
#endif

unsigned int STDCALL test_locktable(void* param)
{
	tchar_t errcode[NUM_LEN + 1] = { 0 };
	tchar_t errtext[ERR_LEN + 1] = { 0 };

	int n, i, j, k;
	bool_t rt;
	link_t_ptr lt = NULL;

	xdl_thread_init(0);

	lt = create_lock_table(_T("mylocktable"), 4096);

	n = 1000;
	while (n --)
	{
		i = Lrand48() % 10;
		j = Lrand48() % 10;

		k = 0;
		while (!enter_lock_table(lt, i, j))
		{
			k = 1;
			thread_yield();
		}

		thread_yield();

		leave_lock_table(lt, i, j);

		if (k)
			_tprintf(_T("map:%d pos:%d locked\n"), i, j);
	}

	destroy_lock_table(lt);

	xdl_thread_uninit(0);

	thread_stop();

	return 0;
}


int main(int argc, char* argv[])
{
	tchar_t errtext[ERR_LEN + 1] = { 0 };

	xdl_process_init(XDL_APARTMENT_THREAD | XDL_INITIALIZE_CONSOLE);

	int maxt = 100;

	Srand48(time(NULL));

	res_thread_t* pth = (res_thread_t*)xmem_alloc(maxt * sizeof(res_thread_t));

	for (int i = 0; i < maxt; i++)
	{
		thread_start(&pth[i], (PF_THREADFUNC)test_locktable, (void*)0);
	}

	for (int i = 0; i < maxt; i++)
	{
		thread_join(pth[i]);
	}

	xmem_free(pth);

	xdl_process_uninit();

	_tprintf(_T("%s\n"), errtext);

#ifdef _OS_WINDOWS
	getch();
#endif

	return 0;
}

