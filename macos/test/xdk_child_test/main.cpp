//
//  main.cpp
//  xdk_child_test
//
//  Created by 张文权 on 18/10/7.
//  Copyright © 2018年 张文权. All rights reserved.
//

#include <iostream>
#include <xdk.h>

void test_pipe()
{
    if_pipe_t if_pipe = { 0 };
    
    xdk_impl_pipe(&if_pipe);
    
    char buf[1024] = {0};
    size_t dw = 0;
    async_t over = {0};

    res_file_t srv = (*if_pipe.pf_pipe_srv)(NULL, 0);
    
    if(!(*if_pipe.pf_pipe_read)(srv, buf, 1024, &over))
        printf("child error : %s\n", strerror(errno));
    else
        printf("child pid=%d recv %d: %s\n", getpid(), (int)dw, buf);
        
}

void test_sock()
{
    if_pipe_t if_pipe = { 0 };
    xdk_impl_pipe(&if_pipe);
    
    if_socket_t if_sock = { 0 };
    xdk_impl_socket(&if_sock);
    
    res_file_t srv = (*if_pipe.pf_pipe_srv)(NULL, 0);
    if(srv == INVALID_FILE)
        printf("child error : %s\n", strerror(errno));
    
    res_file_t so = (*if_sock.pf_socket_dupli)(srv, 0, NULL, 0);
    if(so == INVALID_FILE)
        printf("child error : %s\n", strerror(errno));
    
    net_addr_t addr;
    char token[20] = {0};
    unsigned short port = 0;
    (*if_sock.pf_socket_addr)(so, &addr);
    (*if_sock.pf_conv_addr)(&addr, &port, token);
    
    printf("child sock=%d port: %d addr: %s\n", so, (int)port, token);
    
    (*if_sock.pf_socket_close)(so);
}

int main(int argc, const char * argv[]) {
    
    struct sigaction sa = {0};
    sa.sa_handler = SIG_IGN;
    sigaction(SIGPIPE, &sa, 0 );
    
    printf("child run...\n");
    
    if(argc > 1)
    {
        if(strstr(argv[1],"pipe") != NULL)
            test_pipe();
        else if(strstr(argv[1],"sock") != NULL)
            test_sock();
    }
    
    test_sock();
    
    return 0;
}
