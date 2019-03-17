/***********************************************************************
	Easily xdk v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc file system call document

	@module	_if_comm.c | com port system call windows implement file

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
	pmod->baud_rate = CBR_9600;
	pmod->byte_size = 8;
	pmod->parity = NOPARITY;
	pmod->stop_bits = 1;
}

bool_t _get_comm_mode(res_file_t fh, dev_com_t* pmod)
{
	DCB dcb = { 0 };
	COMMTIMEOUTS tms = { 0 };
	BOOL rt = 0;

	dcb.DCBlength = sizeof(dcb);
	rt = GetCommState(fh, &dcb);

	if (rt)
	{
		pmod->baud_rate = (short)dcb.BaudRate;
		pmod->byte_size = (short)dcb.ByteSize;
		pmod->parity = dcb.Parity;
		pmod->stop_bits = dcb.StopBits;
	}

	rt = GetCommTimeouts(fh, &tms);
	pmod->timeout = (short)tms.ReadTotalTimeoutConstant;

	return rt;
}

bool_t _set_comm_mode(res_file_t fh, const dev_com_t* pmod)
{
	DCB dcb = { 0 };
	COMMTIMEOUTS tms = { 0 };
	BOOL rt = 0;

	dcb.DCBlength = sizeof(dcb);
	GetCommState(fh, &dcb);

	if (pmod->baud_rate)
		dcb.BaudRate = (DWORD)pmod->baud_rate;
	if (pmod->byte_size)
		dcb.ByteSize = (BYTE)pmod->byte_size;
	switch (pmod->parity)
	{
	case 1:
		dcb.Parity = ODDPARITY;
		break;
	case 2:
		dcb.Parity = EVENPARITY;
		break;
	default:
		dcb.Parity = (BYTE)pmod->parity;
		break;
	}
	if (pmod->stop_bits)
		dcb.StopBits = (BYTE)pmod->stop_bits;

	rt = SetCommState(fh, &dcb);

	GetCommTimeouts(fh, &tms);

	if (pmod->timeout)
	{
		tms.ReadTotalTimeoutConstant = pmod->timeout;
		tms.ReadIntervalTimeout = MAXDWORD;
		tms.ReadTotalTimeoutMultiplier = MAXDWORD;

		rt = SetCommTimeouts(fh, &tms);
	}

	return (bool_t)rt;
}

res_file_t _comm_open(const tchar_t* devname, u32_t fmode)
{
	HANDLE hFile = 0;
	DWORD dwAccess, dwShare, dwCreate, dwFlag;
	dev_com_t dev = { 0 };

	if (fmode & FILE_OPEN_APPEND)
	{
		dwCreate = OPEN_EXISTING;
		dwAccess = GENERIC_READ | GENERIC_WRITE;
	}
	else if (fmode & FILE_OPEN_CREATE)
	{
		dwCreate = OPEN_EXISTING;
		dwAccess = GENERIC_READ | GENERIC_WRITE;
	}
	else if (fmode & FILE_OPEN_WRITE)
	{
		dwCreate = OPEN_EXISTING;
		dwAccess = GENERIC_READ | GENERIC_WRITE;
	}
	else
	{
		dwCreate = OPEN_EXISTING;
		dwAccess = GENERIC_READ;
	}

	dwShare = FILE_SHARE_READ | FILE_SHARE_WRITE;

	if (fmode & FILE_OPEN_OVERLAP)
		dwFlag = FILE_FLAG_OVERLAPPED;
	else
		dwFlag = FILE_ATTRIBUTE_NORMAL;

	hFile = CreateFile(devname, dwAccess, dwShare, NULL, dwCreate, dwFlag, NULL);
	if ((HANDLE)hFile == INVALID_HANDLE_VALUE)
	{
		return INVALID_FILE;
	}

	dev.timeout = COMM_BASE_TMO;
	_set_comm_mode(hFile, &dev);
	
	return hFile;
}

void _comm_close(res_file_t fh)
{
	PurgeComm(fh, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

	CloseHandle(fh);
}

u32_t _comm_wait(res_file_t fh, async_t* pb)
{
	LPOVERLAPPED pov = (pb) ? (LPOVERLAPPED)pb->lapp : NULL;
	size_t* pcb = (pb) ? &(pb->size) : NULL;

	DWORD dwCount, dwError, dwLine, dwEvent = 0;
	COMMTIMEOUTS tms = { 0 };
	COMSTAT sta = { 0 };

	DWORD dw = 0;
	ULONG_PTR up = NULL;
	LPOVERLAPPED ul = NULL;

	GetCommTimeouts(fh, &tms);

	PurgeComm(fh, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	ClearCommError(fh, &dwError, NULL);
	ClearCommBreak(fh);
	SetCommMask(fh, EV_RING | EV_RLSD | EV_CTS | EV_DSR | EV_RXCHAR | EV_TXEMPTY | EV_BREAK | EV_ERR);

	dwCount = 0;
	dwEvent = 0;
	while (!dwEvent)
	{
		*pcb = 0;

		if (!WaitCommEvent(fh, &dwEvent, pov))
		{
			if (GetLastError() != ERROR_IO_PENDING)
			{
				dwEvent = COMM_EVNET_ERROR;
			}

			if (pb->type == ASYNC_QUEUE)
			{
				if (GetQueuedCompletionStatus((HANDLE)pb->port, &dw, &up, &ul, ((pb->timo)? pb->timo : INFINITE)))
				{
					ResetEvent(pov->hEvent);
				}
			}
			else
			{
				if (WaitForSingleObject(pov->hEvent, ((pb->timo)? pb->timo : INFINITE)) == WAIT_OBJECT_0)
				{
					ResetEvent(pov->hEvent);
				}
			}
		}

		dwCount++;
		if (!dwEvent && dwCount == tms.ReadTotalTimeoutConstant)
		{
			dwEvent = COMM_EVENT_IDLE;
		}

		if (dwEvent & EV_ERR)
		{
			ClearCommError(fh, &dwError, NULL);
			if (pcb)
			{
				*pcb = (size_t)dwError;
			}
			dwEvent = COMM_EVNET_ERROR;
		}

		if (dwEvent & EV_BREAK)
		{
			ClearCommError(fh, &dwError, NULL);
			if (pcb)
			{
				*pcb = (size_t)dwError;
			}
			dwEvent = COMM_EVNET_BREAK;
		}

		if (dwEvent & EV_RING)
		{
			GetCommModemStatus(fh, &dwLine);
			if (pcb)
			{
				*pcb |= dwLine;
			}
			dwEvent |= COMM_EVNET_RING;
		}

		if (dwEvent & EV_RLSD)
		{
			GetCommModemStatus(fh, &dwLine);
			if (pcb)
			{
				*pcb |= dwLine;
			}
			dwEvent |= COMM_EVNET_RLSD;
		}

		if (dwEvent & EV_CTS)
		{
			GetCommModemStatus(fh, &dwLine);
			if (pcb)
			{
				*pcb |= dwLine;
			}
			dwEvent |= COMM_EVNET_CTS;
		}

		if (dwEvent & EV_DSR)
		{
			GetCommModemStatus(fh, &dwLine);
			if (pcb)
			{
				*pcb |= dwLine;
			}
			dwEvent |= COMM_EVNET_DSR;
		}

		if (dwEvent & EV_RXCHAR)
		{
			ClearCommError(fh, &dwError, &sta);
			if (pcb)
			{
				*pcb = sta.cbInQue;
			}
			dwEvent |= COMM_EVNET_RXCHAR;
		}

		if (dwEvent & EV_TXEMPTY)
		{
			ClearCommError(fh, &dwError, &sta);
			if (pcb)
			{
				*pcb = sta.cbOutQue;
			}
			dwEvent |= COMM_EVNET_TXEMPTY;
		}
	}

	return dwEvent;
}

bool_t _comm_read(res_file_t fh, void* buf, size_t size, async_t* pb)
{
	return _file_read(fh, buf, size, pb);
}

bool_t _comm_write(res_file_t fh, void* buf, size_t size, async_t* pb)
{
	return _file_write(fh, buf, size, pb);
}

bool_t _comm_flush(res_file_t fh)
{
	return _file_flush(fh);
}

#endif //XDK_SUPPORT_COMM