/***********************************************************************
	Easily xdk v5.5

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

#include "xdkiml.h"

#ifdef XDK_SUPPORT_BLUT


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

int _enum_blut_device(dev_blt_t* pdb, int max)
{
	int dev_id = -1;
	struct hci_dev_info di = {0};
	uint8_t lap[3] = { 0x33, 0x8b, 0x9e };
	uint8_t cls[3] = {0};
	int i, j, n;
	int dev;
	inquiry_info *info = NULL;
	struct hci_conn_info_req *pci = NULL;
	uint16_t handle = 0;
	int cc = 0;

	dev_id = hci_get_route(NULL);

	if (dev_id < 0) return 0;

	if (hci_devinfo(dev_id, &di) < 0) return 0;

	n = hci_inquiry(dev_id, 8, 0, lap, &info, 0);
	if (n < 0) return 0;

	if(!pdb) 
	{
		bt_free(info);
		return n;
	}

	dev = hci_open_dev(dev_id);
	if (dev < 0) 
	{
		bt_free(info);
		return 0;
	}

	for (i = 0; i < n && i < max; i++) 
	{
		ba2str(&(info+i)->bdaddr, pdb[i].addr);

		pci = (struct hci_conn_info_req*)malloc(sizeof(struct hci_conn_info_req) + sizeof(struct hci_conn_info));
		
		bacpy(&(pci->bdaddr), &(info+i)->bdaddr);
		pci->type = ACL_LINK;

		if (ioctl(dev, HCIGETCONNINFO, (unsigned long) pci) < 0) 
		{
			handle = 0;
			cc = 1;
		} else 
		{
			handle = htobs(pci->conn_info->handle);
			cc = 0;
		}

		free(pci);

		if (cc) 
		{
			if (hci_create_connection(dev, &(info+i)->bdaddr, htobs(di.pkt_type & ACL_PTYPE_MASK), (info+i)->clock_offset | 0x8000, 0x01, &handle, 25000) < 0) 
			{
				handle = 0;
				cc = 0;
			}
		}

		if (handle > 0) 
		{
			if (hci_read_remote_name_with_clock_offset(dev, &(info+i)->bdaddr, (info+i)->pscan_rep_mode, (info+i)->clock_offset | 0x8000, META_LEN, pdb->name, 100000) < 0) 
			{
				xscpy(pdb[i].name, _T("N/A"));
			} else 
			{
				for (j = 0; j < META_LEN && pdb[i].name[j]; j++) 
				{
					if ((unsigned char) pdb[i].name[j] < 32 || pdb[i].name[j] == 127)
						pdb[i].name[j] = _T('.');
				}
			}
		}

		if (handle > 0) 
		{
			usleep(10000);
			hci_disconnect(dev, handle, HCI_OE_USER_ENDED_CONNECTION, 10000);
		}

		memcpy(cls, (info+i)->dev_class, 3);
		_blt_device_class(cls, pdb[i].major_class, pdb[i].minor_class);
	}

	bt_free(info);

	hci_close_dev(dev);

	return (i + 1);
}

res_file_t _blut_open(const tchar_t* devaddr, int channel, dword_t fmode)
{
    int fd = 0;
    struct sockaddr_l2 addr;
	socklen_t optlen;
	tchar_t devname[META_LEN] = {0};

	fd = socket(PF_BLUETOOTH, SOCK_RAW, BTPROTO_HCI);

	if (fd < 0) return INVALID_FILE;

	memset(&addr, 0, sizeof(addr));
	addr.l2_family = AF_BLUETOOTH;
	bacpy(&addr.l2_bdaddr, BDADDR_ANY);

	if (bind(fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) 
	{
		close(fd);
		return INVALID_FILE;
	}

	memset(&addr, 0, sizeof(addr));
	addr.l2_family = AF_BLUETOOTH;
	str2ba(devaddr, &addr.l2_bdaddr);

	if (connect(fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) 
	{
		close(fd);
		return INVALID_FILE;
	}

	memset(&addr, 0, sizeof(addr));
	optlen = sizeof(addr);

	if (getsockname(fd, (struct sockaddr *) &addr, &optlen) < 0) 
	{
		close(fd);
		return INVALID_FILE;
	}

	ba2str(&addr.l2_bdaddr, devname);

    if(fmode & FILE_OPEN_OVERLAP)
    {
        ioctl(fd, FIONBIO, 1);
    }
	
	return (res_file_t)fd;
}

void _blut_close(res_file_t fh)
{
    close(fh);
}

dword_t _blut_listen(res_file_t fd, async_t* pb)
{
    return _comm_listen(fd, pb);
}

bool_t _blut_read(res_file_t fd, void* buf, dword_t size, async_t* pb)
{
    return _socket_recv(fd, buf, size, pb);
}

bool_t _blut_write(res_file_t fd, void* buf, dword_t size, async_t* pb)
{
    return _socket_send(fd, buf, size, pb);
}

bool_t _blut_flush(res_file_t fh)
{
    return (tcflush(fh, TCOFLUSH) < 0)? 0 : 1;
}


#endif