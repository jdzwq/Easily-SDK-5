// oem_lib_test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

void test_siphash()
{
	uint64_t k1 = 0;
	uint64_t k2 = 0;
	uint64_t k3 = 0;

	siphash64((uint8_t*)"sip hash", 8, (uint8_t*)&k1);

	for (int i = 0; i < 1000000; i++)
	{
		siphash64((uint8_t*)"sip hash", 8, (uint8_t*)&k2);
		siphash64((uint8_t*)"sip hash ", 9, (uint8_t*)&k3);

		XDL_ASSERT(k1 == k2);
		XDL_ASSERT(k1 != k3);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	xdl_process_init(XDL_APARTMENT_PROCESS);

	test_siphash();

	xdl_process_uninit();

	getch();

	return 0;
}

