/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc comm system call document

	@module	_if_comm.c | com port system call macos implement file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
	@devnote 张文权 2017.01 - 2017.12	v5.0
	@devnote 张文权 2018.01 - 2018.12	v5.5
***********************************************************************/

/**********************************************************************
This program is free software : you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
LICENSE.GPL3 for more details.
***********************************************************************/

#include "xdkiml.h"

#ifdef XDK_SUPPORT_COMM

#define COMM_BASE_TMO		500

void _default_comm_mode(dev_com_t* pmod)
{
	pmod->baud_rate = B9600;
	pmod->byte_size = 8;
	pmod->parity = 0;
	pmod->stop_bits = 1;
}

bool_t _get_comm_mode(res_file_t fh, dev_com_t* pmod)
{
    struct termios tcb;

    if(tcgetattr(fh, &tcb) < 0)
    {
        return 0;
    }
    
    pmod->baud_rate = (short)cfgetispeed(&tcb);
    
    if(tcb.c_cflag & CS8)
        pmod->byte_size = 8;
    else if(tcb.c_cflag & CS7)
        pmod->byte_size = 7;
    else
        pmod->byte_size = 0;
    
    if(tcb.c_cflag & PARENB)
    {
        if(tcb.c_cflag & PARODD)
            pmod->parity = 1;
        else
            pmod->parity = 2;
    }else
    {
        pmod->parity = 0;
    }
    
	if(tcb.c_cflag & CSTOPB)
		pmod->stop_bits = 2;
    else
        pmod->stop_bits = 1;

    pmod->timeout = (short)tcb.c_cc[VTIME];

	return 1;
}

bool_t _set_comm_mode(res_file_t fh, const dev_com_t* pmod)
{
    struct termios tcb;
    
    if(tcgetattr(fh, &tcb) < 0)
    {
        return 0;
    }

    tcb.c_cflag &= ~(ICRNL | IXON);
    tcb.c_cflag |= CLOCAL | CREAD;
    
	if (pmod->baud_rate)
    {
        cfsetispeed(&tcb, pmod->baud_rate);
        cfsetospeed(&tcb, pmod->baud_rate);
    }
    
	if (pmod->byte_size == 8)
        tcb.c_cflag |= CS8;
    else if (pmod->byte_size == 7)
        tcb.c_cflag |= CS7;
    
	switch (pmod->parity)
	{
	case 1:
        tcb.c_cflag |= (PARODD | PARENB);
        tcb.c_iflag |= INPCK;
		break;
	case 2:
		tcb.c_cflag |= PARENB;
        tcb.c_cflag &= ~PARODD;
        tcb.c_iflag |= INPCK;
		break;
	default:
        tcb.c_cflag &= ~PARENB;
        tcb.c_iflag &= ~INPCK;
		break;
	}
    
	if (pmod->stop_bits == 2)
        tcb.c_cflag &= ~CSTOPB;
    else
        tcb.c_cflag |= CSTOPB;
    
    if(pmod->timeout)
    {
        tcb.c_cc[VTIME] = pmod->timeout / 100;
        tcb.c_cc[VMIN] = 0;
    }else
    {
        tcb.c_cc[VTIME] = pmod->timeout / 100;
        tcb.c_cc[VMIN] = 1;
    }
    
    //tcb.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); /*Input*/
    //tcb.c_oflag &= ~OPOST; /*Output*/

    tcflush(fh, TCIOFLUSH);
    
    return (tcsetattr(fh, TCSANOW, &tcb) < 0)? 0 : 1;
}

res_file_t _comm_open(const tchar_t* devname, dword_t fmode)
{
    int flag, fd;
	dev_com_t dev = { 0 };
    
    if(fmode & FILE_OPEN_WRITE)
        flag = O_RDWR;
    else
        flag = O_RDONLY;
    
    if(fmode & FILE_OPEN_OVERLAP)
        flag |= O_NONBLOCK;
    
    fd = open(devname, flag | O_NOCTTY);
    if(fd < 0)
        return INVALID_FILE;
    
    dev.timeout = COMM_BASE_TMO;
    
	_set_comm_mode(fd, &dev);
	
	return (res_file_t)fd;
}

void _comm_close(res_file_t fh)
{
	tcflush(fh, TCIOFLUSH);
    
    close(fh);
}

dword_t _comm_listen(res_file_t fd, async_t* pb)
{
    LPOVERLAPPED pov = (pb)? (LPOVERLAPPED)pb->lapp : NULL;
    dword_t* pcb = (pb) ? &(pb->size) : NULL;
    
    dword_t dwEvent = 0;
    int status;
    int rt;
    fd_set fs = {0};
    
    if(!pov)
        return 0;
    
    if(fcntl(fd,F_SETFL,FNDELAY) < 0)
        return 0;
    
    pov->tv.tv_sec = pb->timo / 1000;
    pov->tv.tv_usec = (pb->timo % 1000) * 1000;
    
    if(pcb) *pcb = 0;
    
    while(!dwEvent)
    {
        FD_ZERO(&fs);
        FD_SET(fd, &fs);
        
        if(FD_ISSET(fd, &(fs)))
        {
            rt = select(fd+1, &(fs), NULL, NULL, &(pov->tv));
            if(rt < 0)
            {
                dwEvent = COMM_EVNET_ERROR;
            }else if(rt == 0)
            {
                dwEvent = COMM_EVENT_IDLE;
            }else
            {
                dwEvent = 0;
                
                status = 0;
                ioctl(fd, FIONREAD, &status);
                if(status > 0)
                {
                    if(pcb) *pcb = status;
                
                    dwEvent = COMM_EVNET_RXCHAR;
                }else
                {
                    status = 0;
                    if(ioctl(fd, TIOCMGET, &status) < 0)
                    {
                        dwEvent = COMM_EVNET_ERROR;
                    }
                    
                    if(status & TIOCM_LE)
                    {
                        if(pcb) *pcb |= COMM_STATUS_RLSD_ON;
                        dwEvent |= COMM_EVNET_RLSD;
                    }
                    
                    if(status & TIOCM_RNG)
                    {
                        if(pcb) *pcb |= COMM_STATUS_RING_ON;
                        dwEvent |= COMM_EVNET_RING;
                    }
                    
                    if(status & TIOCM_CTS)
                    {
                        if(pcb) *pcb |= COMM_STATUS_CTS_ON;
                        dwEvent |= COMM_EVNET_CTS;
                    }
                    
                    if(status & TIOCM_DSR)
                    {
                        if(pcb) *pcb |= COMM_STATUS_DSR_ON;
                        dwEvent |= COMM_EVNET_DSR;
                    }
                }
            }
        }else
        {
            dwEvent = COMM_EVNET_BREAK;
        }
        FD_CLR(fd, &fs);
    }
    
    return dwEvent;
}

bool_t _comm_read(res_file_t fd, void* buf, dword_t size, async_t* pb)
{
    return _file_read(fd, buf, size, pb);
}

bool_t _comm_write(res_file_t fd, void* buf, dword_t size, async_t* pb)
{
    return _file_write(fd, buf, size, pb);
}

bool_t _comm_flush(res_file_t fh)
{
    return (tcflush(fh, TCOFLUSH) < 0)? 0 : 1;
}


#endif //XDK_SUPPORT_COMM