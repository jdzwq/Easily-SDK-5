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
    
    xdl_thread_init();
    
    TRY_CATCH;
    
    raise_user_error("-1", "thread error");
    
    END_CATCH;
    
    xdl_thread_uninit(0);
    
    return NULL;
ONERROR:
    
    get_last_error(errnum, errtxt, ERR_LEN);
    
    printf("%s\n", errtxt);

    xdl_thread_uninit(0);
    
    xevent_sign(ev, 1);
    
    return NULL;
}

void test_thread()
{
    tchar_t errnum[INT_LEN];
    tchar_t errtxt[ERR_LEN];

    res_even_t ev;
    
    TRY_CATCH;
    
    ev = xevent_create();
    
    xthread_begin(thr_fun,(void*)ev);
    
    xevent_wait(ev, -1);
    xevent_destroy(ev);
    
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



int main(int argc, const char * argv[]) {
    xdl_process_init(XDL_APARTMENT_THREAD);
    
    //test_error();
    
    //test_thread();
    
    //test_stream();
    
    test_conv();
    
    xdl_process_uninit();
    
    return 0;
}
