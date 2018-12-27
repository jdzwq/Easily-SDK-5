//
//  main.cpp
//  xdk_lib_test
//
//  Created by 张文权 on 18/10/4.
//  Copyright © 2018年 张文权. All rights reserved.
//

#include <iostream>
#include <xdk.h>

void test_date()
{
    if_date_t if_date = { 0 };
    
    xdk_impl_date(&if_date);
    
    xdate_t dt;
    
    (*if_date.pf_get_loc_date)(&dt);
    printf("%d-%02d-%02d %02d %02d:%02d:%02d\n", dt.year, dt.mon, dt.day, dt.wday, dt.hour, dt.min, dt.sec);
    
    dt.day += 1;
    (*if_date.pf_mak_loc_date)(&dt);
    printf("%d-%02d-%02d %02d %02d:%02d:%02d\n", dt.year, dt.mon, dt.day, dt.wday,dt.hour, dt.min, dt.sec);
    (*if_date.pf_get_loc_date)(&dt);
    
    (*if_date.pf_get_utc_date)(&dt);
    printf("%d-%02d-%02d %02d %02d:%02d:%02d\n", dt.year, dt.mon, dt.day, dt.wday, dt.hour, dt.min, dt.sec);
}

void test_file()
{
    if_file_t if_file = { 0 };
    
    xdk_impl_file(&if_file);

    file_info_t fi = {0};
    
    (*if_file.pf_file_info)("./body.bmp", &fi);
    
    printf("size: %d time: %d-%02d-%02d %02d:%02d:%02d\n", (int)fi.low_size, fi.write_time.year, fi.write_time.mon, fi.write_time.day, fi.write_time.hour, fi.write_time.min, fi.write_time.sec);
    
    void* buf = calloc(1, fi.low_size);
    
    res_file_t fh = (*if_file.pf_file_open)("./body.bmp", 0);
    
    async_t over = {0};
    
    (*if_file.pf_file_read)(fh, buf, fi.low_size, &over);
    
    res_file_t fh2 = (*if_file.pf_file_open)("./body2.bmp", FILE_OPEN_CREATE);
    (*if_file.pf_file_write)(fh2, buf, over.size, &over);
    (*if_file.pf_file_close)(fh2);

    
    free(buf);
    (*if_file.pf_file_close)(fh);
    
    (*if_file.pf_file_rename)("./body2.bmp","./body3.bmp");
    
     (*if_file.pf_file_delete)("./body3.bmp");
}

void test_dir()
{
    if_process_t if_proc = { 0 };
    
    xdk_impl_process(&if_proc);
    
    char path[PATH_LEN] = {0};
    (*if_proc.pf_get_runpath)(NULL, path, PATH_LEN);
    printf("running path: %s\n", path);

    if_file_t if_file = { 0 };
    
    xdk_impl_file(&if_file);
    xdk_impl_file_find(&if_file);
    
    file_info_t fi;
    
    res_find_t fd = (*if_file.pf_file_find_first)(path, &fi);
    while(fd && (*if_file.pf_file_find_next)(fd, &fi))
    {
        printf("find file: %s\n", fi.file_name);
    }

    strcat(path,"/test/mydir");
    (*if_file.pf_directory_open)(path, FILE_OPEN_CREATE);

}

