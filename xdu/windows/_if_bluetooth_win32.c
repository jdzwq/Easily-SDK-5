/***********************************************************************
	Easily xdu v5.5

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc socket document

	@module	if_socket_win.c | socket macos implement file

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

#include "xduiml.h"

#ifdef XDU_SUPPORT_BLUT

#ifdef WINCE
#pragma comment(lib,"ws2.lib")
#else
#pragma comment(lib,"ws2_32.lib")
#endif

#pragma comment(lib, "Bthprops.lib")
#pragma comment(lib, "Irprops.lib")

static void _blt_device_class(const tchar_t* dev_class, tchar_t* major_class, tchar_t* minor_class)
{
    int major,minor;

    major = (int)(dev_class[1] & 0x1f);
    minor = (int)(dev_class[0] >> 2);

	switch (major) {
	case 0:
        xscpy(major_class, _T("Miscellaneous"));
        xscpy(minor_class, _T(""));
		return;
	case 1:
         xscpy(major_class, _T("Computer"));
		switch(minor) {
		case 0:
			xscpy(minor_class, _T("Uncategorized"));
            return;
		case 1:
			xscpy(minor_class, _T("Desktop workstation"));
            return;
		case 2:
			xscpy(minor_class, _T("Server"));
            return;
		case 3:
			xscpy(minor_class, _T("Laptop"));
            return;
		case 4:
			xscpy(minor_class, _T("Handheld"));
            return;
		case 5:
			xscpy(minor_class, _T("Palm"));
            return;
		case 6:
			xscpy(minor_class, _T("Wearable"));
            return;
		}
		break;
	case 2:
        xscpy(major_class, _T("Phone"));
		switch(minor) {
		case 0:
			xscpy(minor_class, _T("Uncategorized"));
            return;
		case 1:
			xscpy(minor_class, _T("Cellular"));
            return;
		case 2:
			xscpy(minor_class, _T("Cordless"));
            return;
		case 3:
			xscpy(minor_class, _T("Smart phone"));
            return;
		case 4:
			xscpy(minor_class, _T("Wired modem or voice gateway"));
            return;
		case 5:
			xscpy(minor_class, _T("Common ISDN Access"));
            return;
		case 6:
			xscpy(minor_class, _T("Sim Card Reader"));
            return;
		}
		break;
	case 3:
        xscpy(major_class, _T("LAN Access"));
		if (minor == 0)
        {
			xscpy(minor_class, _T("Uncategorized"));
            return;
        }
		switch(minor / 8) {
		case 0:
			xscpy(minor_class, _T("Fully available"));
            return;
		case 1:
			xscpy(minor_class, _T("1-17% utilized"));
            return;
		case 2:
			xscpy(minor_class, _T("17-33% utilized"));
            return;
		case 3:
			xscpy(minor_class, _T("33-50% utilized"));
            return;
		case 4:
			xscpy(minor_class, _T("50-67% utilized"));
            return;
		case 5:
			xscpy(minor_class, _T("67-83% utilized"));
            return;
		case 6:
			xscpy(minor_class, _T("83-99% utilized"));
            return;
		case 7:
			xscpy(minor_class, _T("No service available"));
            return;
		}
		break;
	case 4:
        xscpy(major_class, _T("Audio/Video"));
		switch(minor) {
		case 0:
			xscpy(minor_class, _T("Uncategorized"));
            return;
		case 1:
			xscpy(minor_class, _T("Device conforms to the Headset profile"));
            return;
		case 2:
			xscpy(minor_class, _T("Hands-free"));
            return;
        case 3:
            xscpy(minor_class, _T("Reserved"));
            return;
		case 4:
			xscpy(minor_class, _T("Microphone"));
            return;
		case 5:
			xscpy(minor_class, _T("Loudspeaker"));
            return;
		case 6:
			xscpy(minor_class, _T("Headphones"));
            return;
		case 7:
			xscpy(minor_class, _T("Portable Audio"));
            return;
		case 8:
			xscpy(minor_class, _T("Car Audio"));
            return;
		case 9:
			xscpy(minor_class, _T("Set-top box"));
            return;
		case 10:
			xscpy(minor_class, _T("HiFi Audio Device"));
            return;
		case 11:
			xscpy(minor_class, _T("VCR"));
            return;
		case 12:
			xscpy(minor_class, _T("Video Camera"));
            return;
		case 13:
			xscpy(minor_class, _T("Camcorder"));
            return;
		case 14:
			xscpy(minor_class, _T("Video Monitor"));
            return;
		case 15:
			xscpy(minor_class, _T("Video Display and Loudspeaker"));
            return;
		case 16:
			xscpy(minor_class, _T("Video Conferencing"));
            return;
        case 17:
            xscpy(minor_class, _T("Reserved"));
            return;
		case 18:
			xscpy(minor_class, _T("Gaming/Toy"));
            return;
		}
		break;
	case 5:
        xscpy(major_class, _T("Peripheral"));
        {
		switch(minor & 48) {
		case 16:
			xscpy(minor_class, _T("Keyboard"));
			break;
		case 32:
			xscpy(minor_class, _T("Pointing device"));
			break;
		case 48:
			xscpy(minor_class, _T("Combo keyboard/pointing device"));
			break;
		}
		if((minor & 15) && (xslen(minor_class) > 0))
			xscat(minor_class, _T("/"));

		switch(minor & 15) {
		case 0:
			break;
		case 1:
			xscat(minor_class, _T("Joystick"));
			break;
		case 2:
			xscat(minor_class, _T("Gamepad"));
			break;
		case 3:
			xscat(minor_class, _T("Remote control"));
			break;
		case 4:
			xscat(minor_class, _T("Sensing device"));
			break;
		case 5:
			xscat(minor_class, _T("Digitizer tablet"));
			break;
		case 6:
			xscat(minor_class, _T("Card reader"));
			break;
		default:
			xscat(minor_class, _T("Reserved"));
			break;
		}
		if(is_null(minor_class))
		{
            xscat(minor_class, _T("Unknown"));
        }
        return;
	}
	case 6:
		xscpy(major_class, _T("Imaging"));
		if (minor & 4)
		{
			xscat(minor_class, _T("Display"));
			return;
		}
		if (minor & 8)
		{
			xscat(minor_class, _T("Camera"));
			return;
		}
		if (minor & 16)
		{
			xscat(minor_class, _T("Scanner"));
			return;
		}
		if (minor & 32)
		{
			xscat(minor_class, _T("Printer"));
			return;
		}
		break;
	case 7:
		xscpy(major_class, _T("Wearable"));
		switch(minor) {
		case 1:
			xscat(minor_class, _T("Wrist Watch"));
			return;
		case 2:
			xscat(minor_class, _T("Pager"));
			return;
		case 3:
			xscat(minor_class, _T("Jacket"));
			return;
		case 4:
			xscat(minor_class, _T("Helmet"));
			return;
		case 5:
			xscat(minor_class, _T("Glasses"));
			return;
		}
		break;
	case 8:
		xscpy(major_class, _T("Toy"));
		switch(minor) {
		case 1:
			xscat(minor_class, _T("Robot"));
			return;
		case 2:
			xscat(minor_class, _T("Vehicle"));
			return;
		case 3:
			xscat(minor_class, _T("Doll / Action Figure"));
			return;
		case 4:
			xscat(minor_class, _T("Controller"));
			return;
		case 5:
			xscat(minor_class, _T("Game"));
			return;
		}
		break;
	case 63:
		xscpy(major_class, _T("Uncategorised"));
		xscat(minor_class, _T(""));
		return;
	default:
		xscpy(major_class, _T("Uncategorised"));
		xscat(minor_class, _T(""));
		return;
	}
}

static bool_t _get_blut_device(const tchar_t *addr, SOCKADDR_BTH* paddr)
{
	HANDLE hsrv = NULL;
	WSAQUERYSET* pbuf = NULL;
	DWORD dw = 4096;
	BOOL done = FALSE;
	SOCKADDR_BTH *pa = NULL;
	int error = 0;
	tchar_t rmt_addr[RES_LEN] = { 0 };
	tchar_t cur_addr[RES_LEN] = { 0 };
	DWORD len_addr = 0;

	pbuf = (WSAQUERYSET*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dw);
	pbuf->dwSize = sizeof(WSAQUERYSET);
	pbuf->dwNameSpace = NS_BTH;
	pbuf->lpcsaBuffer = NULL;

	if (SOCKET_ERROR == WSALookupServiceBegin(pbuf, LUP_CONTAINERS, &hsrv))
	{
		return 0;
	}

	while (!done) 
	{
		if (SOCKET_ERROR == WSALookupServiceNext(hsrv, LUP_FLUSHCACHE | LUP_RETURN_ALL, &dw, pbuf))
		{
			error = WSAGetLastError();
			if (error == WSAEFAULT) 
			{
				pbuf = (WSAQUERYSET*)HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, pbuf, dw);
			}
			else if (error == WSA_E_NO_MORE)
			{
				done = FALSE;
				break;
			}
			else
			{
				done = FALSE;
				break;
			}
		}
		else
		{
			if (pbuf->lpcsaBuffer)
			{
				pa = (SOCKADDR_BTH*)(pbuf->lpcsaBuffer->RemoteAddr.lpSockaddr);
				len_addr = RES_LEN;
				WSAAddressToString((LPSOCKADDR)pa, sizeof(SOCKADDR_BTH), NULL, rmt_addr, &len_addr);
				if (rmt_addr[0] == _T('('))
					xsncpy(cur_addr, rmt_addr + 1, MAC_LEN);
				else
					xsncpy(cur_addr, rmt_addr, MAC_LEN);

				if (xscmp(addr, cur_addr) == 0)
				{
					CopyMemory((void*)paddr, (void*)pa, sizeof(SOCKADDR_BTH));
					done = TRUE;
					break;
				}
			}
		}
	}

	WSALookupServiceEnd(hsrv);

	HeapFree(GetProcessHeap(), 0, pbuf);

	return (done)? 1 : 0;
}

static BOOL _blut_auth_callback(LPVOID pvParam, PBLUETOOTH_DEVICE_INFO pDevice)
{
	tchar_t szKey[BLUETOOTH_MAX_PASSKEY_SIZE] = { 0 };
	
	xscpy(szKey, _T("123456"));

	return (ERROR_SUCCESS == BluetoothSendAuthenticationResponse((HANDLE)pvParam, pDevice, szKey))? TRUE : FALSE;
}

int _enum_blut_device(dev_blt_t* pdb, int max)
{
	HBLUETOOTH_RADIO_FIND hrf = NULL;
	HANDLE hbr = NULL;
	HBLUETOOTH_DEVICE_FIND hdf = NULL;
	BLUETOOTH_FIND_RADIO_PARAMS frp = { 0 };
	BLUETOOTH_RADIO_INFO ri = { 0 };
	BLUETOOTH_DEVICE_SEARCH_PARAMS dsp = { 0 };
	BLUETOOTH_DEVICE_INFO di = { 0 };
	BOOL b_radio = FALSE;
	BOOL b_device = FALSE;
	int i = 0;
	tchar_t sz_key[BLUETOOTH_MAX_PASSKEY_SIZE] = { 0 };
	int err = 0;
	GUID sp_guid = SerialPortServiceClass_UUID;
	GUID srv_guids[2];
	DWORD n = 2;

	srv_guids[0] = OBEXFileTransferServiceClass_UUID;
	srv_guids[1] = OBEXObjectPushServiceClass_UUID;

	frp.dwSize = sizeof(BLUETOOTH_FIND_RADIO_PARAMS);
	ri.dwSize = sizeof(BLUETOOTH_RADIO_INFO);
	dsp.dwSize = sizeof(BLUETOOTH_DEVICE_SEARCH_PARAMS);
	di.dwSize = sizeof(BLUETOOTH_DEVICE_INFO);

	hrf = BluetoothFindFirstRadio(&frp, &hbr);
	b_radio = (hrf != NULL) ? TRUE : FALSE;
	while (b_radio)
	{
		if (BluetoothGetRadioInfo(hbr, &ri) == ERROR_SUCCESS)
		{
			if (!BluetoothIsConnectable(hbr))
			{
				BluetoothEnableIncomingConnections(hbr, TRUE);
			}

			dsp.hRadio = hbr;
			dsp.fReturnAuthenticated = TRUE;
			dsp.fReturnConnected = FALSE;
			dsp.fReturnRemembered = TRUE;
			dsp.fReturnUnknown = TRUE;
			dsp.cTimeoutMultiplier = 30;

			hdf = BluetoothFindFirstDevice(&dsp, &di);
			b_device = (hdf != NULL)? TRUE : FALSE;
			while (b_device && i < max)
			{
				if (pdb)
				{
					if (!di.fAuthenticated)
					{
						if (ERROR_SUCCESS != BluetoothSetServiceState(hbr, &di, &sp_guid, BLUETOOTH_SERVICE_ENABLE))
						{
							err = WSAGetLastError();
						}

						if (ERROR_SUCCESS != BluetoothEnumerateInstalledServices(hbr, &di, &n, srv_guids))
						{
							err = WSAGetLastError();
						}

						if (ERROR_SUCCESS != BluetoothUpdateDeviceRecord(&di))
						{
							err = WSAGetLastError();
						}

						xscpy(sz_key, _T("123456"));
						if (ERROR_SUCCESS != BluetoothAuthenticateDevice(NULL, hbr, &di, sz_key, xslen(sz_key)))
						{
							err = WSAGetLastError();
						}
					}

					xscpy(pdb[i].name, di.szName);
					xsprintf(pdb[i].addr, _T("%02X:%02X:%02X:%02X:%02X:%02X"), (int)di.Address.rgBytes[5], (int)di.Address.rgBytes[4], (int)di.Address.rgBytes[3], (int)di.Address.rgBytes[2], (int)di.Address.rgBytes[1], (int)di.Address.rgBytes[0]);
				}

				i++;
				b_device = BluetoothFindNextDevice(hdf, &di);
			}

			BluetoothFindDeviceClose(hdf);
		}

		CloseHandle(hbr);
		hbr = NULL;
		b_radio = BluetoothFindNextRadio(hrf, &hbr);
	}

	BluetoothFindRadioClose(hrf);

	return i;
}

res_file_t _blut_open(const tchar_t* devaddr, int channel, dword_t fmode)
{
    SOCKET fd = 0;
	SOCKADDR_BTH loc_addr = { 0 };
	SOCKADDR_BTH rmt_addr = { 0 };
	int addr_len;
	socklen_t optlen;
	tchar_t sz_addr[RES_LEN] = {0};
	int err;
	u_long u_mode = 1;

	sz_addr[0] = _T('(');

	if (devaddr[0] == _T('('))
		xsncpy(sz_addr + 1, devaddr + 1, 17);
	else
		xsncpy(sz_addr + 1, devaddr, 17);

	sz_addr[18] = _T(')');

	addr_len = sizeof(rmt_addr);
	if (SOCKET_ERROR == WSAStringToAddress(sz_addr, AF_BTH, NULL, (LPSOCKADDR)&rmt_addr, &addr_len)) 
	{
		return INVALID_FILE;
	}

	fd = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);

	if (fd == INVALID_SOCKET) return INVALID_FILE;

	loc_addr.addressFamily = AF_BTH;
	loc_addr.btAddr = 0;
	loc_addr.serviceClassId = GUID_NULL;
	loc_addr.port = 0;

	if (bind(fd, (struct sockaddr *)&loc_addr, sizeof(loc_addr)) < 0) 
	{
		closesocket(fd);
		return INVALID_FILE;
	}

	optlen = sizeof(loc_addr);
	if (getsockname(fd, (struct sockaddr *)&loc_addr, &optlen) < 0)
	{
		closesocket(fd);
		return INVALID_FILE;
	}

	rmt_addr.serviceClassId = OBEXFileTransferServiceClass_UUID;
	rmt_addr.port = 0;

	if (connect(fd, (struct sockaddr *)&rmt_addr, sizeof(rmt_addr)) < 0) 
	{
		err = WSAGetLastError();
		closesocket(fd);
		return INVALID_FILE;
	}

    if(fmode & FILE_OPEN_OVERLAP)
    {
		ioctlsocket(fd, FIONBIO, &u_mode);
    }
	
	return (res_file_t)fd;
}

void _blut_close(res_file_t fh)
{
	closesocket((SOCKET)fh);
}

dword_t _blut_listen(res_file_t fd, async_t* pb)
{
	return 0;
}

bool_t _blut_read(res_file_t fd, void* buf, dword_t size, async_t* pb)
{
	return 0;
}

bool_t _blut_write(res_file_t fd, void* buf, dword_t size, async_t* pb)
{
	return 0;
}

bool_t _blut_flush(res_file_t fh)
{
	return 1;
}


#endif