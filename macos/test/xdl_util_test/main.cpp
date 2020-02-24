//
//  main.cpp
//  xdl_util_test
//
//  Created by 张文权 on 18/10/6.
//  Copyright © 2018年 张文权. All rights reserved.
//

#include <iostream>
#include <xdl.h>
#include <../xdl/oemconv.h>

void test_error()
{
    tchar_t errnum[INT_LEN];
    tchar_t errtxt[ERR_LEN];
    
    TRY_CATCH;
    
    raise_user_error("-1", "error");
    
    END_CATCH;
    
    return;
ONERROR:
    get_last_error(errnum, errtxt, ERR_LEN);
    
    printf("%s\n", errtxt);
}

void test_conv()
{
    tchar_t utf_buf[] = "我的中国";
    dword_t dw;
    wchar_t ucs_buf[20] = {0};
    char gbk_buf[20] = {0};
    char mbs_buf[20] = {0};
    
    dw = xslen(utf_buf);
    int n = utf8_to_ucs((byte_t*)utf_buf, dw, NULL, 20);
    utf8_to_ucs((byte_t*)utf_buf, dw, ucs_buf, n);
    printf("UTF-8: %s\n", utf_buf);
    wprintf(L"UCS: %S\n", ucs_buf);

    n = ucs_to_gb2312(ucs_buf, -1, NULL, 20);
    ucs_to_gb2312(ucs_buf, -1, (byte_t*)gbk_buf, n);
    printf("GBK: %s\n", gbk_buf);
    wprintf(L"UCS: %S\n", ucs_buf);
    
    dw = xslen(utf_buf);
    n = utf8_to_mbs((byte_t*)utf_buf, dw, NULL, 20);
    utf8_to_mbs((byte_t*)utf_buf, dw, mbs_buf, n);
    printf("UTF-8: %s\n", utf_buf);
    printf("MBS: %s\n", mbs_buf);

}

void* thr_fun(void* param)
{
    tchar_t errnum[INT_LEN];
    tchar_t errtxt[ERR_LEN];
    
    res_even_t ev = (res_even_t)param;
    
    xdl_thread_init(0);
    
    TRY_CATCH;
    
    raise_user_error("-1", "thread error");
    
    END_CATCH;
    
    xdl_thread_uninit(0);
    
    return NULL;
ONERROR:
    
    get_last_error(errnum, errtxt, ERR_LEN);
    
    printf("%s\n", errtxt);

    xdl_thread_uninit(0);
    
    event_sign(ev, 1);
    
    return NULL;
}

void test_thread()
{
    tchar_t errnum[INT_LEN];
    tchar_t errtxt[ERR_LEN];

    res_even_t ev;
    
    TRY_CATCH;
    
    ev = event_create();
    
    thread_start(NULL, thr_fun,(void*)ev);
    
    event_wait(ev, -1);
    event_destroy(ev);
    
    raise_user_error("-1", "process error");
    
    END_CATCH;
    
    return;
ONERROR:
    
    get_last_error(errnum, errtxt, ERR_LEN);
    
    printf("%s\n", errtxt);
    
    return;
}

void test_stream()
{
    xhand_t cons = xcons_alloc();
    
    stream_t stm = stream_alloc(cons);
    
    stream_set_encode(stm, DEF_MBS);
    
    char buf[1024] = {0};
    strcpy(buf, "hello word");
    int len = (int)strlen(buf);
    dword_t dw = 0;
    
    if(!stream_write(stm, buf, len, &dw))
        printf("%s\n", strerror(errno));
    else
        printf("master: %s\n", buf);
    
    memset((void*)buf, 0, 1024);
    if(!stream_read(stm, buf, len, &dw))
        printf("%s\n", strerror(errno));
    else
        printf("slave: %s\n", buf);
    
    stream_free(stm);
    
    xcons_free(cons);
}

void test_path()
{
    tchar_t pfmt[] = _T("$(XSERVICE_DATA)/api/%s");
    tchar_t *path;
    
    int len = printf_path(NULL, pfmt, _T("libloc_api.dylib"));
    path = xsalloc(len + 1);
    printf_path(path, pfmt, _T("llibloc_api.dylib"));
    
    printf(path);
    printf("\n");
    
    xsfree(path);
    
}

typedef struct _sock_param{
    res_file_t so;
    res_even_t ev;
}sock_param;

void* sock_thread(void *param)
{
    sock_param* psp = (sock_param*)param;
    
    proc_info_t pi = { 0 };
    
    xdl_thread_init(0);
    
    event_sign(psp->ev, 1);
    
    net_addr_t addr;
    char token[20] = {0};
    unsigned short port = 0;
    socket_addr(psp->so, &addr);
    conv_addr(&addr, &port, token);
    
    if (create_process("../sbin/xhttps", "CERT:AAA PASS:123", SHARE_SOCK, &pi))
    {
        if(!socket_share(pi.process_id, pi.pip_write, psp->so, NULL, 0))
            printf("parent error : %s\n", strerror(errno));
        else
            printf("parent sock=%d port: %d addr: %s\n", psp->so, (int)port, token);
        
        thread_yield();
        
        release_process(&pi);
    }
    
    socket_close(psp->so);
    
    xdl_thread_uninit(0);
    
    thread_stop();
    
    return 0;
}

void test_sock()
{
    sock_param sp;
    
    sp.so = socket_tcp(0, FILE_OPEN_OVERLAP);
    sp.ev = event_create();
    
    res_thread_t tt;
    
    thread_start(&tt, (PF_THREADFUNC)sock_thread, (void*)&sp);
    
    thread_join(tt);
    
    event_destroy(sp.ev);
}

int main(int argc, const char * argv[]) {
    xdl_process_init(XDL_APARTMENT_THREAD | XDL_INITIALIZE_CONSOLE);
    
    //test_error();
    
    //test_path();
    
    //test_thread();
    
    //test_stream();
    
    //test_conv();
    
    test_sock();
    
    xdl_process_uninit();
    
    return 0;
}