void test_name_pipe()
{
    if_pipe_t if_pipe = { 0 };
    
    xdk_impl_pipe(&if_pipe);
    
    char buf[1024] = {0};
    size_t dw = 0;
    int status;
    pid_t pid;
    async_t over = {0};
    
    res_file_t srv = 0, cli=0;
    
    srv = (*if_pipe.pf_pipe_srv)("xportm", FILE_OPEN_READ | FILE_OPEN_OVERLAP);
    if(!srv)
        printf("parent error : %s\n", strerror(errno));
    
    if((pid = fork()) == 0){
        close(srv);
        
        cli = (*if_pipe.pf_pipe_cli)("xportm", FILE_OPEN_WRITE | FILE_OPEN_OVERLAP);
        if(!cli)
             printf("child error : %s\n", strerror(errno));
        
        strcpy(buf,"hello word!\n");
        dw = strlen(buf);
        (*if_pipe.pf_pipe_write)(cli, buf, dw, &over);

        printf("child pid=%d write %d: %s\n", getpid(), (int)dw, buf);
        
        if(!dw)
            printf("child error : %s\n", strerror(errno));
        
        (*if_pipe.pf_pipe_close)(NULL, cli);
    }else{
        memset((void*)buf,0,1024);
        dw = 0;
        (*if_pipe.pf_pipe_read)(srv, buf, 1024, &over);

        printf("parent pid=%d recv %d: %s\n", getpid(), (int)dw, buf);
        if(!dw)
            printf("parent error : %s\n", strerror(errno));
        
         waitpid(pid, &status, 0);
         (*if_pipe.pf_pipe_close)("xportm", srv);
    }
}

void test_process()
{
    if_process_t if_proc = { 0 };
    
    xdk_impl_process(&if_proc);
    
    sys_info_t si = {0};
    (*if_proc.pf_system_info)(&si);
    printf("processer: %d,page size: %d\n", si.processor_number, si.page_size);
    
    
    char path[PATH_LEN] = {0};
    (*if_proc.pf_get_runpath)(NULL, path, PATH_LEN);
    printf("running path: %s\n", path);
    
    typedef void (*PF_DEFAULT_XPEN)(xpen_t* pxp);
    
    res_modu_t dl = (*if_proc.pf_load_library)("./libxdk.dylib");
    PF_DEFAULT_XPEN pf = (PF_DEFAULT_XPEN)(*if_proc.pf_get_address)(dl, "default_xpen");
    xpen_t xp = {0};
    //(*pf)(&xp);
    (*if_proc.pf_free_library)(dl);
    
    proc_info_t pi = {0};
    (*if_proc.pf_create_process)("./xdk_child_test", NULL, 0, &pi);
    
}

void test_exec()
{
    if_process_t if_proc = { 0 };
    xdk_impl_process(&if_proc);
    
    if_pipe_t if_pipe = { 0 };
    xdk_impl_pipe(&if_pipe);

    proc_info_t pi = {0};
    (*if_proc.pf_create_process)("./xdk_child_test", "pipe", 1, &pi);
    
    char buf[20] = {0};
    strcpy(buf,"hello word!");
    
    async_t over = {0};
    
    size_t dw = strlen(buf);
    if(!(*if_pipe.pf_pipe_write)(pi.pip_write, buf, dw, &over))
        printf("parent error : %s\n", strerror(errno));
    else
        printf("parent pid=%d send %d: %s\n", getpid(), (int)dw, buf);
    
    sleep(3);
}

void test_sock()
{
    if_process_t if_proc = { 0 };
    xdk_impl_process(&if_proc);
    
    if_socket_t if_sock = { 0 };
    xdk_impl_socket(&if_sock);
    
    res_file_t so = (*if_sock.pf_socket_tcp)(0, FILE_OPEN_OVERLAP);
    net_addr_t addr;
    bool_t rt;
    
    (*if_sock.pf_fill_addr)(&addr, 8000, "127.0.0.1");
    rt = (*if_sock.pf_socket_bind)(so, &addr, sizeof(addr));
    //rt = (*if_sock.pf_socket_listen)(so, SOMAXCONN);
    
    char token[20] = {0};
    unsigned short port = 0;
    (*if_sock.pf_socket_addr)(so, &addr);
    (*if_sock.pf_conv_addr)(&addr, &port, token);
    
    proc_info_t pi = {0};
    (*if_proc.pf_create_process)("./xdk_child_test", "sock", 2, &pi);
    
    if(!(*if_sock.pf_socket_share)(pi.process_id, pi.pip_write, so, NULL, 0))
        printf("parent error : %s\n", strerror(errno));  
    else
        printf("parent sock=%d port: %d addr: %s\n", so, (int)port, token);
    
    (*if_sock.pf_socket_close)(so);
}

