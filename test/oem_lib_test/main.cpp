
#include <xdl.h>
#include <xds.h>

#ifdef _OS_WINDOWS
#include <conio.h>
#endif

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

int main(int argc, char* argv[])
{
	xdl_process_init(XDL_APARTMENT_PROCESS);

	test_siphash();

	xdl_process_uninit();

#ifdef _OS_WINDOWS
	getch();
#endif

	return 0;
}

