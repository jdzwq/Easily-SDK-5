#include <stdio.h>
#include <sys/resource.h>
#include <syslog.h>

#include <xdl.h>
#include <xds.h>

#include "xportd.h"

#define PID_FILE "/var/run/xportd.pid"

void _action_term(int sig);

int main(int argc, const char * argv[])
{
    FILE* f_pid = NULL;
    pid_t pid = 0;
    int ret;
    
    struct rlimit rl = {0};
    int fd,i;
    
    struct sigaction sa = {0};
    
    char errnum[INT_LEN] = {0};
    char errtxt[ERR_LEN] = {0};
    
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
        if (pid && kill(pid, 0) == 0)
            return 0;
    
        //unlink(PID_FILE);
        //syslog(LOG_INFO, "remove a zombie pidfile");
    }
    
    umask(0);
    
    if((pid = fork()) < 0){
        syslog(LOG_INFO, "fork first error");
        return -1;
    }
#if defined(_DEBUG) || defined(DEBUG)
    //kill(0, SIGSTOP);
#endif
    if (pid != 0) {
        //parent exit...
        exit(0);
    }
    
    if ((pid = setsid()) < 0){
        raise_user_error("-1" ,"xportd: setsid error");
    }
    
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGHUP, &sa, NULL);
    
    if((pid = fork()) < 0){
        syslog(LOG_INFO, "fork next error");
        return -1;
    }
#if defined(_DEBUG) || defined(DEBUG)
    //kill(0, SIGSTOP);
#endif
    if (pid != 0) {
        //parent exit...
        exit(0);
    }

    //child process begin...
    xdl_process_init(XDL_APARTMENT_THREAD | XDL_INITIALIZE_SERVER);
    
    TRY_CATCH;

    if (chdir("/") < 0) {
        raise_user_error("-1", "xportd: chidr change directory error");
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
        fd = 0;
    }
    
    f_pid = fopen(PID_FILE, "w");
    if (!f_pid){
        raise_user_error("-1", "xportd write pidfile failed");
    }
    
    fprintf(f_pid, "%d", getpid());
    fclose(f_pid);
    f_pid = NULL;
    
    xportd_start();
    
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = _action_term;
    sa.sa_flags = SA_SIGINFO | SA_RESETHAND;
    
    ret = sigaction(SIGALRM, &sa, 0);
    ret = sigaction(SIGINT, &sa, 0);
    ret = sigaction(SIGTERM, &sa, 0);
    
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = SIG_IGN;
    ret = sigaction( SIGPIPE, &sa, 0 );
    
    while(1) {
        if(xportd_state() == XPORTD_STATE_STOPPED)
            break;
        
        usleep(100000);
    }
    
    END_CATCH;
    
    xdl_process_uninit();
    
    return 0;
ONERROR:
    
    if(f_pid)
        fclose(f_pid);
    
    get_last_error(errnum, errtxt, ERR_LEN);
    
    syslog(LOG_INFO, "%s\n", errtxt);
    
    xdl_process_uninit();
    
    exit((int)xstol(errnum));
}

void _action_term(int sig)
{
    xportd_stop();
}