void test_heap()
{
    if_memo_t if_memo = { 0 };
    
    xdk_impl_memo_heap(&if_memo);
    
    res_heap_t hp = (*if_memo.pf_heap_create)();
    
    malloc_statistics_t ms = {0};
    
    void *p = (*if_memo.pf_heap_alloc)(hp, 500);
    if(!p)
        printf("parent error : %s\n", strerror(errno));
    
    malloc_zone_statistics((malloc_zone_t*)hp, &ms);
    
    p = (*if_memo.pf_heap_realloc)(hp, p, 10000);
    if(!p)
        printf("parent error : %s\n", strerror(errno));
    
     malloc_zone_statistics((malloc_zone_t*)hp, &ms);
    
    (*if_memo.pf_heap_free)(hp, p);
    
    (*if_memo.pf_heap_clean)(hp);
    
    (*if_memo.pf_heap_destroy)(hp);
}

void test_page()
{
    if_memo_t if_memo = { 0 };
    
    xdk_impl_memo_page(&if_memo);
    
    void* p = (*if_memo.pf_page_alloc)(1024);
    size_t dw = (*if_memo.pf_page_size)(p);
    p = (*if_memo.pf_page_lock)(p);
    (*if_memo.pf_page_unlock)(p);
    p = (*if_memo.pf_page_realloc)(p, 4096);
    (*if_memo.pf_page_free)(p);
}

void test_cache()
{
    if_memo_t if_memo = { 0 };
    
    xdk_impl_memo_cache(&if_memo);

    void *p = (*if_memo.pf_cache_open)();
    if(!p)
        printf("parent error : %s\n", strerror(errno));
    
    char buf[4096] = {0};
    strcpy(buf, "hello word!");
    
    size_t dw = 0;
    (*if_memo.pf_cache_write)(p, 10, buf, 4096, &dw);
    printf("write: %s\n", buf);
    
    memset((void*)buf,0,4096);
    dw = 0;
    (*if_memo.pf_cache_read)(p, 10, buf, 4096, &dw);
    printf("read: %s\n", buf);

    (*if_memo.pf_cache_close)(p);
}

void test_block()
{
    if_file_t if_file = { 0 };
    
    xdk_impl_file_block(&if_file);
    
    res_file_t fh = (*if_file.pf_block_open)("./demo.txt", FILE_OPEN_CREATE);
    if(!fh)
        printf("parent error : %s\n", strerror(errno));
    
    char buf[4096] = {0};
    size_t dw = 0;
    
    strcpy(buf, "hello word!");

    if(!(*if_file.pf_block_write)(fh, 0, 0, buf, 4096, &dw))
        printf("parent error : %s\n", strerror(errno));
    
    memset((void*)buf,0,4096);
    dw = 0;
    if(!(*if_file.pf_block_read)(fh, 0, 0, buf, 4096, &dw))
        printf("parent error : %s\n", strerror(errno));
    
    if(!(*if_file.pf_block_truncate)(fh, 0, 2))
        printf("parent error : %s\n", strerror(errno));
    
    (*if_file.pf_block_close)(fh);
}

