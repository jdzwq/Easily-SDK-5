//
//  main.cpp
//  xdl_util_test
//
//  Created by 张文权 on 18/10/6.
//  Copyright © 2018年 张文权. All rights reserved.
//

#include <iostream>
#include <syslog.h>
#include <xdl.h>
#include <xds.h>

#include "xportd.h"

#define PID_FILE "/var/run/xportd.pid"

static void _action_term(int sig)
{
    xportd_stop();
}

static void _action_child(int sig)
{
    pid_t pid;
    int stat;
    
    while((pid = waitpid(-1, &stat, WNOHANG)) > 0);
    
    return;
}

int main(int argc, const char * argv[])
{
    FILE* f_pid = NULL;
    pid_t pid = 0;
    int ret;
    
    struct rlimit rl = {0};
    int fd,i;
    
    int err;
    struct sigaction sa = {0};
    
    char errnum[INT_LEN] = {0};
    char errtxt[ERR_LEN] = {0};
    
    char param[RES_LEN] = {0};
    
    if(argc > 1)
    {
        strcpy(param, argv[1]);
    }
    
    f_pid = fopen(PID_FILE, "r");
    if (f_pid)
    {
        ret = fscanf(f_pid, "%d", &pid);
        if (ret == EOF && ferror(f_pid) != 0){
            syslog(LOG_INFO, "read xportd.pid falied");
        }
        
        fclose(f_pid);
        f_pid = NULL;
        
        //instance is running
        if (pid)
        {
            kill(pid, SIGTERM);
            waitpid(pid, NULL, 0);
        }
        
        //empty pid
        f_pid = fopen(PID_FILE, "w");
        if (f_pid)
        {
            fclose(f_pid);
            f_pid = NULL;
        }

        syslog(LOG_INFO, "remove a zombie pidfile");
    }
    
    if(strstr(param, "shutdown") != NULL)
    {
        return 0;
    }
    
    umask(0);
    
    if((pid = fork()) < 0){
        syslog(LOG_INFO, "fork first error");
        return -1;
    }
#if defined(DEBUG)
    //kill(0, SIGSTOP);
#endif
    if (pid != 0) {
        //parent exit...
        exit(0);
    }
    
    if ((pid = setsid()) < 0){
        raise_user_error("-1" ,"xportd: setsid error");
    }
    
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    sigaction(SIGHUP, &sa, NULL);
    
    if((pid = fork()) < 0){
        syslog(LOG_INFO, "fork next error");
        return -1;
    }
#if defined(DEBUG)
    //kill(0, SIGSTOP);
#endif
    if (pid != 0) {
        //parent exit...
        exit(0);
    }

    //child process begin...

    if (chdir("/") < 0) {
        syslog(LOG_INFO,  "xportd: chidr change directory error");
        exit(-1);
    }
    
    if(getrlimit(RLIMIT_NOFILE, &rl) >= 0)
    {
        if ( rl.rlim_max == RLIM_INFINITY ){
            rl.rlim_max = 1024;
        }
        for(i = 0 ; i < rl.rlim_max; i ++) {
            close(i);
        }
    }

    fd = open("/dev/null",O_RDWR,0);
    if(fd != -1){
        dup2(fd,STDIN_FILENO);
        dup2(fd,STDOUT_FILENO);
        dup2(fd,STDERR_FILENO);
    }
    if (fd > 2){
        close(fd);
        syslog(LOG_INFO,  "xportd: /dev/null opened");
        exit(-1);
    }
    
    fd = creat(PID_FILE, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if(fd != -1){
        f_pid = fdopen(fd,"w");
    }

    if (!f_pid){
        syslog(LOG_INFO, "write xportd.pid falied");
        exit(-1);
    }
    
    fprintf(f_pid, "%d", getpid());
    fclose(f_pid);
    f_pid = NULL;
    
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = _action_term;
    sa.sa_flags = SA_SIGINFO | SA_RESETHAND;
    ret = sigaction(SIGINT, &sa, 0);
    ret = sigaction(SIGTERM, &sa, 0);
    
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    ret = sigaction(SIGCHLD, &sa, 0);

    xdl_process_init(XDL_APARTMENT_THREAD | XDL_INITIALIZE_SERVER);
    
    xportd_start();
    
    while(1) {
        if(xportd_state() == XPORTD_STATE_STOPPED)
            break;
        
        usleep(100000);
    }
    
    //empty pid
    f_pid = fopen(PID_FILE, "w");
    if (f_pid)
    {
        fclose(f_pid);
        f_pid = NULL;
    }
    
    unlink(PID_FILE);
    
    xdl_process_uninit();
    
    return 0;
}
