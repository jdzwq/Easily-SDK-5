
#include <xdl.h>
#include <xds.h>

#ifdef _OS_WINDOWS
#include <conio.h>
#endif


#define URL		_T("http://localhost:8889/loc/mymovi.mp4")
//#define URL		_T("https://localhost:8888/loc/mymovi.mp4")
//#define URL		_T("https://myssl.com:443/www.sspanda.com?status=q")
//#define URL		_T("https://www.baidu.com:443")

void set_ssl(xhand_t ssl)
{
    file_t xf;
    byte_t buf_crt[X509_CERT_SIZE] = { 0 };
    byte_t buf_rsa[RSA_KEY_SIZE] = { 0 };
    dword_t dw;
    
    xf = xfile_open(NULL, _T("../sbin/ssl/sslsrv.crt"), 0);
    if (xf)
    {
        dw = X509_CERT_SIZE;
        xfile_read(xf, buf_crt, dw);
        
        xssl_set_cert(ssl, buf_crt, X509_CERT_SIZE);
        xfile_close(xf);
    }
    
    xf = xfile_open(NULL, _T("../sbin/ssl/sslsrv.key"), 0);
    if (xf)
    {
        dw = RSA_KEY_SIZE;
        xfile_read(xf, buf_rsa, dw);
        
        xssl_set_rsa(ssl, buf_rsa, RSA_KEY_SIZE, _T("123456"), -1);
        xfile_close(xf);
    }
    
    xssl_set_auth(ssl, SSL_VERIFY_NONE);
}

void test_https_get()
{
    xhand_t xhttp = xhttp_client(_T("GET"), URL);
    
    if (!(xhttp))
    {
        printf("connect failed\n");
        return;
    }
    
    //set_ssl(xhttp_bio(xhttp));
    //set_tls(xhttp_bio(xhttp));
    
    xhttp_set_request_default_header(xhttp);
    xhttp_set_request_header(xhttp, _T("Content-Type"), -1, _T("text/html"), -1);
    
    //send response
    if (!xhttp_send_request(xhttp))
    {
        printf("send request failed\n");
        xhttp_close(xhttp);
        return;
    }
    
    //recv request
    if (!xhttp_recv_response(xhttp))
    {
        printf("recv response failed\n");
        xhttp_close(xhttp);
        return;
    }
    
    tchar_t szBytes[NUM_LEN + 1] = { 0 };
    tchar_t szCode[INT_LEN + 1] = { 0 };
    xhttp_get_response_header(xhttp, _T("Content-Length"), -1, szBytes, NUM_LEN);
    xhttp_get_response_code(xhttp, szCode);
    
    dword_t len = xstol(szBytes);
    if (!len)
        len = 4 * 1024 * 1024;
    
    byte_t* buf = (byte_t*)xmem_alloc(len);
    
    xhttp_recv(xhttp, (byte_t*)buf, &len);
    
    xhttp_close(xhttp);
    
    if (IS_XHTTP_SUCCEED(szCode))
    {
        file_t fh = xfile_open(NULL, _T("mymovi.mp4"), FILE_OPEN_CREATE);
        
        xfile_write(fh, buf, len);
        
        xfile_close(fh);
    }
    else
    {
        LINKPTR ptr_xml = create_xml_doc();
        
        bool_t rt = parse_xml_doc_from_bytes(ptr_xml, buf, len);
        
        destroy_xml_doc(ptr_xml);
        
        printf("%s", (char*)buf);
    }
    
    xmem_free(buf);
}

void test_ssh_get()
{
    XHANDLE xhttp = xhttp_client(_T("GET"), URL);
    
    //set_ssl(xhttp_bio(xhttp));
    //set_tls(xhttp_bio(xhttp));
    
    xhttp_set_request_default_header(xhttp);
    xhttp_set_request_header(xhttp, _T("Content-Type"), -1, _T("text/html"), -1);
    
    //send response
    if (!xhttp_send_request(xhttp))
    {
        printf("send request failed\n");
        xhttp_close(xhttp);
        return;
    }
    
    //recv request
    if (!xhttp_recv_response(xhttp))
    {
        printf("recv response failed\n");
        xhttp_close(xhttp);
        return;
    }
    
    tchar_t szBytes[NUM_LEN + 1] = { 0 };
    tchar_t szCode[INT_LEN + 1] = { 0 };
    xhttp_get_response_header(xhttp, _T("Content-Length"), -1, szBytes, NUM_LEN);
    xhttp_get_response_code(xhttp, szCode);
    
    dword_t len = xstol(szBytes);
    if (!len)
        len = 4 * 1024 * 1024;
    
    byte_t* buf = (byte_t*)xmem_alloc(len);
    
    xhttp_recv(xhttp, (byte_t*)buf, &len);
    
    xhttp_close(xhttp);
    
    if (IS_XHTTP_SUCCEED(szCode))
    {
        file_t fh = xfile_open(NULL, _T("mymovi.mp4"), FILE_OPEN_CREATE);
        
        xfile_write(fh, buf, len);
        
        xfile_close(fh);
    }
    else
    {
        LINKPTR ptr_xml = create_xml_doc();
        
        bool_t rt = parse_xml_doc_from_bytes(ptr_xml, buf, len);
        
        destroy_xml_doc(ptr_xml);
        
        printf("%s", (char*)buf);
    }
    
    xmem_free(buf);
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
    
    xhand_t ssl = xssl_cli(8888, _T("127.0.0.1"));
    
    if(!ssl)
        return;
    
    byte_t buf[100] = {0};
    dword_t dw = 100;
    xssl_write(ssl, buf, &dw);
    
    xssl_close(ssl);
}

int main()
{
    xdl_process_init(XDL_APARTMENT_PROCESS);
    
    //test_https_get();
    
    //test_ssh_get();
    
    //test_ssl_srv();
    
    test_ssl_cli();
    
    xdl_process_uninit();
    
#ifdef _OS_WINDOWS
    getch();
#endif
    
    return 0;
}

