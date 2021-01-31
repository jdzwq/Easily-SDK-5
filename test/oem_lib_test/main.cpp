#include <xds.h>
#include <assert.h>
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

		assert(k1 == k2);
		assert(k1 != k3);
	}
}

int main(int argc, char* argv[])
{
	//test_siphash();

	//ecp_self_test(0);

	ecp_self_test(1);

#ifdef _OS_WINDOWS
	getch();
#endif

	return 0;
}