void test_share()
{
    if_share_t if_share = { 0 };
    
    xdk_impl_share(&if_share);
    
    res_file_t fh = (*if_share.pf_share_srv)("mytest","./demo.txt",1024);
    if(!fh)
        printf("parent error : %s\n", strerror(errno));
    
    char buf[4096] = {0};
    size_t dw = 0;
    
    strcpy(buf, "hello word!");
    
    if(!(*if_share.pf_share_write)(fh, 0, buf, 4096, &dw))
        printf("parent error : %s\n", strerror(errno));
    else
        printf("parent write : %s\n", buf);
    
    pid_t pid;
    int status;
    
    pid = fork();
    kill(0, SIGSTOP);
    
    if(pid == 0)
    {
        res_file_t ch = (*if_share.pf_share_cli)("mytest",1024);
        if(!ch)
            printf("child error : %s\n", strerror(errno));
        
        memset((void*)buf,0,4096);
        dw = 0;
        if(!(*if_share.pf_share_read)(ch, 0, buf, 4096, &dw))
            printf("child error : %s\n", strerror(errno));
        else
            printf("child read : %s\n", buf);
        
        (*if_share.pf_share_close)("mytest", ch);
    }
    else{
        waitpid(pid, &status, 0);
        (*if_share.pf_share_close)("mytest", fh);
    }
}

void test_mbcs()
{
    if_mbcs_t if_mbcs = { 0 };
    
    xdk_impl_mbcs(&if_mbcs);
    
    char utf_buf[] = "我的中国";
    wchar_t ucs_buf[20] = {0};
    char gbk_buf[20] = {0};
    
    int n = (*if_mbcs.pf_utf8_to_ucs)(utf_buf, -1, NULL, 20);
    (*if_mbcs.pf_utf8_to_ucs)(utf_buf, -1, ucs_buf, n);
    printf("UTF-8: %s\n", utf_buf);
    wprintf(L"UCS: %S\n", ucs_buf);
    
    n = (*if_mbcs.pf_ucs_to_gbk)(ucs_buf, -1, NULL, 20);
    (*if_mbcs.pf_ucs_to_gbk)(ucs_buf, -1, gbk_buf, n);
    printf("GBK: %s\n", gbk_buf);
    wprintf(L"UCS: %S\n", ucs_buf);
}

void test_cons()
{
    if_cons_t if_cons = { 0 };
    
    xdk_impl_cons(&if_cons);
    
    tchar_t cname[50] = {0};
    
    res_file_t con = (*if_cons.pf_cons_alloc)(cname, 50);
    if(!con)
        printf("error : %s\n", strerror(errno));
    else
        printf("device : %s\n", cname);
    
    strcat(cname,"\n");
    size_t len = strlen(cname);
    
    if((*if_cons.pf_cons_write)(con, cname, len, &len))
        printf("master write : %s\n", cname);
    else
        printf("error : %s\n", strerror(errno));
    
    memset((void*)cname,0,50);
    if((*if_cons.pf_cons_read)(con, cname, len, &len))
        printf("slave read : %s\n", cname);
    else
        printf("error : %s\n", strerror(errno));
    
    (*if_cons.pf_cons_free)(con);
}

void test_comm()
{
    if_comm_t if_comm = { 0 };
    
    xdk_impl_comm(&if_comm);
    
    res_file_t com = (*if_comm.pf_comm_open)(_T("/dev/tty.BF4030-SerialPort"), FILE_OPEN_OVERLAP);
    if(!com)
        printf("error : %s\n", strerror(errno));
    
    char buf[1024] = { 0 };
    async_t over = {0};
    size_t msk = 0;
    size_t dw;
    int idle = 0;
    
    while (msk = (*if_comm.pf_comm_listen)(com, &over))
    {
        if (msk == COMM_EVNET_ERROR)
        {
            printf("comm ERR:~\n");
            break;
        }
        
        if (msk == COMM_EVNET_BREAK)
        {
            printf("comm BREAK:~\n");
        }
        
        if (msk == COMM_EVENT_IDLE)
        {
            idle ++;
            printf("comm IDLE:~\n");
        }
        
        if (msk & COMM_EVNET_RING)
        {
            if (dw & COMM_STATUS_RING_ON)
                printf("comm RING: on\n");
            else
            {
                printf("comm RING: off\n");
                break;
            }
        }
        
        if (msk & COMM_EVNET_RLSD)
        {
            if (dw & COMM_STATUS_RLSD_ON)
                printf("comm RLSD: on\n");
            else
                printf("comm RLSD: off\n");
        }
        
        if (msk & COMM_EVNET_CTS)
        {
            if (dw & COMM_STATUS_CTS_ON)
                printf("comm CTS: on\n");
            else
                printf("comm CTS: off\n");
        }
        
        if (msk & COMM_EVNET_DSR)
        {
            if (dw & COMM_STATUS_DSR_ON)
                printf("comm DSR: on\n");
            else
                printf("comm DSR: off\n");
        }
        
        if (msk & COMM_EVNET_RXCHAR)
        {
            (*if_comm.pf_comm_read)(com, (char*)buf, dw, &over);
            buf[dw] = 0;
            
            if (dw)
            {
                printf("comm DATA: %s\n", (char*)buf);
                
            }
        }
        
        if(idle > 10)
            break;
    }
    
    (*if_comm.pf_comm_close)(com);
    
}

