
#include <xdl.h>
#include <xds.h>

#ifdef _OS_WINDOWS
#include <conio.h>
#endif


//#define URL		_T("http://localhost:8889/loc/mymovi.mp4")
//#define URL		_T("https://localhost:8888/loc/mymovi.mp4")
//#define URL		_T("https://myssl.com:443/www.sspanda.com?status=q")
//#define URL		_T("https://www.baidu.com:443")
#define URL		_T("https://mp.weixin.qq.com:443")

void set_ssl(xhand_t ssl)
{
	if_fio_t* pxf = { 0 };
    byte_t buf_crt[X509_CERT_SIZE] = { 0 };
    byte_t buf_rsa[RSA_KEY_SIZE] = { 0 };
	dword_t dw_crt = 0;
	dword_t dw_key = 0;
    
    pxf = xfile_open(NULL, _T("../sbin/ssl/sslsrv.crt"), 0);
    if (pxf)
    {
        dw_crt = X509_CERT_SIZE;
        xfile_read(pxf, buf_crt, dw_crt);
		dw_crt = a_xslen((schar_t*)buf_crt);
       
        xfile_close(pxf);
    }
    
    pxf = xfile_open(NULL, _T("../sbin/ssl/sslsrv.key"), 0);
    if (pxf)
    {
        dw_key = RSA_KEY_SIZE;
        xfile_read(pxf, buf_rsa, dw_key);
		dw_key = a_xslen((schar_t*)buf_rsa);

        xfile_close(pxf);
    }

	xssl_set_cert(ssl, buf_crt, dw_crt, buf_rsa, dw_key, _T("123456"), -1);
	xssl_set_version(ssl, TLSv12);
}

void test_ssl_srv()
{
    net_addr_t sin;
    res_file_t so;
    sys_info_t si = { 0 };
    
    so = socket_tcp(0, FILE_OPEN_OVERLAP);
    if (so == INVALID_FILE)
    {
        return;
    }
    
    xmem_zero((void*)&sin, sizeof(sin));
    
    fill_addr(&sin, 8888, NULL);
    
    if (!socket_bind(so, (res_addr_t)&sin, sizeof(sin)))
    {
        socket_close(so);
        return; //bind sock error
    }
    
    if (!socket_listen(so, SOMAXCONN))
    {
        socket_close(so);
        return; //listen error
    }
    
    net_addr_t locaddr, rmtaddr;
    int addr_len;
    res_file_t ao;
    async_t* over = async_alloc_lapp(ASYNC_EVENT, TCP_BASE_TIMO, INVALID_FILE);
    
    addr_len = sizeof(net_addr_t);
    ao = socket_accept(so, (res_addr_t)&rmtaddr, &addr_len, over);
    if (ao == INVALID_FILE)
    {
        async_free_lapp(over);
        socket_close(so);
        return;
    }
    
    xhand_t ssl = xssl_srv(ao);
    if(!ssl)
    {
        async_free_lapp(over);
        socket_close(ao);
        socket_close(so);
        return;
    }
    
    set_ssl(ssl);
    
    byte_t buf[100] = {0};
    dword_t dw = 100;
    xssl_read(ssl, buf, &dw);
    
    printf("%s", (char*)buf);
    
    xssl_close(ssl);
    
    socket_close(ao);
    socket_close(so);

    async_free_lapp(over);
}

void test_ssl_cli()
{
	tchar_t addr[ADDR_LEN + 1] = { 0 };

	host_addr(_T("mp.weixin.qq.com"), addr);

    xhand_t ssl = xssl_cli(443, addr);
    
    if(!ssl)
        return;
    
	set_ssl(ssl);

    byte_t buf[10] = {'0','1','2','3','4','5','6','7','8','9'};
    dword_t dw = 100;
    xssl_write(ssl, buf, &dw);
    
    xssl_close(ssl);
}

int main()
{
    xdl_process_init(XDL_APARTMENT_PROCESS);
    
    //test_ssl_srv();
    
    test_ssl_cli();
    
    xdl_process_uninit();
    
    return 0;
}

