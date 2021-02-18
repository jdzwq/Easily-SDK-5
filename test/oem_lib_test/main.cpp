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

void test_zlib()
{
	byte_t data[] = "object test 变体对象测试 变体对象测试 变体对象测试 变体对象测试";
	dword_t len = a_xslen((schar_t*)data);
	byte_t buf[100] = { 0 };
	dword_t dw;
	byte_t dst[100] = { 0 };

	dw = 100;
	bool_t rt = xzlib_compress_bytes(data, len, buf, &dw);

	len = 100;
	rt = xzlib_uncompress_bytes(buf, dw, dst, &len);
}

int main(int argc, char* argv[])
{
	//test_siphash();

	test_zlib();

	//ecp_self_test(0);

	//ecp_self_test(1);

	//gcm_self_test(1);

#ifdef _OS_WINDOWS
	getch();
#endif

	return 0;
}