void test_context()
{
    if_context_t if_context = { 0 };
    
    xdk_impl_context(&if_context);
    
    (*if_context.pf_context_startup)();
    
    dev_cap_t dev = {0};
    
    (*if_context.pf_get_device_caps)(NULL, &dev);
    
    res_ctx_t rdc = (*if_context.pf_create_display_context)();
    
    res_ctx_t xdc = (*if_context.pf_create_compatible_context)(rdc);
    
    xfont_t xf;
    default_xfont(&xf);
    //_tstrcpy(xf.family, "Roman");
    
    float fw, fh;
    
    (*if_context.pf_text_mm_size)(rdc, &xf, "hello world!", -1, &fw, &fh);
    
    float f = (*if_context.pf_pt_per_mm)(rdc,1);
    
    int s = (*if_context.pf_font_size)(rdc,7 * 3.77);
    
    (*if_context.pf_destroy_context)(rdc);
    
    (*if_context.pf_context_cleanup)();
}

void test_bitmap()
{
    if_context_t if_context = { 0 };
    
    xdk_impl_context(&if_context);
    
    xdk_impl_context_bitmap(&if_context);

    (*if_context.pf_context_startup)();
    
    res_ctx_t rdc = (*if_context.pf_create_display_context)();
    
    res_bmp_t bmp = (*if_context.pf_create_compatible_bitmap)(rdc, 32, 32);
    
    int n = (*if_context.pf_save_bitmap_to_bytes)(rdc, bmp, NULL, MAX_LONG);
    
    unsigned char* buf = (unsigned char*)calloc(1, n);
    
    (*if_context.pf_save_bitmap_to_bytes)(rdc, bmp, buf, n);
    
    free(buf);
    
    (*if_context.pf_destroy_bitmap)(bmp);
    
    xcolor_t clr;
    parse_xcolor(&clr, GDI_ATTR_RGB_AZURE);

    bmp = (*if_context.pf_create_color_bitmap)(rdc, &clr, 32, 32);
    
    (*if_context.pf_destroy_bitmap)(bmp);

    (*if_context.pf_destroy_context)(rdc);

    (*if_context.pf_context_cleanup)();
}

int main(int argc, const char * argv[]) {
    struct sigaction sa = {0};
    sa.sa_handler = SIG_IGN;
    sigaction(SIGPIPE, &sa, 0 );
    
    // insert code here...
    
    //test_date();
    
    //test_file();
    
    //test_dir();
    
    //test_null_pipe();
    
    //test_name_pipe();
    
    //test_process();
    
    //test_exec();
    
    //test_sock();
    
    //test_heap();
    
    //test_page();
    
    //test_cache();
    
    //test_block();
    
    //test_share();
    
    //test_mbcs();
    
    //test_cons();
    
    //test_comm();
    
    //test_context();
    
    test_bitmap();
    
    return 0;
}
