
#include <xdl.h>
#include <xds.h>

#ifdef _OS_WINDOWS
#include <conio.h>
#endif

//#define ADDR_SSH		_T("172.16.190.190")
#define ADDR_SSH		_T("127.0.0.1")
//#define ADDR_SSH		_T("49.234.135.113")

//#define PORT_SSH		22
#define PORT_SSH		8887

static const char *my[] = {
	"1," "2," "hello",
	"3"
};

static char dhm_G[] = "2";
static char dhm_P[] = "E4004C1F94182000103D883A448B3F802CE4B44A83301270002C20D0321CFD00" \
"11CCEF784C26A400F43DFB901BCA7538F2C6B176001CF5A0FD16D2C48B1D0C1C" \
"F6AC8E1DA6BCC3B4E1F96B0564965300FFA1D0B601EB2800F489AA512C4B248C" \
"01F76949A60BB7F00A40B1EAB64BDD48E8A700D60B7F1200FA8E77B0A979DABF";

void test_dhm()
{
	havege_state rng = { 0 };

	havege_init(&rng);

	dhm_context dhm = { 0 };

	mpi_read_string(&(dhm.P), 16, dhm_P, -1);
	mpi_read_string(&(dhm.G), 16, dhm_G, -1);

	byte_t dhm_buf[1024] = { 0 };
	int dhm_len = 0;
	dhm_make_public(&(dhm), 2048, dhm_buf, &(dhm_len), havege_rand, &(rng));

	dhm_context dhm2 = { 0 };

	mpi_read_string(&(dhm2.P), 16, dhm_P, -1);
	mpi_read_string(&(dhm2.G), 16, dhm_G, -1);

	dhm_read_public(&dhm2, dhm_buf, dhm_len);
	
	dhm_free(&dhm);
	dhm_free(&dhm2);

	rsa_context rsa = { 0 };

	rsa_init(&rsa, RSA_PKCS_V15, RSA_SHA1, havege_rand, &rng);

	rsa_gen_key(&rsa, 2048, 3);

	byte_t rsa_buf[1024] = { 0 };
	int rsa_len = 0;

	rsa_export_pubkey(&rsa,rsa_buf, &rsa_len, 0);

	rsa_context rsa2 = { 0 };

	rsa_init(&rsa2, RSA_PKCS_V15, RSA_SHA1, havege_rand, &rng);

	rsa_gen_key(&rsa2, 2048, 3);

	rsa_import_pubkey(&rsa2, rsa_buf, rsa_len, 0);

	rsa_check_pubkey(&rsa2);

	sha1_context sha = { 0 };

	byte_t sha_buf[20] = { 0 };
	int sha_len = 20;

	sha1_starts(&sha);
	sha1_update(&sha, dhm_buf, dhm_len);
	sha1_finish(&sha, sha_buf);

	byte_t rsa_sig[256] = { 0 };

	rsa_pkcs1_sign(&rsa, RSA_PRIVATE, RSA_SHA1, sha_len, sha_buf, rsa_sig);

	rsa_pkcs1_verify(&rsa2, RSA_PUBLIC, RSA_SHA1, sha_len, sha_buf, rsa_sig);

	rsa_free(&rsa);
	rsa_free(&rsa2);
}

void test_aes()
{
	byte_t iv[16] = { 0 };;
	byte_t key[20] = { 0 };

	havege_state rnd = { 0 };
	havege_init(&rnd);
	for (int i = 0; i < 20; i++)
	{
		key[i] = (byte_t)havege_rand(&rnd);
	}

	byte_t tmp[26] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', '1', '2', '3', '4', '5', '6' };

	aes_context aes = { 0 };

	aes_setkey_enc(&aes, key, 128);

	for (int j = 0; j < 16; j++)
	{
		iv[j] = j;
	}

	aes_crypt_cbc(&aes, AES_ENCRYPT, 10, iv, tmp, tmp);

	//aes_crypt_cbc(&aes, AES_ENCRYPT, 6, iv, tmp + 10, tmp + 10);

	aes_context aes2 = { 0 };

	aes_setkey_dec(&aes2, key, 128);

	for (int j = 0; j < 16; j++)
	{
		iv[j] = j;
	}

	aes_crypt_cbc(&aes2, AES_DECRYPT, 10, iv, tmp, tmp);
	//aes_crypt_cbc(&aes2, AES_DECRYPT, 6, iv, tmp + 10, tmp + 10);
}

void test_ssh()
{
	schar_t msg[NUM_LEN] = { "hello world!\r\n" };
	dword_t len = a_xslen(msg);

	xhand_t ssh = xssh_cli(PORT_SSH, ADDR_SSH);

	xssh_write(ssh, (byte_t*)msg, &len);

	xssh_flush(ssh);

	xssh_close(ssh);
}

int main(int argc, char* argv[])
{
	tchar_t errtext[ERR_LEN + 1] = { 0 };
    
	xdl_process_init(XDL_APARTMENT_THREAD | XDL_INITIALIZE_CONSOLE);
    
	//test_aes();
	//test_dhm();
	test_ssh();

	xdl_process_uninit();

	printf("%s\n", errtext);

#ifdef _OS_WINDOWS
	getch();
#endif

	return 0;
}

