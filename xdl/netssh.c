/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc ssh document

	@module	netssh.c | implement file

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

#include "netssh.h"
#include "nettcp.h"
#include "bioinf.h"
#include "stream.h"

#include "xdlimp.h"
#include "xdlstd.h"
#include "xdlinit.h"

#if defined(XDK_SUPPORT_SOCK)

static unsigned char dhm_G[] = {0x02};

static unsigned char group1_dhm_P[] = {
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC9, 0x0F, 0xDA, 0xA2,
	0x21, 0x68, 0xC2, 0x34, 0xC4, 0xC6, 0x62, 0x8B, 0x80, 0xDC, 0x1C, 0xD1,
	0x29, 0x02, 0x4E, 0x08, 0x8A, 0x67, 0xCC, 0x74, 0x02, 0x0B, 0xBE, 0xA6,
	0x3B, 0x13, 0x9B, 0x22, 0x51, 0x4A, 0x08, 0x79, 0x8E, 0x34, 0x04, 0xDD,
	0xEF, 0x95, 0x19, 0xB3, 0xCD, 0x3A, 0x43, 0x1B, 0x30, 0x2B, 0x0A, 0x6D,
	0xF2, 0x5F, 0x14, 0x37, 0x4F, 0xE1, 0x35, 0x6D, 0x6D, 0x51, 0xC2, 0x45,
	0xE4, 0x85, 0xB5, 0x76, 0x62, 0x5E, 0x7E, 0xC6, 0xF4, 0x4C, 0x42, 0xE9,
	0xA6, 0x37, 0xED, 0x6B, 0x0B, 0xFF, 0x5C, 0xB6, 0xF4, 0x06, 0xB7, 0xED,
	0xEE, 0x38, 0x6B, 0xFB, 0x5A, 0x89, 0x9F, 0xA5, 0xAE, 0x9F, 0x24, 0x11,
	0x7C, 0x4B, 0x1F, 0xE6, 0x49, 0x28, 0x66, 0x51, 0xEC, 0xE6, 0x53, 0x81,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
#define GROUP1_DHM_P_LEN 128	/* Size in bytes of the p number for group1 */


static unsigned char group14_dhm_P[] = {
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC9, 0x0F, 0xDA, 0xA2,
	0x21, 0x68, 0xC2, 0x34, 0xC4, 0xC6, 0x62, 0x8B, 0x80, 0xDC, 0x1C, 0xD1,
	0x29, 0x02, 0x4E, 0x08, 0x8A, 0x67, 0xCC, 0x74, 0x02, 0x0B, 0xBE, 0xA6,
	0x3B, 0x13, 0x9B, 0x22, 0x51, 0x4A, 0x08, 0x79, 0x8E, 0x34, 0x04, 0xDD,
	0xEF, 0x95, 0x19, 0xB3, 0xCD, 0x3A, 0x43, 0x1B, 0x30, 0x2B, 0x0A, 0x6D,
	0xF2, 0x5F, 0x14, 0x37, 0x4F, 0xE1, 0x35, 0x6D, 0x6D, 0x51, 0xC2, 0x45,
	0xE4, 0x85, 0xB5, 0x76, 0x62, 0x5E, 0x7E, 0xC6, 0xF4, 0x4C, 0x42, 0xE9,
	0xA6, 0x37, 0xED, 0x6B, 0x0B, 0xFF, 0x5C, 0xB6, 0xF4, 0x06, 0xB7, 0xED,
	0xEE, 0x38, 0x6B, 0xFB, 0x5A, 0x89, 0x9F, 0xA5, 0xAE, 0x9F, 0x24, 0x11,
	0x7C, 0x4B, 0x1F, 0xE6, 0x49, 0x28, 0x66, 0x51, 0xEC, 0xE4, 0x5B, 0x3D,
	0xC2, 0x00, 0x7C, 0xB8, 0xA1, 0x63, 0xBF, 0x05, 0x98, 0xDA, 0x48, 0x36,
	0x1C, 0x55, 0xD3, 0x9A, 0x69, 0x16, 0x3F, 0xA8, 0xFD, 0x24, 0xCF, 0x5F,
	0x83, 0x65, 0x5D, 0x23, 0xDC, 0xA3, 0xAD, 0x96, 0x1C, 0x62, 0xF3, 0x56,
	0x20, 0x85, 0x52, 0xBB, 0x9E, 0xD5, 0x29, 0x07, 0x70, 0x96, 0x96, 0x6D,
	0x67, 0x0C, 0x35, 0x4E, 0x4A, 0xBC, 0x98, 0x04, 0xF1, 0x74, 0x6C, 0x08,
	0xCA, 0x18, 0x21, 0x7C, 0x32, 0x90, 0x5E, 0x46, 0x2E, 0x36, 0xCE, 0x3B,
	0xE3, 0x9E, 0x77, 0x2C, 0x18, 0x0E, 0x86, 0x03, 0x9B, 0x27, 0x83, 0xA2,
	0xEC, 0x07, 0xA2, 0x8F, 0xB5, 0xC5, 0x5D, 0xF0, 0x6F, 0x4C, 0x52, 0xC9,
	0xDE, 0x2B, 0xCB, 0xF6, 0x95, 0x58, 0x17, 0x18, 0x39, 0x95, 0x49, 0x7C,
	0xEA, 0x95, 0x6A, 0xE5, 0x15, 0xD2, 0x26, 0x18, 0x98, 0xFA, 0x05, 0x10,
	0x15, 0x72, 0x8E, 0x5A, 0x8A, 0xAC, 0xAA, 0x68, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF };
#define GROUP14_DHM_P_LEN 256 /* Size in bytes of the p number for group 14 */

static unsigned char group16_dhm_P[] = {
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC9, 0x0F, 0xDA, 0xA2,
	0x21, 0x68, 0xC2, 0x34, 0xC4, 0xC6, 0x62, 0x8B, 0x80, 0xDC, 0x1C, 0xD1,
	0x29, 0x02, 0x4E, 0x08, 0x8A, 0x67, 0xCC, 0x74, 0x02, 0x0B, 0xBE, 0xA6,
	0x3B, 0x13, 0x9B, 0x22, 0x51, 0x4A, 0x08, 0x79, 0x8E, 0x34, 0x04, 0xDD,
	0xEF, 0x95, 0x19, 0xB3, 0xCD, 0x3A, 0x43, 0x1B, 0x30, 0x2B, 0x0A, 0x6D,
	0xF2, 0x5F, 0x14, 0x37, 0x4F, 0xE1, 0x35, 0x6D, 0x6D, 0x51, 0xC2, 0x45,
	0xE4, 0x85, 0xB5, 0x76, 0x62, 0x5E, 0x7E, 0xC6, 0xF4, 0x4C, 0x42, 0xE9,
	0xA6, 0x37, 0xED, 0x6B, 0x0B, 0xFF, 0x5C, 0xB6, 0xF4, 0x06, 0xB7, 0xED,
	0xEE, 0x38, 0x6B, 0xFB, 0x5A, 0x89, 0x9F, 0xA5, 0xAE, 0x9F, 0x24, 0x11,
	0x7C, 0x4B, 0x1F, 0xE6, 0x49, 0x28, 0x66, 0x51, 0xEC, 0xE4, 0x5B, 0x3D,
	0xC2, 0x00, 0x7C, 0xB8, 0xA1, 0x63, 0xBF, 0x05, 0x98, 0xDA, 0x48, 0x36,
	0x1C, 0x55, 0xD3, 0x9A, 0x69, 0x16, 0x3F, 0xA8, 0xFD, 0x24, 0xCF, 0x5F,
	0x83, 0x65, 0x5D, 0x23, 0xDC, 0xA3, 0xAD, 0x96, 0x1C, 0x62, 0xF3, 0x56,
	0x20, 0x85, 0x52, 0xBB, 0x9E, 0xD5, 0x29, 0x07, 0x70, 0x96, 0x96, 0x6D,
	0x67, 0x0C, 0x35, 0x4E, 0x4A, 0xBC, 0x98, 0x04, 0xF1, 0x74, 0x6C, 0x08,
	0xCA, 0x18, 0x21, 0x7C, 0x32, 0x90, 0x5E, 0x46, 0x2E, 0x36, 0xCE, 0x3B,
	0xE3, 0x9E, 0x77, 0x2C, 0x18, 0x0E, 0x86, 0x03, 0x9B, 0x27, 0x83, 0xA2,
	0xEC, 0x07, 0xA2, 0x8F, 0xB5, 0xC5, 0x5D, 0xF0, 0x6F, 0x4C, 0x52, 0xC9,
	0xDE, 0x2B, 0xCB, 0xF6, 0x95, 0x58, 0x17, 0x18, 0x39, 0x95, 0x49, 0x7C,
	0xEA, 0x95, 0x6A, 0xE5, 0x15, 0xD2, 0x26, 0x18, 0x98, 0xFA, 0x05, 0x10,
	0x15, 0x72, 0x8E, 0x5A, 0x8A, 0xAA, 0xC4, 0x2D, 0xAD, 0x33, 0x17, 0x0D,
	0x04, 0x50, 0x7A, 0x33, 0xA8, 0x55, 0x21, 0xAB, 0xDF, 0x1C, 0xBA, 0x64,
	0xEC, 0xFB, 0x85, 0x04, 0x58, 0xDB, 0xEF, 0x0A, 0x8A, 0xEA, 0x71, 0x57,
	0x5D, 0x06, 0x0C, 0x7D, 0xB3, 0x97, 0x0F, 0x85, 0xA6, 0xE1, 0xE4, 0xC7,
	0xAB, 0xF5, 0xAE, 0x8C, 0xDB, 0x09, 0x33, 0xD7, 0x1E, 0x8C, 0x94, 0xE0,
	0x4A, 0x25, 0x61, 0x9D, 0xCE, 0xE3, 0xD2, 0x26, 0x1A, 0xD2, 0xEE, 0x6B,
	0xF1, 0x2F, 0xFA, 0x06, 0xD9, 0x8A, 0x08, 0x64, 0xD8, 0x76, 0x02, 0x73,
	0x3E, 0xC8, 0x6A, 0x64, 0x52, 0x1F, 0x2B, 0x18, 0x17, 0x7B, 0x20, 0x0C,
	0xBB, 0xE1, 0x17, 0x57, 0x7A, 0x61, 0x5D, 0x6C, 0x77, 0x09, 0x88, 0xC0,
	0xBA, 0xD9, 0x46, 0xE2, 0x08, 0xE2, 0x4F, 0xA0, 0x74, 0xE5, 0xAB, 0x31,
	0x43, 0xDB, 0x5B, 0xFC, 0xE0, 0xFD, 0x10, 0x8E, 0x4B, 0x82, 0xD1, 0x20,
	0xA9, 0x21, 0x08, 0x01, 0x1A, 0x72, 0x3C, 0x12, 0xA7, 0x87, 0xE6, 0xD7,
	0x88, 0x71, 0x9A, 0x10, 0xBD, 0xBA, 0x5B, 0x26, 0x99, 0xC3, 0x27, 0x18,
	0x6A, 0xF4, 0xE2, 0x3C, 0x1A, 0x94, 0x68, 0x34, 0xB6, 0x15, 0x0B, 0xDA,
	0x25, 0x83, 0xE9, 0xCA, 0x2A, 0xD4, 0x4C, 0xE8, 0xDB, 0xBB, 0xC2, 0xDB,
	0x04, 0xDE, 0x8E, 0xF9, 0x2E, 0x8E, 0xFC, 0x14, 0x1F, 0xBE, 0xCA, 0xA6,
	0x28, 0x7C, 0x59, 0x47, 0x4E, 0x6B, 0xC0, 0x5D, 0x99, 0xB2, 0x96, 0x4F,
	0xA0, 0x90, 0xC3, 0xA2, 0x23, 0x3B, 0xA1, 0x86, 0x51, 0x5B, 0xE7, 0xED,
	0x1F, 0x61, 0x29, 0x70, 0xCE, 0xE2, 0xD7, 0xAF, 0xB8, 0x1B, 0xDD, 0x76,
	0x21, 0x70, 0x48, 0x1C, 0xD0, 0x06, 0x91, 0x27, 0xD5, 0xB0, 0x5A, 0xA9,
	0x93, 0xB4, 0xEA, 0x98, 0x8D, 0x8F, 0xDD, 0xC1, 0x86, 0xFF, 0xB7, 0xDC,
	0x90, 0xA6, 0xC0, 0x8F, 0x4D, 0xF4, 0x35, 0xC9, 0x34, 0x06, 0x31, 0x99,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
#define GROUP16_DHM_P_LEN 512 /* Size in bytes of the p number for group 16 */

static unsigned char group18_dhm_P[] = {
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC9, 0x0F, 0xDA, 0xA2,
	0x21, 0x68, 0xC2, 0x34, 0xC4, 0xC6, 0x62, 0x8B, 0x80, 0xDC, 0x1C, 0xD1,
	0x29, 0x02, 0x4E, 0x08, 0x8A, 0x67, 0xCC, 0x74, 0x02, 0x0B, 0xBE, 0xA6,
	0x3B, 0x13, 0x9B, 0x22, 0x51, 0x4A, 0x08, 0x79, 0x8E, 0x34, 0x04, 0xDD,
	0xEF, 0x95, 0x19, 0xB3, 0xCD, 0x3A, 0x43, 0x1B, 0x30, 0x2B, 0x0A, 0x6D,
	0xF2, 0x5F, 0x14, 0x37, 0x4F, 0xE1, 0x35, 0x6D, 0x6D, 0x51, 0xC2, 0x45,
	0xE4, 0x85, 0xB5, 0x76, 0x62, 0x5E, 0x7E, 0xC6, 0xF4, 0x4C, 0x42, 0xE9,
	0xA6, 0x37, 0xED, 0x6B, 0x0B, 0xFF, 0x5C, 0xB6, 0xF4, 0x06, 0xB7, 0xED,
	0xEE, 0x38, 0x6B, 0xFB, 0x5A, 0x89, 0x9F, 0xA5, 0xAE, 0x9F, 0x24, 0x11,
	0x7C, 0x4B, 0x1F, 0xE6, 0x49, 0x28, 0x66, 0x51, 0xEC, 0xE4, 0x5B, 0x3D,
	0xC2, 0x00, 0x7C, 0xB8, 0xA1, 0x63, 0xBF, 0x05, 0x98, 0xDA, 0x48, 0x36,
	0x1C, 0x55, 0xD3, 0x9A, 0x69, 0x16, 0x3F, 0xA8, 0xFD, 0x24, 0xCF, 0x5F,
	0x83, 0x65, 0x5D, 0x23, 0xDC, 0xA3, 0xAD, 0x96, 0x1C, 0x62, 0xF3, 0x56,
	0x20, 0x85, 0x52, 0xBB, 0x9E, 0xD5, 0x29, 0x07, 0x70, 0x96, 0x96, 0x6D,
	0x67, 0x0C, 0x35, 0x4E, 0x4A, 0xBC, 0x98, 0x04, 0xF1, 0x74, 0x6C, 0x08,
	0xCA, 0x18, 0x21, 0x7C, 0x32, 0x90, 0x5E, 0x46, 0x2E, 0x36, 0xCE, 0x3B,
	0xE3, 0x9E, 0x77, 0x2C, 0x18, 0x0E, 0x86, 0x03, 0x9B, 0x27, 0x83, 0xA2,
	0xEC, 0x07, 0xA2, 0x8F, 0xB5, 0xC5, 0x5D, 0xF0, 0x6F, 0x4C, 0x52, 0xC9,
	0xDE, 0x2B, 0xCB, 0xF6, 0x95, 0x58, 0x17, 0x18, 0x39, 0x95, 0x49, 0x7C,
	0xEA, 0x95, 0x6A, 0xE5, 0x15, 0xD2, 0x26, 0x18, 0x98, 0xFA, 0x05, 0x10,
	0x15, 0x72, 0x8E, 0x5A, 0x8A, 0xAA, 0xC4, 0x2D, 0xAD, 0x33, 0x17, 0x0D,
	0x04, 0x50, 0x7A, 0x33, 0xA8, 0x55, 0x21, 0xAB, 0xDF, 0x1C, 0xBA, 0x64,
	0xEC, 0xFB, 0x85, 0x04, 0x58, 0xDB, 0xEF, 0x0A, 0x8A, 0xEA, 0x71, 0x57,
	0x5D, 0x06, 0x0C, 0x7D, 0xB3, 0x97, 0x0F, 0x85, 0xA6, 0xE1, 0xE4, 0xC7,
	0xAB, 0xF5, 0xAE, 0x8C, 0xDB, 0x09, 0x33, 0xD7, 0x1E, 0x8C, 0x94, 0xE0,
	0x4A, 0x25, 0x61, 0x9D, 0xCE, 0xE3, 0xD2, 0x26, 0x1A, 0xD2, 0xEE, 0x6B,
	0xF1, 0x2F, 0xFA, 0x06, 0xD9, 0x8A, 0x08, 0x64, 0xD8, 0x76, 0x02, 0x73,
	0x3E, 0xC8, 0x6A, 0x64, 0x52, 0x1F, 0x2B, 0x18, 0x17, 0x7B, 0x20, 0x0C,
	0xBB, 0xE1, 0x17, 0x57, 0x7A, 0x61, 0x5D, 0x6C, 0x77, 0x09, 0x88, 0xC0,
	0xBA, 0xD9, 0x46, 0xE2, 0x08, 0xE2, 0x4F, 0xA0, 0x74, 0xE5, 0xAB, 0x31,
	0x43, 0xDB, 0x5B, 0xFC, 0xE0, 0xFD, 0x10, 0x8E, 0x4B, 0x82, 0xD1, 0x20,
	0xA9, 0x21, 0x08, 0x01, 0x1A, 0x72, 0x3C, 0x12, 0xA7, 0x87, 0xE6, 0xD7,
	0x88, 0x71, 0x9A, 0x10, 0xBD, 0xBA, 0x5B, 0x26, 0x99, 0xC3, 0x27, 0x18,
	0x6A, 0xF4, 0xE2, 0x3C, 0x1A, 0x94, 0x68, 0x34, 0xB6, 0x15, 0x0B, 0xDA,
	0x25, 0x83, 0xE9, 0xCA, 0x2A, 0xD4, 0x4C, 0xE8, 0xDB, 0xBB, 0xC2, 0xDB,
	0x04, 0xDE, 0x8E, 0xF9, 0x2E, 0x8E, 0xFC, 0x14, 0x1F, 0xBE, 0xCA, 0xA6,
	0x28, 0x7C, 0x59, 0x47, 0x4E, 0x6B, 0xC0, 0x5D, 0x99, 0xB2, 0x96, 0x4F,
	0xA0, 0x90, 0xC3, 0xA2, 0x23, 0x3B, 0xA1, 0x86, 0x51, 0x5B, 0xE7, 0xED,
	0x1F, 0x61, 0x29, 0x70, 0xCE, 0xE2, 0xD7, 0xAF, 0xB8, 0x1B, 0xDD, 0x76,
	0x21, 0x70, 0x48, 0x1C, 0xD0, 0x06, 0x91, 0x27, 0xD5, 0xB0, 0x5A, 0xA9,
	0x93, 0xB4, 0xEA, 0x98, 0x8D, 0x8F, 0xDD, 0xC1, 0x86, 0xFF, 0xB7, 0xDC,
	0x90, 0xA6, 0xC0, 0x8F, 0x4D, 0xF4, 0x35, 0xC9, 0x34, 0x02, 0x84, 0x92,
	0x36, 0xC3, 0xFA, 0xB4, 0xD2, 0x7C, 0x70, 0x26, 0xC1, 0xD4, 0xDC, 0xB2,
	0x60, 0x26, 0x46, 0xDE, 0xC9, 0x75, 0x1E, 0x76, 0x3D, 0xBA, 0x37, 0xBD,
	0xF8, 0xFF, 0x94, 0x06, 0xAD, 0x9E, 0x53, 0x0E, 0xE5, 0xDB, 0x38, 0x2F,
	0x41, 0x30, 0x01, 0xAE, 0xB0, 0x6A, 0x53, 0xED, 0x90, 0x27, 0xD8, 0x31,
	0x17, 0x97, 0x27, 0xB0, 0x86, 0x5A, 0x89, 0x18, 0xDA, 0x3E, 0xDB, 0xEB,
	0xCF, 0x9B, 0x14, 0xED, 0x44, 0xCE, 0x6C, 0xBA, 0xCE, 0xD4, 0xBB, 0x1B,
	0xDB, 0x7F, 0x14, 0x47, 0xE6, 0xCC, 0x25, 0x4B, 0x33, 0x20, 0x51, 0x51,
	0x2B, 0xD7, 0xAF, 0x42, 0x6F, 0xB8, 0xF4, 0x01, 0x37, 0x8C, 0xD2, 0xBF,
	0x59, 0x83, 0xCA, 0x01, 0xC6, 0x4B, 0x92, 0xEC, 0xF0, 0x32, 0xEA, 0x15,
	0xD1, 0x72, 0x1D, 0x03, 0xF4, 0x82, 0xD7, 0xCE, 0x6E, 0x74, 0xFE, 0xF6,
	0xD5, 0x5E, 0x70, 0x2F, 0x46, 0x98, 0x0C, 0x82, 0xB5, 0xA8, 0x40, 0x31,
	0x90, 0x0B, 0x1C, 0x9E, 0x59, 0xE7, 0xC9, 0x7F, 0xBE, 0xC7, 0xE8, 0xF3,
	0x23, 0xA9, 0x7A, 0x7E, 0x36, 0xCC, 0x88, 0xBE, 0x0F, 0x1D, 0x45, 0xB7,
	0xFF, 0x58, 0x5A, 0xC5, 0x4B, 0xD4, 0x07, 0xB2, 0x2B, 0x41, 0x54, 0xAA,
	0xCC, 0x8F, 0x6D, 0x7E, 0xBF, 0x48, 0xE1, 0xD8, 0x14, 0xCC, 0x5E, 0xD2,
	0x0F, 0x80, 0x37, 0xE0, 0xA7, 0x97, 0x15, 0xEE, 0xF2, 0x9B, 0xE3, 0x28,
	0x06, 0xA1, 0xD5, 0x8B, 0xB7, 0xC5, 0xDA, 0x76, 0xF5, 0x50, 0xAA, 0x3D,
	0x8A, 0x1F, 0xBF, 0xF0, 0xEB, 0x19, 0xCC, 0xB1, 0xA3, 0x13, 0xD5, 0x5C,
	0xDA, 0x56, 0xC9, 0xEC, 0x2E, 0xF2, 0x96, 0x32, 0x38, 0x7F, 0xE8, 0xD7,
	0x6E, 0x3C, 0x04, 0x68, 0x04, 0x3E, 0x8F, 0x66, 0x3F, 0x48, 0x60, 0xEE,
	0x12, 0xBF, 0x2D, 0x5B, 0x0B, 0x74, 0x74, 0xD6, 0xE6, 0x94, 0xF9, 0x1E,
	0x6D, 0xBE, 0x11, 0x59, 0x74, 0xA3, 0x92, 0x6F, 0x12, 0xFE, 0xE5, 0xE4,
	0x38, 0x77, 0x7C, 0xB6, 0xA9, 0x32, 0xDF, 0x8C, 0xD8, 0xBE, 0xC4, 0xD0,
	0x73, 0xB9, 0x31, 0xBA, 0x3B, 0xC8, 0x32, 0xB6, 0x8D, 0x9D, 0xD3, 0x00,
	0x74, 0x1F, 0xA7, 0xBF, 0x8A, 0xFC, 0x47, 0xED, 0x25, 0x76, 0xF6, 0x93,
	0x6B, 0xA4, 0x24, 0x66, 0x3A, 0xAB, 0x63, 0x9C, 0x5A, 0xE4, 0xF5, 0x68,
	0x34, 0x23, 0xB4, 0x74, 0x2B, 0xF1, 0xC9, 0x78, 0x23, 0x8F, 0x16, 0xCB,
	0xE3, 0x9D, 0x65, 0x2D, 0xE3, 0xFD, 0xB8, 0xBE, 0xFC, 0x84, 0x8A, 0xD9,
	0x22, 0x22, 0x2E, 0x04, 0xA4, 0x03, 0x7C, 0x07, 0x13, 0xEB, 0x57, 0xA8,
	0x1A, 0x23, 0xF0, 0xC7, 0x34, 0x73, 0xFC, 0x64, 0x6C, 0xEA, 0x30, 0x6B,
	0x4B, 0xCB, 0xC8, 0x86, 0x2F, 0x83, 0x85, 0xDD, 0xFA, 0x9D, 0x4B, 0x7F,
	0xA2, 0xC0, 0x87, 0xE8, 0x79, 0x68, 0x33, 0x03, 0xED, 0x5B, 0xDD, 0x3A,
	0x06, 0x2B, 0x3C, 0xF5, 0xB3, 0xA2, 0x78, 0xA6, 0x6D, 0x2A, 0x13, 0xF8,
	0x3F, 0x44, 0xF8, 0x2D, 0xDF, 0x31, 0x0E, 0xE0, 0x74, 0xAB, 0x6A, 0x36,
	0x45, 0x97, 0xE8, 0x99, 0xA0, 0x25, 0x5D, 0xC1, 0x64, 0xF3, 0x1C, 0xC5,
	0x08, 0x46, 0x85, 0x1D, 0xF9, 0xAB, 0x48, 0x19, 0x5D, 0xED, 0x7E, 0xA1,
	0xB1, 0xD5, 0x10, 0xBD, 0x7E, 0xE7, 0x4D, 0x73, 0xFA, 0xF3, 0x6B, 0xC3,
	0x1E, 0xCF, 0xA2, 0x68, 0x35, 0x90, 0x46, 0xF4, 0xEB, 0x87, 0x9F, 0x92,
	0x40, 0x09, 0x43, 0x8B, 0x48, 0x1C, 0x6C, 0xD7, 0x88, 0x9A, 0x00, 0x2E,
	0xD5, 0xEE, 0x38, 0x2B, 0xC9, 0x19, 0x0D, 0xA6, 0xFC, 0x02, 0x6E, 0x47,
	0x95, 0x58, 0xE4, 0x47, 0x56, 0x77, 0xE9, 0xAA, 0x9E, 0x30, 0x50, 0xE2,
	0x76, 0x56, 0x94, 0xDF, 0xC8, 0x1F, 0x56, 0xE8, 0x80, 0xB9, 0x6E, 0x71,
	0x60, 0xC9, 0x80, 0xDD, 0x98, 0xED, 0xD3, 0xDF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF };
#define GROUP18_DHM_P_LEN 1024 /* Size in bytes of the p number for group 18 */


#define SSH_KEXDH_GROUP18_SHA512	"diffie-hellman-group18-sha512"
#define SSH_KEXDH_GROUP16_SHA512	"diffie-hellman-group16-sha512"
#define SSH_KEXDH_GROUP14_SHA1		"diffie-hellman-group14-sha1"
#define SSH_KEXDH_GROUP1_SHA1		"diffie-hellman-group1-sha1"

#define SSH_KEYSECRET_DSS		"ssh-dss"
#define SSH_KEYSECRET_RSA		"ssh-rsa"
#define SSH_KEYSECRET_ECDSA		"ssh-ecdsa"
#define SSH_KEYSECRET_ED25519	"ssh-ed25519"
#define SSH_KEYSECRET_RSASHA2_256	"rsa-sha2-256"
#define SSH_KEYSECRET_RSASHA2_512	"rsa-sha2-512"


#define SSH_CIPH_BLOWFISH_CBC	"blowfish-cbc"
#define SSH_CIPH_AES128_CTR		"aes128-ctr"
#define SSH_CIPH_AES128_CBC		"aes128-cbc"
#define SSH_CIPH_AES192_CTR		"aes192-ctr"
#define SSH_CIPH_AES192_CBC		"aes192-cbc"
#define SSH_CIPH_AES256_CTR		"aes256-ctr"
#define SSH_CIPH_AES256_CBC		"aes256-cbc"
#define SSH_CIPH_3DES_CBC		"3des-cbc"
#define SSH_CIPH_ARC4			"arcfour"
#define SSH_CIPH_ARC4_128		"arcfour128"
#define SSH_CIPH_ARC4_256		"arcfour256"

#define SSH_CIPH_BLOWFISH_KEY_SIZE		128
#define SSH_CIPH_BLOWFISH_BLK_SIZE		8
#define SSH_CIPH_AES128_CTR_KEY_SIZ		128
#define SSH_CIPH_AES128_CTR_BLK_SIZ		16
#define SSH_CIPH_AES128_CBC_KEY_SIZE	128
#define SSH_CIPH_AES128_CBC_BLK_SIZE	16
#define SSH_CIPH_AES192_CTR_KEY_SIZE	192
#define SSH_CIPH_AES192_CTR_BLK_SIZE	16
#define SSH_CIPH_AES192_CBC_KEY_SIZE	192
#define SSH_CIPH_AES192_CBC_BLK_SIZE	16
#define SSH_CIPH_AES256_CTR_KEY_SIZE	256
#define SSH_CIPH_AES256_CTR_BLK_SIZE	16
#define SSH_CIPH_AES256_CBC_KEY_SIZE	256
#define SSH_CIPH_AES256_CBC_BLK_SIZE	16
#define SSH_CIPH_3DES_CBC_KEY_SIZE		192
#define SSH_CIPH_3DES_CBC_BLK_SIZE		8
#define SSH_CIPH_ARC4_KEY_SIZE			64
#define SSH_CIPH_ARC4_128_KEY_SIZE		128
#define SSH_CIPH_ARC4_256_KEY_SIZE		256

#define SSH_HMAC_SHA1		"hmac-sha1"
#define SSH_HMAC_SHA2_256	"hmac-sha2-256"
#define SSH_HMAC_SHA2_512	"hmac-sha2-512"
#define SSH_HMAC_MD5		"hmac-md5"
#define SSH_AEAD_POLY305	"aead-poly1305"

#define SSH_HMAC_SHA1_SIZE		20
#define SSH_HMAC_SHA2_256_SIZE	32
#define SSH_HMAC_SHA2_512_SIZE	64
#define SSH_HMAC_MD5_SIZE		16
#define SSH_AEAD_POLY305_SIZE	16

#define SSH_ZIP_ZLIB	"zlib"
#define SSH_ZIP_NONE	"none"

#ifdef SSH_SUPPORT_CURVE25519
#define CURVE25519 "curve25519-sha256,"
#else
#define CURVE25519 ""
#endif

#ifdef SSH_SUPPORT_ECDH
#define ECDH "ecdh-sha2-nistp256,ecdh-sha2-nistp384,ecdh-sha2-nistp521,"
#else
#define ECDH ""
#endif

#ifdef SSH_SUPPORT_DSA
#define DSA	",ssh-dss"
#else
#define DSA	""
#endif

#define SSH_ALGO_KEXDH	CURVE25519 ECDH "diffie-hellman-group14-sha1,diffie-hellman-group1-sha1"

#define SSH_ALGO_KEYSECRET	"ssh-rsa" DSA

#define SSH_ALGO_CIPH	"arcfour128,arcfour256,arcfour,aes256-cbc,aes192-cbc,aes128-cbc,blowfish-cbc,3des-cbc"

#define SSH_ALGO_HMAC	"hmac-sha2-256,hmac-sha2-512,hmac-sha1"

#define SSH_ALGO_COMPRESS	"none,zlib"

#define SSH_ALGO_EXTENSION_CLIENT ",ext-info-c"

#define SSH_ALGO_EXTENSION_SERVER ",ext-info-s"

#define SSH_ALGO_NAMETABLE_SIZE	10
static const schar_t* ssh_algo_server[SSH_ALGO_NAMETABLE_SIZE] = {
	SSH_ALGO_KEXDH SSH_ALGO_EXTENSION_SERVER, //kex algos
	SSH_ALGO_KEYSECRET, //server key secret algo
	SSH_ALGO_CIPH, //encryption client->server
	SSH_ALGO_CIPH, //encryption server->client
	SSH_ALGO_HMAC, //mac algo client->server
	SSH_ALGO_HMAC, //mac algo server->client
	SSH_ALGO_COMPRESS, //compression algo client->server
	SSH_ALGO_COMPRESS, //compression algo server->client
	"", //languages client->server
	"" //languages server->client
};

static const schar_t* ssh_algo_client[SSH_ALGO_NAMETABLE_SIZE] = {
	SSH_ALGO_KEXDH SSH_ALGO_EXTENSION_CLIENT, //kex algos
	SSH_ALGO_KEYSECRET, //server key secret algo
	SSH_ALGO_CIPH, //encryption client->server
	SSH_ALGO_CIPH, //encryption server->client
	SSH_ALGO_HMAC, //mac algo client->server
	SSH_ALGO_HMAC, //mac algo server->client
	SSH_ALGO_COMPRESS, //compression algo client->server
	SSH_ALGO_COMPRESS, //compression algo server->client
	"", //languages client->server
	"" //languages server->client
};

#define SSH_METHOD_INDEX_KEXDH			0
#define SSH_METHOD_INDEX_KEYSECRET		1
#define SSH_METHOD_INDEX_CIPHCLIENT		2
#define SSH_METHOD_INDEX_CIPHSERVER		3
#define SSH_METHOD_INDEX_HMACCLIENT		4
#define SSH_METHOD_INDEX_HMACSERVER		5
#define SSH_METHOD_INDEX_COMPRESSCLIENT	6
#define SSH_METHOD_INDEX_COMPRESSSERVER	7
#define SSH_METHOD_INDEX_LANGCLIENT		8
#define SSH_METHOD_INDEX_LANGSERVER		9

typedef struct _ssh_t{
	xhand_head head;

	if_bio_t* pif;

	int type;
	//server or client handshake state
	int state;

	//random bytes generator
	havege_state rng;

	//algorithm method table
	schar_t alg_method[SSH_ALGO_NAMETABLE_SIZE][256];

	//server or client key exchange dhm context
	dhm_context kex_dhm;
	//server or client key exchange hash context, eg: sha1_context, sha2_context, sha4_context
	void* kex_sha;
	//server or client key exchange secret context, eg: rsa_context, dss_context
	void* kex_ctx;

	//client banner
	schar_t kex_VC[SSH_BANNER_SIZE];
	//server banner
	schar_t kex_VS[SSH_BANNER_SIZE];
	//client key exchange algorithm namelist tokens
	byte_t* kex_IC;
	dword_t len_IC;
	//server key exchange algorithm namelist tokens
	byte_t* kex_IS;
	dword_t len_IS;
	//the key exchange K_S token
	byte_t* kex_KS;
	dword_t len_KS;
	//client mpi e blob
	byte_t* kex_E;
	dword_t len_E;
	//server mpi f blob
	byte_t* kex_F;
	dword_t len_F;
	//key exchange H value, and it then used as session id
	byte_t* kex_H;
	dword_t len_H;
	//key exchange signature
	dword_t len_S;
	byte_t* kex_S;

	//the sending packet crypt IV 
	byte_t snd_blk[256];
	dword_t len_snd_blk;
	//the recving packet crypt IV
	byte_t rcv_blk[256];
	dword_t len_rcv_blk;
	//the sending packet crypt key
	byte_t snd_key[256];
	dword_t len_snd_key;
	//the recving packet crypt key
	byte_t rcv_key[256];
	dword_t len_rcv_key;
	//the sending packet hmac key
	byte_t snd_mac[256];
	dword_t len_snd_mac;
	//the recving packet hmac key
	byte_t rcv_mac[256];
	dword_t len_rcv_mac;

	//the crypt context
	void* snd_ciph;
	void* rcv_ciph;
	
	//the hmac context
	void* snd_hmac;
	void* rcv_hmac;

	//the send packet sequence
	int snd_sequ;
	//the recv packet sequence
	int rcv_sequ;

	//session id length
	dword_t len_sess;
	//session id
	byte_t pid_sess[KEY_LEN];

	//the send pdv (user data)
	byte_t snd_pdv[SSH_PDV_SIZE];
	dword_t snd_pdv_len;
	dword_t snd_pdv_pop;

	//the recv pdv (user data)
	byte_t rcv_pdv[SSH_PDV_SIZE];
	dword_t rcv_pdv_len;
	dword_t rcv_pdv_pop;
}ssh_t;

#define SSH_SERVER_STATE_ERROR		-1
#define SSH_SERVER_STATE_INIT		0
#define SSH_SERVER_STATE_BANNER		1
#define SSH_SERVER_STATE_KEXINIT	2
#define SSH_SERVER_STATE_DHREPLY	3
#define SSH_SERVER_STATE_NEWKEYS	4
#define SSH_SERVER_STATE_OK			100

#define SSH_CLIENT_STATE_ERROR		-1
#define SSH_CLIENT_STATE_INIT		0
#define SSH_CLIENT_STATE_BANNER		1
#define SSH_CLIENT_STATE_KEXINIT	2
#define SSH_CLIENT_STATE_DHINIT		3
#define SSH_CLIENT_STATE_NEWKEYS	4
#define SSH_CLIENT_STATE_OK			100

#define SSH_OKEY(pssh)	((pssh->type == SSH_TYPE_SERVER && pssh->state > SSH_SERVER_STATE_NEWKEYS) || (pssh->type == SSH_TYPE_CLIENT && pssh->state > SSH_CLIENT_STATE_NEWKEYS))

/*******************************************PACKET PROTOCOL********************************************/

static int _ssh_hmac_size(const schar_t* hmac_name)
{
	if (a_xsicmp(hmac_name, SSH_HMAC_SHA1) == 0)
		return SSH_HMAC_SHA1_SIZE;
	else if (a_xsicmp(hmac_name, SSH_HMAC_SHA2_256) == 0)
		return SSH_HMAC_SHA2_256_SIZE;
	else if (a_xsicmp(hmac_name, SSH_HMAC_SHA2_512) == 0)
		return SSH_HMAC_SHA2_512_SIZE;
	else if (a_xsicmp(hmac_name, SSH_HMAC_MD5) == 0)
		return SSH_HMAC_MD5_SIZE;
	else if (a_xsicmp(hmac_name, SSH_AEAD_POLY305) == 0)
		return SSH_AEAD_POLY305_SIZE;
	else
		return 0;
}

static int _ssh_ciph_key_size(const schar_t* ciph_name)
{
	if (a_xsicmp(ciph_name, SSH_CIPH_ARC4) == 0)
		return SSH_CIPH_ARC4_KEY_SIZE; 
	else if (a_xsicmp(ciph_name, SSH_CIPH_ARC4_128) == 0)
		return SSH_CIPH_ARC4_128_KEY_SIZE;
	else if (a_xsicmp(ciph_name, SSH_CIPH_ARC4_256) == 0)
		return SSH_CIPH_ARC4_256_KEY_SIZE;
	else if (a_xsicmp(ciph_name, SSH_CIPH_BLOWFISH_CBC) == 0)
		return SSH_CIPH_BLOWFISH_KEY_SIZE;
	else if (a_xsicmp(ciph_name, SSH_CIPH_AES128_CTR) == 0)
		return SSH_CIPH_AES128_CTR_KEY_SIZ;
	else if (a_xsicmp(ciph_name, SSH_CIPH_AES128_CBC) == 0)
		return SSH_CIPH_AES128_CBC_KEY_SIZE;
	else if (a_xsicmp(ciph_name, SSH_CIPH_AES192_CTR) == 0)
		return SSH_CIPH_AES192_CTR_KEY_SIZE;
	else if (a_xsicmp(ciph_name, SSH_CIPH_AES192_CBC) == 0)
		return SSH_CIPH_AES192_CBC_KEY_SIZE;
	else if (a_xsicmp(ciph_name, SSH_CIPH_AES256_CTR) == 0)
		return SSH_CIPH_AES256_CTR_KEY_SIZE;
	else if (a_xsicmp(ciph_name, SSH_CIPH_AES256_CBC) == 0)
		return SSH_CIPH_AES256_CBC_KEY_SIZE;
	else if (a_xsicmp(ciph_name, SSH_CIPH_3DES_CBC) == 0)
		return SSH_CIPH_3DES_CBC_KEY_SIZE;
	else
		return 0;
}

static int _ssh_ciph_blk_size(const schar_t* ciph_name)
{
	if (a_xsicmp(ciph_name, SSH_CIPH_BLOWFISH_CBC) == 0)
		return SSH_CIPH_BLOWFISH_BLK_SIZE;
	else if (a_xsicmp(ciph_name, SSH_CIPH_AES128_CTR) == 0)
		return SSH_CIPH_AES128_CTR_BLK_SIZ;
	else if (a_xsicmp(ciph_name, SSH_CIPH_AES128_CBC) == 0)
		return SSH_CIPH_AES128_CBC_BLK_SIZE;
	else if (a_xsicmp(ciph_name, SSH_CIPH_AES192_CTR) == 0)
		return SSH_CIPH_AES192_CTR_BLK_SIZE;
	else if (a_xsicmp(ciph_name, SSH_CIPH_AES192_CBC) == 0)
		return SSH_CIPH_AES192_CBC_BLK_SIZE;
	else if (a_xsicmp(ciph_name, SSH_CIPH_AES256_CTR) == 0)
		return SSH_CIPH_AES256_CTR_BLK_SIZE;
	else if (a_xsicmp(ciph_name, SSH_CIPH_AES256_CBC) == 0)
		return SSH_CIPH_AES256_CBC_BLK_SIZE;
	else if (a_xsicmp(ciph_name, SSH_CIPH_3DES_CBC) == 0)
		return SSH_CIPH_3DES_CBC_BLK_SIZE;
	else
		return 0;
}

static int _ssh_write_packet(ssh_t* pssh, byte_t* payload, dword_t size)
{
	dword_t block_len, packet_len, padding_len, payload_len, hmac_len;
	byte_t num_buf[8], rng_buf[32], mac_buf[128];
	dword_t i, n;

	const schar_t* hmac_name;
	int hmac_size;
	const schar_t* ciph_name;
	int blk_size;

	if (size > SSH_PDV_SIZE)
	{
		set_last_error(_T("_ssh_write_packet"), _T("too large payload"), -1);
		return C_ERR;
	}

	if (SSH_OKEY(pssh) && pssh->type == SSH_TYPE_SERVER)
	{
		hmac_name = pssh->alg_method[SSH_METHOD_INDEX_HMACSERVER];
		hmac_size = _ssh_hmac_size(hmac_name);

		ciph_name = pssh->alg_method[SSH_METHOD_INDEX_CIPHSERVER];
		blk_size = _ssh_ciph_blk_size(ciph_name);
	}
	else if (SSH_OKEY(pssh) && pssh->type == SSH_TYPE_CLIENT)
	{
		hmac_name = pssh->alg_method[SSH_METHOD_INDEX_HMACCLIENT];
		hmac_size = _ssh_hmac_size(hmac_name);

		ciph_name = pssh->alg_method[SSH_METHOD_INDEX_CIPHCLIENT];
		blk_size = _ssh_ciph_blk_size(ciph_name);
	}
	else
	{
		hmac_name = NULL;
		hmac_size = 0;

		ciph_name = NULL;
		blk_size = 0;
	}

	if (a_xsicmp(hmac_name, SSH_HMAC_SHA1) == 0)
	{
		sha1_hmac_starts((sha1_context*)pssh->snd_hmac, pssh->snd_mac, pssh->len_snd_mac);
	}
	else if (a_xsicmp(hmac_name, SSH_HMAC_SHA2_256) == 0)
	{
		sha2_hmac_starts((sha2_context*)pssh->snd_hmac, pssh->snd_mac, pssh->len_snd_mac, 0);
	}
	else if (a_xsicmp(hmac_name, SSH_HMAC_SHA2_512) == 0)
	{
		sha4_hmac_starts((sha4_context*)pssh->snd_hmac, pssh->snd_mac, pssh->len_snd_mac, 0);
	}

	payload_len = size;
	block_len = (blk_size) ? blk_size : 8;
	padding_len = block_len - payload_len % block_len;
	if (padding_len < 4)
		padding_len += block_len;
	hmac_len = hmac_size;

	//packet sequence
	PUT_DWORD_NET(num_buf, 0, pssh->snd_sequ);
	n = 4;
	//data integrity
	if (a_xsicmp(hmac_name, SSH_HMAC_SHA1) == 0)
	{
		sha1_hmac_update((sha1_context*)pssh->snd_hmac, num_buf, n);
	}
	else if (a_xsicmp(hmac_name, SSH_HMAC_SHA2_256) == 0)
	{
		sha2_hmac_update((sha2_context*)pssh->snd_hmac, num_buf, n);
	}
	else if (a_xsicmp(hmac_name, SSH_HMAC_SHA2_512) == 0)
	{
		sha4_hmac_update((sha4_context*)pssh->snd_hmac, num_buf, n);
	}

	packet_len = 1 + payload_len + padding_len;

	//0~3: packet length
	PUT_DWORD_NET(num_buf, 0, packet_len);
	n = 4;
	//data integrity
	if (a_xsicmp(hmac_name, SSH_HMAC_SHA1) == 0)
	{
		sha1_hmac_update((sha1_context*)pssh->snd_hmac, num_buf, n);
	}
	else if (a_xsicmp(hmac_name, SSH_HMAC_SHA2_256) == 0)
	{
		sha2_hmac_update((sha2_context*)pssh->snd_hmac, num_buf, n);
	}
	else if (a_xsicmp(hmac_name, SSH_HMAC_SHA2_512) == 0)
	{
		sha4_hmac_update((sha4_context*)pssh->snd_hmac, num_buf, n);
	}
	//encrypt
	if (a_xsicmp(ciph_name, SSH_CIPH_ARC4) == 0 || a_xsicmp(ciph_name, SSH_CIPH_ARC4_128) == 0 || a_xsicmp(ciph_name, SSH_CIPH_ARC4_256) == 0)
	{
		arc4_crypt((arc4_context*)pssh->snd_ciph, (int)n, num_buf, num_buf);
	}
	
	if (!(*pssh->pif->pf_write)(pssh->pif->fd, num_buf, &n))
	{
		return C_ERR;
	}

	//4: padding length
	PUT_BYTE(num_buf, 0, (byte_t)padding_len);
	n = 1;
	//data integrity
	if (a_xsicmp(hmac_name, SSH_HMAC_SHA1) == 0)
	{
		sha1_hmac_update((sha1_context*)pssh->snd_hmac, num_buf, n);
	}
	else if (a_xsicmp(hmac_name, SSH_HMAC_SHA2_256) == 0)
	{
		sha2_hmac_update((sha2_context*)pssh->snd_hmac, num_buf, n);
	}
	else if (a_xsicmp(hmac_name, SSH_HMAC_SHA2_512) == 0)
	{
		sha4_hmac_update((sha4_context*)pssh->snd_hmac, num_buf, n);
	}
	//encrypt
	if (a_xsicmp(ciph_name, SSH_CIPH_ARC4) == 0 || a_xsicmp(ciph_name, SSH_CIPH_ARC4_128) == 0 || a_xsicmp(ciph_name, SSH_CIPH_ARC4_256) == 0)
	{
		arc4_crypt((arc4_context*)pssh->snd_ciph, (int)n, num_buf, num_buf);
	}

	if (!(*pssh->pif->pf_write)(pssh->pif->fd, num_buf, &n))
	{
		return C_ERR;
	}

	//5~(5+n1-1): payload
	n = payload_len;

	//data integrity
	if (a_xsicmp(hmac_name, SSH_HMAC_SHA1) == 0)
	{
		sha1_hmac_update((sha1_context*)pssh->snd_hmac, payload, n);
	}
	else if (a_xsicmp(hmac_name, SSH_HMAC_SHA2_256) == 0)
	{
		sha2_hmac_update((sha2_context*)pssh->snd_hmac, payload, n);
	}
	else if (a_xsicmp(hmac_name, SSH_HMAC_SHA2_512) == 0)
	{
		sha4_hmac_update((sha4_context*)pssh->snd_hmac, payload, n);
	}
	//encrypt
	if (a_xsicmp(ciph_name, SSH_CIPH_ARC4) == 0 || a_xsicmp(ciph_name, SSH_CIPH_ARC4_128) == 0 || a_xsicmp(ciph_name, SSH_CIPH_ARC4_256) == 0)
	{
		arc4_crypt((arc4_context*)pssh->snd_ciph, (int)n, payload, payload);
	}

	if (!(pssh->pif->pf_write)(pssh->pif->fd, payload, &n))
	{
		return C_ERR;
	}

	//(5+n1)~(5+n1+n2-1): padding
	n = padding_len;
	for (i = 0; i < n; i++)
	{
		rng_buf[i] = (byte_t)havege_rand(&pssh->rng);
	}
	//data integrity
	if (a_xsicmp(hmac_name, SSH_HMAC_SHA1) == 0)
	{
		sha1_hmac_update((sha1_context*)pssh->snd_hmac, rng_buf, n);
	}
	else if (a_xsicmp(hmac_name, SSH_HMAC_SHA2_256) == 0)
	{
		sha2_hmac_update((sha2_context*)pssh->snd_hmac, rng_buf, n);
	}
	else if (a_xsicmp(hmac_name, SSH_HMAC_SHA2_512) == 0)
	{
		sha4_hmac_update((sha4_context*)pssh->snd_hmac, rng_buf, n);
	}
	//encrypt
	if (a_xsicmp(ciph_name, SSH_CIPH_ARC4) == 0 || a_xsicmp(ciph_name, SSH_CIPH_ARC4_128) == 0 || a_xsicmp(ciph_name, SSH_CIPH_ARC4_256) == 0)
	{
		arc4_crypt((arc4_context*)pssh->snd_ciph, (int)n, rng_buf, rng_buf);
	}
	if (!(pssh->pif->pf_write)(pssh->pif->fd, rng_buf, &n))
	{
		return C_ERR;
	}

	//hmac final
	if (a_xsicmp(hmac_name, SSH_HMAC_SHA1) == 0)
	{
		sha1_hmac_finish((sha1_context*)pssh->snd_hmac, mac_buf);
	}
	else if (a_xsicmp(hmac_name, SSH_HMAC_SHA2_256) == 0)
	{
		sha2_hmac_finish((sha2_context*)pssh->snd_hmac, mac_buf);
	}
	else if (a_xsicmp(hmac_name, SSH_HMAC_SHA2_512) == 0)
	{
		sha4_hmac_finish((sha4_context*)pssh->snd_hmac, mac_buf);
	}

	//(5+n1+n2)~(5+n1+n2+k-1): mac
	n = hmac_len;
	if (!(pssh->pif->pf_write)(pssh->pif->fd, mac_buf, &n))
	{
		return C_ERR;
	}

	pssh->snd_sequ++;

	return C_OK;

}

static int _ssh_read_packet(ssh_t* pssh, byte_t** pbuf, dword_t* psize)
{
	dword_t packet_len, padding_len, payload_len, hmac_len;
	byte_t num_buf[8], rng_buf[256], mac_buf[128], rcv_mac[128];
	dword_t n;
	byte_t* payload = NULL;

	const schar_t* hmac_name;
	int hmac_size;
	const schar_t* ciph_name;
	int blk_size;

	if (SSH_OKEY(pssh) && pssh->type == SSH_TYPE_SERVER)
	{
		hmac_name = pssh->alg_method[SSH_METHOD_INDEX_HMACCLIENT];
		hmac_size = _ssh_hmac_size(hmac_name);

		ciph_name = pssh->alg_method[SSH_METHOD_INDEX_CIPHCLIENT];
		blk_size = _ssh_ciph_blk_size(ciph_name);
	}
	else if (SSH_OKEY(pssh) && pssh->type == SSH_TYPE_CLIENT)
	{
		hmac_name = pssh->alg_method[SSH_METHOD_INDEX_HMACSERVER];
		hmac_size = _ssh_hmac_size(hmac_name);

		ciph_name = pssh->alg_method[SSH_METHOD_INDEX_CIPHSERVER];
		blk_size = _ssh_ciph_blk_size(ciph_name);
	}
	else
	{
		hmac_name = NULL;
		hmac_size = 0;

		ciph_name = NULL;
		blk_size = 0;
	}

	if (a_xsicmp(hmac_name, SSH_HMAC_SHA1) == 0)
	{
		sha1_hmac_starts((sha1_context*)pssh->rcv_hmac, pssh->rcv_mac, pssh->len_rcv_mac);
	}
	else if (a_xsicmp(hmac_name, SSH_HMAC_SHA2_256) == 0)
	{
		sha2_hmac_starts((sha2_context*)pssh->rcv_hmac, pssh->rcv_mac, pssh->len_rcv_mac, 0);
	}
	else if (a_xsicmp(hmac_name, SSH_HMAC_SHA2_512) == 0)
	{
		sha4_hmac_starts((sha4_context*)pssh->rcv_hmac, pssh->rcv_mac, pssh->len_rcv_mac, 0);
	}

	//packet sequence
	PUT_DWORD_NET(num_buf, 0, pssh->rcv_sequ);
	n = 4;
	//data integrity
	if (a_xsicmp(hmac_name, SSH_HMAC_SHA1) == 0)
	{
		sha1_hmac_update((sha1_context*)pssh->rcv_hmac, num_buf, n);
	}
	else if (a_xsicmp(hmac_name, SSH_HMAC_SHA2_256) == 0)
	{
		sha2_hmac_update((sha2_context*)pssh->rcv_hmac, num_buf, n);
	}
	else if (a_xsicmp(hmac_name, SSH_HMAC_SHA2_512) == 0)
	{
		sha4_hmac_update((sha4_context*)pssh->rcv_hmac, num_buf, n);
	}

	//0~3: packet length
	n = 4;
	if (!(*pssh->pif->pf_read)(pssh->pif->fd, num_buf, &n))
	{
		return C_ERR;
	}
	//decrypt
	if (a_xsicmp(ciph_name, SSH_CIPH_ARC4) == 0 || a_xsicmp(ciph_name, SSH_CIPH_ARC4_128) == 0 || a_xsicmp(ciph_name, SSH_CIPH_ARC4_256) == 0)
	{
		arc4_crypt((arc4_context*)pssh->rcv_ciph, (int)n, num_buf, num_buf);
	}
	packet_len = GET_DWORD_NET(num_buf, 0);
	if (packet_len > SSH_PDV_SIZE)
	{
		set_last_error(_T("_ssh_read_packet"), _T("too large payload"), -1);
		return C_ERR;
	}
	//data integrity
	if (a_xsicmp(hmac_name, SSH_HMAC_SHA1) == 0)
	{
		sha1_hmac_update((sha1_context*)pssh->rcv_hmac, num_buf, n);
	}
	else if (a_xsicmp(hmac_name, SSH_HMAC_SHA2_256) == 0)
	{
		sha2_hmac_update((sha2_context*)pssh->rcv_hmac, num_buf, n);
	}
	else if (a_xsicmp(hmac_name, SSH_HMAC_SHA2_512) == 0)
	{
		sha4_hmac_update((sha4_context*)pssh->rcv_hmac, num_buf, n);
	}

	//4: padding length
	n = 1;
	if (!(*pssh->pif->pf_read)(pssh->pif->fd, num_buf, &n))
	{
		return C_ERR;
	}
	//decrypt
	if (a_xsicmp(ciph_name, SSH_CIPH_ARC4) == 0 || a_xsicmp(ciph_name, SSH_CIPH_ARC4_128) == 0 || a_xsicmp(ciph_name, SSH_CIPH_ARC4_256) == 0)
	{
		arc4_crypt((arc4_context*)pssh->rcv_ciph, (int)n, num_buf, num_buf);
	}
	padding_len = (dword_t)GET_BYTE(num_buf, 0);
	//data integrity
	if (a_xsicmp(hmac_name, SSH_HMAC_SHA1) == 0)
	{
		sha1_hmac_update((sha1_context*)pssh->rcv_hmac, num_buf, n);
	}
	else if (a_xsicmp(hmac_name, SSH_HMAC_SHA2_256) == 0)
	{
		sha2_hmac_update((sha2_context*)pssh->rcv_hmac, num_buf, n);
	}
	else if (a_xsicmp(hmac_name, SSH_HMAC_SHA2_512) == 0)
	{
		sha4_hmac_update((sha4_context*)pssh->rcv_hmac, num_buf, n);
	}

	payload_len = packet_len - padding_len - 1;

	payload = bytes_realloc(pbuf, payload_len);

	//5~(5+n1-1): payload
	n = payload_len;
	if (!(*pssh->pif->pf_read)(pssh->pif->fd, payload, &n))
	{
		*psize = 0;
		return C_ERR;
	}
	*psize = n;
	//decrypt
	if (a_xsicmp(ciph_name, SSH_CIPH_ARC4) == 0 || a_xsicmp(ciph_name, SSH_CIPH_ARC4_128) == 0 || a_xsicmp(ciph_name, SSH_CIPH_ARC4_256) == 0)
	{
		arc4_crypt((arc4_context*)pssh->rcv_ciph, (int)n, payload, payload);
	}
	//data integrity
	if (a_xsicmp(hmac_name, SSH_HMAC_SHA1) == 0)
	{
		sha1_hmac_update((sha1_context*)pssh->rcv_hmac, payload, n);
	}
	else if (a_xsicmp(hmac_name, SSH_HMAC_SHA2_256) == 0)
	{
		sha2_hmac_update((sha2_context*)pssh->rcv_hmac, payload, n);
	}
	else if (a_xsicmp(hmac_name, SSH_HMAC_SHA2_512) == 0)
	{
		sha4_hmac_update((sha4_context*)pssh->rcv_hmac, payload, n);
	}

	//(5+n1)~(5+n1+n2-1): padding
	n = padding_len;
	if (!(*pssh->pif->pf_read)(pssh->pif->fd, rng_buf, &n))
	{
		return C_ERR;
	}
	//decrypt
	if (a_xsicmp(ciph_name, SSH_CIPH_ARC4) == 0 || a_xsicmp(ciph_name, SSH_CIPH_ARC4_128) == 0 || a_xsicmp(ciph_name, SSH_CIPH_ARC4_256) == 0)
	{
		arc4_crypt((arc4_context*)pssh->rcv_ciph, (int)n, rng_buf, rng_buf);
	}
	//data integrity
	if (a_xsicmp(hmac_name, SSH_HMAC_SHA1) == 0)
	{
		sha1_hmac_update((sha1_context*)pssh->rcv_hmac, rng_buf, n);
	}
	else if (a_xsicmp(hmac_name, SSH_HMAC_SHA2_256) == 0)
	{
		sha2_hmac_update((sha2_context*)pssh->rcv_hmac, rng_buf, n);
	}
	else if (a_xsicmp(hmac_name, SSH_HMAC_SHA2_512) == 0)
	{
		sha4_hmac_update((sha4_context*)pssh->rcv_hmac, rng_buf, n);
	}

	//hmac final
	if (a_xsicmp(hmac_name, SSH_HMAC_SHA1) == 0)
	{
		sha1_hmac_finish((sha1_context*)pssh->rcv_hmac, mac_buf);
	}
	else if (a_xsicmp(hmac_name, SSH_HMAC_SHA2_256) == 0)
	{
		sha2_hmac_finish((sha2_context*)pssh->rcv_hmac, mac_buf);
	}
	else if (a_xsicmp(hmac_name, SSH_HMAC_SHA2_512) == 0)
	{
		sha4_hmac_finish((sha4_context*)pssh->rcv_hmac, mac_buf);
	}

	hmac_len = hmac_size;

	//(5+n1+n2)~(5+n1+n2+k-1): mac
	n = hmac_len;
	if (!(*pssh->pif->pf_read)(pssh->pif->fd, rcv_mac, &n))
	{
		return C_ERR;
	}

	if (xmem_comp((void*)mac_buf, hmac_len, (void*)rcv_mac, hmac_len) != 0)
	{
		set_last_error(_T("_ssh_read_packet"), _T("data integrity error"), -1);
		return C_ERR;
	}

	pssh->rcv_sequ++;

	return C_OK;
}

static byte_t _ssh_packet_msg(const byte_t* payload, dword_t size)
{
	if (!size)
	{
		return 0;
	}

	return GET_BYTE(payload, 0);
}

static int _ssh_compress_payload(ssh_t* pssh, byte_t* payload, dword_t* psize)
{
	dword_t src_len, zip_len;
	byte_t* zip_buf;

	const schar_t* compress_name;

	if (SSH_OKEY(pssh) && pssh->type == SSH_TYPE_SERVER)
	{
		compress_name = pssh->alg_method[SSH_METHOD_INDEX_COMPRESSSERVER];
	}
	else if (SSH_OKEY(pssh))
	{
		compress_name = pssh->alg_method[SSH_METHOD_INDEX_COMPRESSCLIENT];
	}
	else
	{
		compress_name = NULL;
	}

	if (a_xsicmp(compress_name, SSH_ZIP_ZLIB) == 0)
	{
		src_len = zip_len = *psize;
		zip_buf = (byte_t*)xmem_alloc(zip_len);
		if (!xzlib_compress_bytes(payload, src_len, zip_buf, &zip_len))
		{
			xmem_free(zip_buf);

			set_last_error(_T("_ssh_compress_payload"), _T("commpressing failed"), -1);
			return C_ERR;
		}

		xmem_copy((void*)payload, (void*)zip_buf, zip_len);
		xmem_free(zip_buf);
		*psize = zip_len;
	}

	return C_OK;
}

static int _ssh_uncompress_payload(ssh_t* pssh, byte_t* payload, dword_t* psize)
{
	dword_t src_len, unzip_len;
	byte_t* unzip_buf;
	
	const schar_t* compress_name;

	if (SSH_OKEY(pssh) && pssh->type == SSH_TYPE_SERVER)
	{
		compress_name = pssh->alg_method[SSH_METHOD_INDEX_COMPRESSCLIENT];
	}
	else if (SSH_OKEY(pssh))
	{
		compress_name = pssh->alg_method[SSH_METHOD_INDEX_COMPRESSSERVER];
	}
	else
	{
		compress_name = NULL;
	}

	if (a_xsicmp(compress_name, SSH_ZIP_ZLIB) == 0)
	{
		src_len = *psize;
		unzip_len = SSH_PDV_SIZE;
		unzip_buf = (byte_t*)xmem_alloc(unzip_len);
		if (!xzlib_uncompress_bytes(payload, src_len, unzip_buf, &unzip_len))
		{
			xmem_free(unzip_buf);

			set_last_error(_T("_ssh_uncompress_payload"), _T("uncommpressing failed"), -1);
			return C_ERR;
		}

		xmem_copy((void*)payload, (void*)unzip_buf, unzip_len);
		xmem_free(unzip_buf);
		*psize = unzip_len;
	}

	return C_OK;
}

/*******************************************TRANSPORT PROTOCOL********************************************/

static void _ssh_start_kex_secret(ssh_t* pssh)
{
	const schar_t* kex_dh = pssh->alg_method[SSH_METHOD_INDEX_KEXDH];

	if (a_xsicmp(kex_dh, SSH_KEXDH_GROUP1_SHA1) == 0)
	{
		pssh->kex_sha = (sha1_context*)xmem_alloc(sizeof(sha1_context));
		sha1_starts((sha1_context*)pssh->kex_sha);
		pssh->len_H = 0;
	}
	else if (a_xsicmp(kex_dh, SSH_KEXDH_GROUP14_SHA1) == 0)
	{
		pssh->kex_sha = (sha1_context*)xmem_alloc(sizeof(sha1_context));
		sha1_starts((sha1_context*)pssh->kex_sha);
		pssh->len_H = 0;
	}
	else if (a_xsicmp(kex_dh, SSH_KEXDH_GROUP16_SHA512) == 0)
	{
		pssh->kex_sha = (sha4_context*)xmem_alloc(sizeof(sha4_context));
		sha4_starts((sha4_context*)pssh->kex_sha, 0);
		pssh->len_H = 0;
	}
	else if (a_xsicmp(kex_dh, SSH_KEXDH_GROUP16_SHA512) == 0)
	{
		pssh->kex_sha = (sha4_context*)xmem_alloc(sizeof(sha4_context));
		sha4_starts((sha4_context*)pssh->kex_sha, 0);
		pssh->len_H = 0;
	}
}

static void _ssh_update_kex_secret(ssh_t* pssh, const byte_t* buf, dword_t len)
{
	const schar_t* kex_dh = pssh->alg_method[SSH_METHOD_INDEX_KEXDH];

	if (a_xsicmp(kex_dh, SSH_KEXDH_GROUP1_SHA1) == 0)
	{
		sha1_update((sha1_context*)pssh->kex_sha, buf, len);
	}
	else if (a_xsicmp(kex_dh, SSH_KEXDH_GROUP14_SHA1) == 0)
	{
		sha1_update((sha1_context*)pssh->kex_sha, buf, len);
	}
	else if (a_xsicmp(kex_dh, SSH_KEXDH_GROUP16_SHA512) == 0)
	{
		sha4_update((sha4_context*)pssh->kex_sha, buf, len);
	}
	else if (a_xsicmp(kex_dh, SSH_KEXDH_GROUP16_SHA512) == 0)
	{
		sha4_update((sha4_context*)pssh->kex_sha, buf, len);
	}
}

static void _ssh_finish_kex_secret(ssh_t* pssh)
{
	const schar_t* kex_dh = pssh->alg_method[SSH_METHOD_INDEX_KEXDH];

	if (a_xsicmp(kex_dh, SSH_KEXDH_GROUP1_SHA1) == 0)
	{
		pssh->len_H = SSH_HMAC_SHA1_SIZE;
		pssh->kex_H = (byte_t*)xmem_alloc(pssh->len_H);
		sha1_finish((sha1_context*)pssh->kex_sha, pssh->kex_H);
	}
	else if (a_xsicmp(kex_dh, SSH_KEXDH_GROUP14_SHA1) == 0)
	{
		pssh->len_H = SSH_HMAC_SHA1_SIZE;
		pssh->kex_H = (byte_t*)xmem_alloc(pssh->len_H);
		sha1_finish((sha1_context*)pssh->kex_sha, pssh->kex_H);
	}
	else if (a_xsicmp(kex_dh, SSH_KEXDH_GROUP16_SHA512) == 0)
	{
		pssh->len_H = SSH_HMAC_SHA2_512_SIZE;
		pssh->kex_H = (byte_t*)xmem_alloc(pssh->len_H);
		sha4_finish((sha4_context*)pssh->kex_sha, pssh->kex_H);
	}
	else if (a_xsicmp(kex_dh, SSH_KEXDH_GROUP16_SHA512) == 0)
	{
		pssh->len_H = SSH_HMAC_SHA2_512_SIZE;
		pssh->kex_H = (byte_t*)xmem_alloc(pssh->len_H);
		sha4_finish((sha4_context*)pssh->kex_sha, pssh->kex_H);
	}

	if (pssh->kex_sha)
	{
		xmem_free(pssh->kex_sha);
		pssh->kex_sha = NULL;
	}
}

static void _ssh_init(ssh_t* pssh)
{
	havege_init(&pssh->rng);
}

static void _ssh_reset(ssh_t* pssh)
{
	const schar_t* kex_type;

	xmem_zero((void*)pssh->kex_VS, SSH_BANNER_SIZE);

	xmem_zero((void*)pssh->kex_VC, SSH_BANNER_SIZE);
	
	if (pssh->kex_IS)
	{
		xmem_free(pssh->kex_IS);
		pssh->kex_IS = NULL;
		pssh->len_IS = 0;
	}

	if (pssh->kex_IC)
	{
		xmem_free(pssh->kex_IC);
		pssh->kex_IC = NULL;
		pssh->len_IC = 0;
	}

	if (pssh->kex_F)
	{
		xmem_free(pssh->kex_F);
		pssh->kex_F = NULL;
		pssh->len_F = 0;
	}

	if (pssh->kex_E)
	{
		xmem_free(pssh->kex_E);
		pssh->kex_E = NULL;
		pssh->len_E = 0;
	}

	if (pssh->kex_KS)
	{
		xmem_free(pssh->kex_KS);
		pssh->len_KS = 0;
	}

	if (pssh->kex_H)
	{
		xmem_free(pssh->kex_H);
		pssh->kex_H = NULL;
		pssh->len_H = 0;
	}

	if (pssh->kex_S)
	{
		xmem_free(pssh->kex_S);
		pssh->kex_S = NULL;
		pssh->len_S = 0;
	}

	if (pssh->kex_sha)
	{
		xmem_free(pssh->kex_sha);
	}

	if (pssh->kex_ctx)
	{
		kex_type = pssh->alg_method[SSH_METHOD_INDEX_KEYSECRET];

		if (a_xscmp(kex_type, SSH_KEYSECRET_RSA) == 0)
		{
			rsa_free((rsa_context*)pssh->kex_ctx);
		}

		xmem_free(pssh->kex_ctx);
		pssh->kex_ctx = NULL;
	}

	dhm_free(&(pssh->kex_dhm));
	xmem_zero((void*)&(pssh->kex_dhm), sizeof(dhm_context));
}

static void _ssh_uninit(ssh_t* pssh)
{
	_ssh_reset(pssh);

	if (pssh->snd_ciph)
	{
		xmem_free(pssh->snd_ciph);
		pssh->snd_ciph = NULL;
	}

	if (pssh->rcv_ciph)
	{
		xmem_free(pssh->rcv_ciph);
		pssh->rcv_ciph = NULL;
	}

	if (pssh->snd_hmac)
	{
		xmem_free(pssh->snd_hmac);
		pssh->snd_hmac = NULL;
	}
	
	if (pssh->rcv_hmac)
	{
		xmem_free(pssh->rcv_hmac);
		pssh->rcv_hmac = NULL;
	}
}

static int _ssh_send_banner(ssh_t* pssh)
{
	dword_t n;
	byte_t* buf;
	byte_t bs[2] = {'\r','\n'};

	if (pssh->type == SSH_TYPE_SERVER)
	{
		a_xscpy(pssh->kex_VS, SSH_VER_SERVER);
		n = a_xslen(pssh->kex_VS);
		buf = pssh->kex_VS;
	}
	else
	{
		a_xscpy(pssh->kex_VC, SSH_VER_CLIENT);
		n = a_xslen(pssh->kex_VC);
		buf = pssh->kex_VC;
	}

	if (!(*pssh->pif->pf_write)(pssh->pif->fd, buf, &n))
	{
		return C_ERR;
	}

	n = 2;
	if (!(*pssh->pif->pf_write)(pssh->pif->fd, (byte_t*)bs, &n))
	{
		return C_ERR;
	}

	return C_OK;
}

static int _ssh_recv_banner(ssh_t* pssh)
{
	dword_t n;
	byte_t* buf;
	int i;

	if (pssh->type == SSH_TYPE_SERVER)
	{
		buf = pssh->kex_VC;
	}
	else
	{
		buf = pssh->kex_VS;
	}

	for (i = 0; i < SSH_BANNER_SIZE; i++)
	{
		n = 1;
		if (!(*pssh->pif->pf_read)(pssh->pif->fd, (byte_t*)(buf + i), &n))
			return C_ERR;

		if (buf[i] == '\r')
			buf[i] = '\0';

		if (buf[i] == '\n')
		{
			buf[i] = '\0';
			break;
		}
	}

	return C_OK;
}

static dword_t _ssh_format_kexinit(ssh_t* pssh, byte_t* buf, dword_t max)
{
	dword_t n, total = 0;
	schar_t** nametable;
	int i;

	if (pssh->type == SSH_TYPE_SERVER)
		nametable = ssh_algo_server;
	else
		nametable = ssh_algo_client;

	//0: packet type
	if (total + 1 > max)
		return total;
	if (buf)
	{
		PUT_BYTE(buf, total, SSH2_MSG_KEXINIT);
	}
	total += 1;

	//1~16: cookie
	if (total + SSH_COOKIE_SIZE > max)
		return total;
	if (buf)
	{
		for (i = 0; i < SSH_COOKIE_SIZE; i++)
		{
			buf[total + i] = (byte_t)havege_rand(&pssh->rng);
		}
	}
	total += SSH_COOKIE_SIZE;

	//17~n: kex namelist
	for (i = 0; i < SSH_ALGO_NAMETABLE_SIZE; i++)
	{
		n = a_xslen(nametable[i]);

		if (total + 4 > max)
			return total;
		if (buf)
		{
			PUT_DWORD_NET(buf, total, n);
		}
		total += 4;

		if (total + n > max)
			return total;
		if (buf)
		{
			xmem_copy((void*)(buf + total), (void*)nametable[i], n);
		}
		total += n;
	}

	//(17+n)~(17+n+1): first_kex_packet_follows
	if (total + 1 > max)
		return total;
	if (buf)
	{
		PUT_BYTE(buf, total, 0);
	}
	total += 1;

	//(17+n+2)~(17+n+5): reserved for future extension
	if (total + 4 > max)
		return total;
	if (buf)
	{
		PUT_DWORD_NET(buf, total, 0);
	}
	total += 4;

	return total;
}

static bool_t _ssh_parse_kexinit(ssh_t* pssh, const byte_t* buf, dword_t len)
{
	byte_t msg;
	dword_t n, total = 0;
	schar_t algo[1024];
	const schar_t* srv_token;
	const schar_t* cli_token;
	int i, srv_len, cli_len;
	bool_t matched;
	byte_t cookie[SSH_COOKIE_SIZE] = { 0 };
	byte_t fllow = 0;
	dword_t ext = 0;

	//0: packet type
	if (total + 1 > len)
	{
		set_last_error(_T("_ssh_parse_kexinit"), _T("invalid KEXINIT pakcet size"), -1);
		return 0;
	}
	msg = GET_BYTE(buf, total);
	total += 1;

	if (msg != SSH2_MSG_KEXINIT)
	{
		set_last_error(_T("_ssh_parse_kexinit"), _T("invalid KEXINIT message type"), -1);
		return 0;
	}

	//1~16: cookie
	if (total + SSH_COOKIE_SIZE > len)
	{
		set_last_error(_T("_ssh_parse_kexinit"), _T("invalid KEXINIT pakcet size"), -1);
		return 0;
	}
	xmem_copy((void*)cookie, (void*)(buf + total), SSH_COOKIE_SIZE);
	total += SSH_COOKIE_SIZE;

	//17~n: kex namelist
	for (i = 0; i < SSH_ALGO_NAMETABLE_SIZE; i++)
	{
		if (total + 4 > len)
		{
			set_last_error(_T("_ssh_parse_kexinit"), _T("invalid KEXINIT pakcet size"), -1);
			return 0;
		}
		n = GET_DWORD_NET(buf, total);
		total += 4;

		a_xsncpy(algo, (schar_t*)(buf + total), n);
		algo[n] = '\0';
		total += n;

		if (pssh->type == SSH_TYPE_SERVER)
		{
			srv_token = ssh_algo_server[i];
			cli_token = algo;
		}
		else
		{
			srv_token = algo;
			cli_token = ssh_algo_client[i];
		}

        matched = 0;
        cli_len = 0;
        
		while (*cli_token)
		{
			cli_len = 0;
			while (*(cli_token + cli_len) != ',' && *(cli_token + cli_len) != '\0')
				cli_len++;

			matched = 0;

			if (cli_len)
			{
				while (*srv_token)
				{
					srv_len = 0;
					while (*(srv_token + srv_len) != ',' && *(srv_token + srv_len) != '\0')
						srv_len++;

					if (cli_len == srv_len && a_xsnicmp(cli_token, srv_token, srv_len) == 0)
					{
						matched = 1;
						break;
					}

					srv_token += srv_len;
					if (*(srv_token) == ',')
						srv_token++;
				}
			}

			if (matched)
				break;

			if (pssh->type == SSH_TYPE_SERVER)
			{
				srv_token = ssh_algo_server[i];
			}
			else
			{
				srv_token = algo;
			}

			cli_token += cli_len;
			if (*(cli_token) == ',')
				cli_token++;
			cli_len = 0;
		}

		if (matched)
			a_xsncpy(pssh->alg_method[i], cli_token, cli_len);
		else
			a_xscpy(pssh->alg_method[i], "");

		matched = 0;
	}

	//(17+n)~(17+n+1): first_kex_packet_follows
	if (total + 1 > len)
	{
		set_last_error(_T("_ssh_parse_kexinit"), _T("invalid KEXINIT pakcet size"), -1);
		return 0;
	}
	fllow = GET_BYTE(buf, total);
	total += 1;

	//(17+n+2)~(17+n+5): reserved for future extension
	if (total + 4 > len)
	{
		set_last_error(_T("_ssh_parse_kexinit"), _T("invalid KEXINIT pakcet size"), -1);
		return 0;
	}
	ext = GET_DWORD_NET(buf, total);
	total += 4;

	return (total == len)? 1 : 0;
}

static int _ssh_send_kexinit(ssh_t* pssh)
{
	dword_t n;
	byte_t* buf;

	//retain the kexinit payload for H hashing
	n = _ssh_format_kexinit(pssh, NULL, MAX_LONG);

	if (pssh->type == SSH_TYPE_SERVER)
	{
		pssh->kex_IS = (byte_t*)xmem_alloc(n);
		pssh->len_IS = n;
		buf = pssh->kex_IS;
	}
	else
	{
		pssh->kex_IC = (byte_t*)xmem_alloc(n);
		pssh->len_IC = n;
		buf = pssh->kex_IC;
	}

	_ssh_format_kexinit(pssh, buf, n);

	if (C_OK != _ssh_write_packet(pssh, buf, n))
	{
		return C_ERR;
	}

	return C_OK;
}

static int _ssh_recv_kexinit(ssh_t* pssh)
{
	byte_t** pp = NULL;
	dword_t n = 0;
	byte_t pkt = 0;

	pp = bytes_alloc();

	while (1)
	{
		if (C_OK != _ssh_read_packet(pssh, pp, &n))
		{
			bytes_free(pp);

			return C_ERR;
		}

		pkt = _ssh_packet_msg(*pp, n);
		if (pkt != SSH2_MSG_IGNORE && pkt != SSH2_MSG_UNIMPLEMENTED && pkt != SSH2_MSG_DEBUG)
			break;
	}

	if (!_ssh_parse_kexinit(pssh, *pp, n))
	{
		bytes_free(pp);

		return C_ERR;
	}

	//retain I_C or I_S
	if (pssh->type == SSH_TYPE_SERVER)
	{
		pssh->len_IC = n;
		pssh->kex_IC = (byte_t*)xmem_alloc(pssh->len_IC);
		xmem_copy((void*)pssh->kex_IC,(void*)(*pp), n);
	}
	else
	{
		pssh->len_IS = n;
		pssh->kex_IS = (byte_t*)xmem_alloc(pssh->len_IS);
		xmem_copy((void*)pssh->kex_IS, (void*)(*pp), n);
	}

	bytes_free(pp);

	return C_OK;
}

static bool_t _ssh_make_dh(ssh_t* pssh)
{
	const schar_t* kex_dh;
	const schar_t* kex_sec;
	int bits;

	kex_dh = pssh->alg_method[SSH_METHOD_INDEX_KEXDH];

	mpi_read_binary(&(pssh->kex_dhm.G), dhm_G, 1);

	if (a_xsicmp(kex_dh, SSH_KEXDH_GROUP18_SHA512) == 0)
	{
		bits = 8192;
		mpi_read_binary(&(pssh->kex_dhm.P), group18_dhm_P, GROUP18_DHM_P_LEN);
	}
	else if (a_xsicmp(kex_dh, SSH_KEXDH_GROUP16_SHA512) == 0)
	{
		bits = 4096;
		mpi_read_binary(&(pssh->kex_dhm.P), group16_dhm_P, GROUP16_DHM_P_LEN);
	}
	else if (a_xsicmp(kex_dh, SSH_KEXDH_GROUP14_SHA1) == 0)
	{
		bits = 2048;
		mpi_read_binary(&(pssh->kex_dhm.P), group14_dhm_P, GROUP14_DHM_P_LEN);
	}
	else if (a_xsicmp(kex_dh, SSH_KEXDH_GROUP1_SHA1) == 0)
	{
		bits = 1024;
		mpi_read_binary(&(pssh->kex_dhm.P), group1_dhm_P, GROUP1_DHM_P_LEN);
	}else
    {
        bits = 0;
    }

	kex_sec = pssh->alg_method[SSH_METHOD_INDEX_KEYSECRET];

	if (pssh->type == SSH_TYPE_SERVER)
	{
		//make server mpint f
		pssh->len_F = dhm_make_public_size(&pssh->kex_dhm, bits);
		pssh->kex_F = (byte_t*)xmem_alloc(pssh->len_F);

		if (C_OK != dhm_make_public(&(pssh->kex_dhm), bits, pssh->kex_F, &(pssh->len_F), havege_rand, &(pssh->rng)))
		{
			return 0;
		}

		if (a_xscmp(kex_sec, SSH_KEYSECRET_RSA) == 0)
		{
			pssh->kex_ctx = (rsa_context*)xmem_alloc(sizeof(rsa_context));

			rsa_init((rsa_context*)pssh->kex_ctx, RSA_PKCS_V15, RSA_SHA1, havege_rand, &pssh->rng);

			rsa_gen_key((rsa_context*)pssh->kex_ctx, bits, SSH_MIN_PRIM);
		}
	}
	else
	{
		//make client mpint e
		pssh->len_E = dhm_make_public_size(&pssh->kex_dhm, bits);
		pssh->kex_E = (byte_t*)xmem_alloc(pssh->len_E);

		if (C_OK != dhm_make_public(&(pssh->kex_dhm), bits, pssh->kex_E, &(pssh->len_E), havege_rand, &(pssh->rng)))
		{
			return 0;
		}

		if (a_xscmp(kex_sec, SSH_KEYSECRET_RSA) == 0)
		{
			pssh->kex_ctx = (rsa_context*)xmem_alloc(sizeof(rsa_context));

			rsa_init((rsa_context*)pssh->kex_ctx, RSA_PKCS_V15, RSA_SHA1, havege_rand, &pssh->rng);

			rsa_gen_key((rsa_context*)pssh->kex_ctx, bits, SSH_MIN_PRIM);
		}
	}

	return 1;
}

static dword_t _ssh_format_dhinit(ssh_t* pssh, byte_t* buf, dword_t max)
{
	dword_t n, total = 0;

	//0: packet type
	if (total + 1 > max)
		return total;
	if (buf)
	{
		PUT_BYTE(buf, total, SSH2_MSG_KEXDH_INIT);
	}
	total += 1;

	n = pssh->len_E;

	//1~4: dhm e length
	if (total + 4 > max)
		return total;
	if (buf)
	{
		PUT_DWORD_NET(buf, total, n);
	}
	total += 4;

	//5~(5 + n): dhm e
	if (buf)
	{
		xmem_copy((void*)(buf + total), (void*)(pssh->kex_E), pssh->len_E);
	}
	total += n;
	
	return total;
}

static bool_t _ssh_parse_dhinit(ssh_t* pssh, const byte_t* buf, dword_t len)
{
	byte_t msg;
	dword_t n, total = 0;

	//0: packet type
	if (total + 1 > len)
	{
		set_last_error(_T("_ssh_parse_dhinit"), _T("invalid KEXDHINIT pakcet size"), -1);
		return 0;
	}
	msg = GET_BYTE(buf, total);
	total += 1;

	if (msg != SSH2_MSG_KEXDH_INIT)
	{
		set_last_error(_T("_ssh_parse_dhinit"), _T("invalid KEXDHINIT message type"), -1);
		return 0;
	}

	//1~4: dhm e length
	if (total + 4 > len)
	{
		set_last_error(_T("_ssh_parse_dhinit"), _T("invalid KEXDHINIT pakcet size"), -1);
		return 0;
	}
	n = GET_DWORD_NET(buf, total);
	total += 4;

	//5~(5 + n): dhm e data
	pssh->len_E = n;
	pssh->kex_E = (byte_t*)xmem_alloc(pssh->len_E);
	xmem_copy((void*)pssh->kex_E, (void*)(buf + total), n);
	total += n;

	if (C_OK != dhm_read_public(&pssh->kex_dhm, pssh->kex_E, pssh->len_E))
		return 0;

	return (total == len)? 1 : 0;
}

static int _ssh_send_dhinit(ssh_t* pssh)
{
	byte_t* payload;
	dword_t n;

	XDL_ASSERT(pssh->type == SSH_TYPE_CLIENT);

	//make the kexdh context
	if (!_ssh_make_dh(pssh))
		return C_ERR;

	n = _ssh_format_dhinit(pssh, NULL, MAX_LONG);

	payload = (byte_t*)xmem_alloc(n);

	n = _ssh_format_dhinit(pssh, payload, n);

	if (C_OK != _ssh_write_packet(pssh, payload, n))
	{
		xmem_free(payload);

		return C_ERR;
	}

	xmem_free(payload);

	return C_OK;
}

static int _ssh_recv_dhinit(ssh_t* pssh)
{
	byte_t** pp = NULL;
	dword_t n = 0;
	byte_t msg = 0;

	XDL_ASSERT(pssh->type == SSH_TYPE_SERVER);

	//make the kexdh context
	if (!_ssh_make_dh(pssh))
		return C_ERR;

	pp = bytes_alloc();

	while (1)
	{
		if (C_OK != _ssh_read_packet(pssh, pp, &n))
		{
			bytes_free(pp);

			return C_ERR;
		}

		msg = _ssh_packet_msg(*pp, n);
		if (msg != SSH2_MSG_IGNORE && msg != SSH2_MSG_UNIMPLEMENTED && msg != SSH2_MSG_DEBUG)
			break;
	}

	if (!_ssh_parse_dhinit(pssh, *pp, n))
	{
		bytes_free(pp);

		return C_ERR;
	}

	bytes_free(pp);

	return C_OK;
}


static dword_t _ssh_format_pubkey(ssh_t* pssh, byte_t* buf, dword_t max)
{
	dword_t n, total = 0;
	const schar_t* kex_type;

	kex_type = pssh->alg_method[SSH_METHOD_INDEX_KEYSECRET];

	//string	certificate or public key format identifier
	//byte[n]	key / certificate data
	n = a_xslen(kex_type);

	//key type length
	if (total + 4 > max)
		return total;

	if (buf)
	{
		PUT_DWORD_NET(buf, total, n);
	}
	total += 4;

	//key type
	if (total + n > max)
		return total;

	if (buf)
	{
		xmem_copy((void*)(buf + total), (void*)kex_type, n);
	}
	total += n;

	//key exchange mpint set
	if (a_xsicmp(kex_type, SSH_KEYSECRET_RSA) == 0)
	{
		//export mpint e, n
		n = rsa_pubkey_size((rsa_context*)pssh->kex_ctx);
		if (total + n > max)
			return total;

		if (buf)
		{
			rsa_export_pubkey((rsa_context*)pssh->kex_ctx, buf + total, &n, 0);
		}
		total += n;
	}

	return total;
}

static bool_t _ssh_parse_pubkey(ssh_t* pssh, const byte_t* buf, dword_t size)
{
	dword_t n, total = 0;
	schar_t kex_type[RES_LEN] = { 0 };
	byte_t key_data[1024] = { 0 };

	//string	certificate or public key format identifier
	//byte[n]	key / certificate data
	n = GET_DWORD_NET(buf, total);
	total += 4;
	a_xsncpy(kex_type, (schar_t*)(buf + total), n);
	total += n;

	if (a_xsicmp(kex_type, pssh->alg_method[SSH_METHOD_INDEX_KEYSECRET]) != 0)
	{
		return 0;
	}

	if (a_xsicmp(kex_type, SSH_KEYSECRET_RSA) == 0)
	{
		n = size - total;
		//read mpint e and n
		if (C_OK != rsa_import_pubkey((rsa_context*)pssh->kex_ctx, buf + total, n, 0))
			return 0;

		if (C_OK != rsa_check_pubkey((rsa_context*)pssh->kex_ctx))
			return 0;

		//mpint	e
		n = GET_DWORD_NET(buf, total);
		total += 4;
		xmem_copy((void*)key_data, (void*)(buf + total), n);
		total += n;

		//mpint	n
		n = GET_DWORD_NET(buf, total);
		total += 4;
		xmem_copy((void*)key_data, (void*)(buf + total), n);
		total += n;
	}

	return (total == size) ? 1 : 0;
}

static dword_t _ssh_format_pubsig(ssh_t* pssh, byte_t* buf, dword_t max)
{
	dword_t n, total = 0;
	const schar_t* kex_type;

	kex_type = pssh->alg_method[SSH_METHOD_INDEX_KEYSECRET];

	//string	signature format identifier (as specified by the public key / certificate format)
	//byte[n]	signature blob in format specific encoding.
	n = a_xslen(kex_type);

	//key type length
	if (total + 4 > max)
		return total;

	if (buf)
	{
		PUT_DWORD_NET(buf, total, n);
	}
	total += 4;

	//key type
	if (total + n > max)
		return total;

	if (buf)
	{
		xmem_copy((void*)(buf + total), (void*)kex_type, n);
	}
	total += n;

	//kex exchange signature
	if (a_xsicmp(kex_type, SSH_KEYSECRET_RSA) == 0)
	{
		//signature length
		n = pssh->len_S;
		if (total + 4 > max)
			return total;

		if (buf)
		{
			PUT_DWORD_NET(buf, total, n);
		}
		total += 4;

		//signature data
		if (total + n > max)
			return total;

		if (buf)
		{
			xmem_copy((void*)(buf + total), (void*)pssh->kex_S, n);
		}
		total += n;
	}
	
	return total;
}

static bool_t _ssh_parse_pubsig(ssh_t* pssh, const byte_t* buf, dword_t size)
{
	dword_t n, total = 0;
	schar_t sig_type[RES_LEN] = { 0 };

	//string	signature format identifier (as specified by the public key / certificate format)
	//byte[n]	signature blob in format specific encoding.
	n = GET_DWORD_NET(buf, total);
	total += 4;
	a_xsncpy(sig_type, (schar_t*)(buf + total), n);
	total += n;

	if (a_xsicmp(sig_type, pssh->alg_method[SSH_METHOD_INDEX_KEYSECRET]) != 0)
	{
		return 0;
	}

	if (a_xsicmp(sig_type, SSH_KEYSECRET_RSA) == 0)
	{
		//signature size
		n = GET_DWORD_NET(buf, total);
		total += 4;
		//signature data
		pssh->len_S= n;
		pssh->kex_S = (byte_t*)xmem_alloc(pssh->len_S);
		xmem_copy((void*)pssh->kex_S, (void*)(buf + total), n);
		total += n;
	}

	return (total == size) ? 1 : 0;
}

static bool_t _ssh_make_session(ssh_t* pssh)
{
	byte_t num[4];
	dword_t n;
	byte_t K[1024] = { 0 };
	dword_t len_K;
	const schar_t* kex_sec;
	const schar_t* kex_dh;

	sha1_context sha1 = { 0 };
	sha2_context sha2 = { 0 };
	sha4_context sha4 = { 0 };

	const schar_t* snd_ciph = NULL;
	const schar_t* rcv_ciph = NULL;
	const schar_t* snd_hmac = NULL;
	const schar_t* rcv_hmac = NULL;
	dword_t blk_size, key_size, mac_size;

	//calc the K_S first
	if (pssh->type == SSH_TYPE_SERVER)
	{
		pssh->len_KS = _ssh_format_pubkey(pssh, NULL, MAX_LONG);
		pssh->kex_KS = (byte_t*)xmem_alloc(pssh->len_KS);
		_ssh_format_pubkey(pssh, pssh->kex_KS, pssh->len_KS);
	}

	//start kex secret hash after kexinit
	//H = hash(V_C || V_S || I_C || I_S || K_S || e || f || K)
	_ssh_start_kex_secret(pssh);

	//H = hash(V_C || V_S || ...)
	//V_C: client banner
	//V_S: server banner
	n = a_xslen(pssh->kex_VC);
	PUT_DWORD_NET(num, 0, n);
	_ssh_update_kex_secret(pssh, num, 4);
	_ssh_update_kex_secret(pssh, pssh->kex_VC, n);

	n = a_xslen(pssh->kex_VS);
	PUT_DWORD_NET(num, 0, n);
	_ssh_update_kex_secret(pssh, num, 4);
	_ssh_update_kex_secret(pssh, pssh->kex_VS, n);

	//H = hash(... || I_C || I_S || ...)
	//I_C: client keyinit payload
	//I_S: server keyinit payload
	PUT_DWORD_NET(num, 0, pssh->len_IC);
	_ssh_update_kex_secret(pssh, num, 4);
	_ssh_update_kex_secret(pssh, pssh->kex_IC, pssh->len_IC);

	PUT_DWORD_NET(num, 0, pssh->len_IS);
	_ssh_update_kex_secret(pssh, num, 4);
	_ssh_update_kex_secret(pssh, pssh->kex_IS, pssh->len_IS);

	//H = hash(... || K_S || ...)
	//K_S: public key blob
	PUT_DWORD_NET(num, 0, pssh->len_KS);
	_ssh_update_kex_secret(pssh, num, 4);
	_ssh_update_kex_secret(pssh, pssh->kex_KS, pssh->len_KS);

	//H = hash(... || e || ...)
	//e: client mpi blob
	PUT_DWORD_NET(num, 0, pssh->len_E);
	_ssh_update_kex_secret(pssh, num, 4);
	_ssh_update_kex_secret(pssh, pssh->kex_E, pssh->len_E);

	//H = hash(... || f || ...)
	//f: server mpi blob
	PUT_DWORD_NET(num, 0, pssh->len_F);
	_ssh_update_kex_secret(pssh, num, 4);
	_ssh_update_kex_secret(pssh, pssh->kex_F, pssh->len_F);

	//calc the K mpi
	len_K = 0;
	if (C_OK != dhm_calc_secret(&(pssh->kex_dhm), K, &len_K))
	{
		return 0;
	}

	//H = hash(... || K)
	//K: mpi blob
	PUT_DWORD_NET(num, 0, len_K);
	_ssh_update_kex_secret(pssh, num, 4);
	_ssh_update_kex_secret(pssh, K, len_K);

	//final generate the H
	_ssh_finish_kex_secret(pssh);

	kex_sec = pssh->alg_method[SSH_METHOD_INDEX_KEYSECRET];

	//calc the key exchange signature last
	if (pssh->type == SSH_TYPE_SERVER)
	{
		//server generate the signature
		if (a_xscmp(kex_sec, SSH_KEYSECRET_RSA) == 0)
		{
			pssh->len_S= rsa_size((rsa_context*)pssh->kex_ctx);
			pssh->kex_S = (byte_t*)xmem_alloc(pssh->len_S);

			if (C_OK != rsa_pkcs1_sign((rsa_context*)pssh->kex_ctx, RSA_PRIVATE, RSA_SHA1, pssh->len_H, pssh->kex_H, pssh->kex_S))
				return 0;
		}
	}
	else
	{
		//client verify the signature
		if (a_xscmp(kex_sec, SSH_KEYSECRET_RSA) == 0)
		{
			if (C_OK != rsa_pkcs1_verify((rsa_context*)pssh->kex_ctx, RSA_PUBLIC, RSA_SHA1, pssh->len_H, pssh->kex_H, pssh->kex_S))
				return 0;
		}
	}

	kex_dh = pssh->alg_method[SSH_METHOD_INDEX_KEXDH];

	if (pssh->type == SSH_TYPE_SERVER)
	{
		snd_hmac = pssh->alg_method[SSH_METHOD_INDEX_HMACSERVER];
		rcv_hmac = pssh->alg_method[SSH_METHOD_INDEX_HMACCLIENT];

		snd_ciph = pssh->alg_method[SSH_METHOD_INDEX_CIPHSERVER];
		rcv_ciph = pssh->alg_method[SSH_METHOD_INDEX_CIPHCLIENT];
	}
	else
	{
		rcv_hmac = pssh->alg_method[SSH_METHOD_INDEX_HMACSERVER];
		snd_hmac = pssh->alg_method[SSH_METHOD_INDEX_HMACCLIENT];

		rcv_ciph = pssh->alg_method[SSH_METHOD_INDEX_CIPHSERVER];
		snd_ciph = pssh->alg_method[SSH_METHOD_INDEX_CIPHCLIENT];
	}

	//new session id
	xmem_copy((void*)pssh->pid_sess, (void*)pssh->kex_H, pssh->len_H);
	pssh->len_sess = pssh->len_H;

	if (a_xsicmp(kex_dh, SSH_KEXDH_GROUP1_SHA1) == 0 || a_xsicmp(kex_dh, SSH_KEXDH_GROUP14_SHA1) == 0)
	{
		//new crypt IV
		if (pssh->type == SSH_TYPE_SERVER)
		{
			//client->server: HASH(K || H || "A" || session_id)
			sha1_starts(&sha1);
			sha1_update(&sha1, K, len_K);
			sha1_update(&sha1, pssh->kex_H, pssh->len_H);
			sha1_update(&sha1, "A", 1);
			sha1_update(&sha1, pssh->pid_sess, pssh->len_sess);
			sha1_finish(&sha1, pssh->rcv_blk);
			pssh->len_rcv_blk = SSH_HMAC_SHA1_SIZE;

			blk_size = _ssh_ciph_blk_size(rcv_ciph);
			while (pssh->len_rcv_blk < blk_size)
			{
				sha1_starts(&sha1);
				sha1_update(&sha1, K, len_K);
				sha1_update(&sha1, pssh->kex_H, pssh->len_H);
				sha1_update(&sha1, pssh->rcv_blk, pssh->len_rcv_blk);
				sha1_finish(&sha1, pssh->rcv_blk + pssh->len_rcv_blk);

				pssh->len_rcv_blk += SSH_HMAC_SHA1_SIZE;
			}
			pssh->len_rcv_blk = blk_size;

			//server->client: HASH(K || H || "B" || session_id)
			sha1_starts(&sha1);
			sha1_update(&sha1, K, len_K);
			sha1_update(&sha1, pssh->kex_H, pssh->len_H);
			sha1_update(&sha1, "B", 1);
			sha1_update(&sha1, pssh->pid_sess, pssh->len_sess);
			sha1_finish(&sha1, pssh->snd_blk);
			pssh->len_snd_blk = SSH_HMAC_SHA1_SIZE;

			blk_size = _ssh_ciph_blk_size(snd_ciph);
			while (pssh->len_snd_blk < blk_size)
			{
				sha1_starts(&sha1);
				sha1_update(&sha1, K, len_K);
				sha1_update(&sha1, pssh->kex_H, pssh->len_H);
				sha1_update(&sha1, pssh->snd_blk, pssh->len_snd_blk);
				sha1_finish(&sha1, pssh->snd_blk + pssh->len_snd_blk);

				pssh->len_snd_blk += SSH_HMAC_SHA1_SIZE;
			}
			pssh->len_snd_blk = blk_size;
		}
		else
		{
			//client->server: HASH(K || H || "A" || session_id)
			sha1_starts(&sha1);
			sha1_update(&sha1, K, len_K);
			sha1_update(&sha1, pssh->kex_H, pssh->len_H);
			sha1_update(&sha1, "A", 1);
			sha1_update(&sha1, pssh->pid_sess, pssh->len_sess);
			sha1_finish(&sha1, pssh->snd_blk);
			pssh->len_snd_blk = SSH_HMAC_SHA1_SIZE;

			blk_size = _ssh_ciph_blk_size(snd_ciph);
			while (pssh->len_snd_blk < blk_size)
			{
				sha1_starts(&sha1);
				sha1_update(&sha1, K, len_K);
				sha1_update(&sha1, pssh->kex_H, pssh->len_H);
				sha1_update(&sha1, pssh->snd_blk, pssh->len_snd_blk);
				sha1_finish(&sha1, pssh->snd_blk + pssh->len_snd_blk);

				pssh->len_snd_blk += SSH_HMAC_SHA1_SIZE;
			}
			pssh->len_snd_blk = blk_size;

			//server->client: HASH(K || H || "B" || session_id)
			sha1_starts(&sha1);
			sha1_update(&sha1, K, len_K);
			sha1_update(&sha1, pssh->kex_H, pssh->len_H);
			sha1_update(&sha1, "B", 1);
			sha1_update(&sha1, pssh->pid_sess, pssh->len_sess);
			sha1_finish(&sha1, pssh->rcv_blk);
			pssh->len_rcv_blk = SSH_HMAC_SHA1_SIZE;

			blk_size = _ssh_ciph_blk_size(rcv_ciph);
			while (pssh->len_rcv_blk < blk_size)
			{
				sha1_starts(&sha1);
				sha1_update(&sha1, K, len_K);
				sha1_update(&sha1, pssh->kex_H, pssh->len_H);
				sha1_update(&sha1, pssh->rcv_blk, pssh->len_rcv_blk);
				sha1_finish(&sha1, pssh->rcv_blk + pssh->len_rcv_blk);

				pssh->len_rcv_blk += SSH_HMAC_SHA1_SIZE;
			}
			pssh->len_rcv_blk = blk_size;
		}

		//crypt key
		if (pssh->type == SSH_TYPE_SERVER)
		{
			//client->server: HASH(K || H || "C" || session_id)
			sha1_starts(&sha1);
			sha1_update(&sha1, K, len_K);
			sha1_update(&sha1, pssh->kex_H, pssh->len_H);
			sha1_update(&sha1, "C", 1);
			sha1_update(&sha1, pssh->pid_sess, pssh->len_sess);
			sha1_finish(&sha1, pssh->rcv_key);
			pssh->len_rcv_key = SSH_HMAC_SHA1_SIZE;

			key_size = _ssh_ciph_key_size(rcv_ciph);
			while (pssh->len_rcv_key < key_size)
			{
				sha1_starts(&sha1);
				sha1_update(&sha1, K, len_K);
				sha1_update(&sha1, pssh->kex_H, pssh->len_H);
				sha1_update(&sha1, pssh->rcv_key, pssh->len_rcv_key);
				sha1_finish(&sha1, pssh->rcv_key + pssh->len_rcv_key);

				pssh->len_rcv_key += SSH_HMAC_SHA1_SIZE;
			}
			pssh->len_rcv_key = key_size;

			//server->client: HASH(K || H || "D" || session_id)
			sha1_starts(&sha1);
			sha1_update(&sha1, K, len_K);
			sha1_update(&sha1, pssh->kex_H, pssh->len_H);
			sha1_update(&sha1, "D", 1);
			sha1_update(&sha1, pssh->pid_sess, pssh->len_sess);
			sha1_finish(&sha1, pssh->snd_key);
			pssh->len_snd_key = SSH_HMAC_SHA1_SIZE;

			key_size = _ssh_ciph_key_size(snd_ciph);
			while (pssh->len_snd_key < key_size)
			{
				sha1_starts(&sha1);
				sha1_update(&sha1, K, len_K);
				sha1_update(&sha1, pssh->kex_H, pssh->len_H);
				sha1_update(&sha1, pssh->snd_key, pssh->len_snd_key);
				sha1_finish(&sha1, pssh->snd_key + pssh->len_snd_key);

				pssh->len_snd_key += SSH_HMAC_SHA1_SIZE;
			}
			pssh->len_snd_key = key_size;
		}
		else
		{
			//client->server: HASH(K || H || "C" || session_id)
			sha1_starts(&sha1);
			sha1_update(&sha1, K, len_K);
			sha1_update(&sha1, pssh->kex_H, pssh->len_H);
			sha1_update(&sha1, "C", 1);
			sha1_update(&sha1, pssh->pid_sess, pssh->len_sess);
			sha1_finish(&sha1, pssh->snd_key);
			pssh->len_snd_key = SSH_HMAC_SHA1_SIZE;

			key_size = _ssh_ciph_key_size(snd_ciph);
			while (pssh->len_snd_key < key_size)
			{
				sha1_starts(&sha1);
				sha1_update(&sha1, K, len_K);
				sha1_update(&sha1, pssh->kex_H, pssh->len_H);
				sha1_update(&sha1, pssh->snd_key, pssh->len_snd_key);
				sha1_finish(&sha1, pssh->snd_key + pssh->len_snd_key);

				pssh->len_snd_key += SSH_HMAC_SHA1_SIZE;
			}
			pssh->len_snd_key = key_size;

			//server->client: HASH(K || H || "D" || session_id)
			sha1_starts(&sha1);
			sha1_update(&sha1, K, len_K);
			sha1_update(&sha1, pssh->kex_H, pssh->len_H);
			sha1_update(&sha1, "D", 1);
			sha1_update(&sha1, pssh->pid_sess, pssh->len_sess);
			sha1_finish(&sha1, pssh->rcv_key);
			pssh->len_rcv_key = SSH_HMAC_SHA1_SIZE;

			key_size = _ssh_ciph_key_size(rcv_ciph);
			while (pssh->len_rcv_key < key_size)
			{
				sha1_starts(&sha1);
				sha1_update(&sha1, K, len_K);
				sha1_update(&sha1, pssh->kex_H, pssh->len_H);
				sha1_update(&sha1, pssh->rcv_key, pssh->len_rcv_key);
				sha1_finish(&sha1, pssh->rcv_key + pssh->len_rcv_key);

				pssh->len_rcv_key += SSH_HMAC_SHA1_SIZE;
			}
			pssh->len_rcv_key = key_size;
		}

		//mac key
		if (pssh->type == SSH_TYPE_SERVER)
		{
			//client->server: HASH(K || H || "E" || session_id)
			sha1_starts(&sha1);
			sha1_update(&sha1, K, len_K);
			sha1_update(&sha1, pssh->kex_H, pssh->len_H);
			sha1_update(&sha1, "E", 1);
			sha1_update(&sha1, pssh->pid_sess, pssh->len_sess);
			sha1_finish(&sha1, pssh->rcv_mac);
			pssh->len_rcv_mac = SSH_HMAC_SHA1_SIZE;

			mac_size = _ssh_hmac_size(rcv_hmac);
			while (pssh->len_rcv_mac < mac_size)
			{
				sha1_starts(&sha1);
				sha1_update(&sha1, K, len_K);
				sha1_update(&sha1, pssh->kex_H, pssh->len_H);
				sha1_update(&sha1, pssh->rcv_mac, pssh->len_rcv_mac);
				sha1_finish(&sha1, pssh->rcv_mac + pssh->len_rcv_mac);

				pssh->len_rcv_mac += SSH_HMAC_SHA1_SIZE;
			}
			pssh->len_rcv_mac = mac_size;

			//server->client: HASH(K || H || "F" || session_id)
			sha1_starts(&sha1);
			sha1_update(&sha1, K, len_K);
			sha1_update(&sha1, pssh->kex_H, pssh->len_H);
			sha1_update(&sha1, "F", 1);
			sha1_update(&sha1, pssh->pid_sess, pssh->len_sess);
			sha1_finish(&sha1, pssh->snd_mac);
			pssh->len_snd_mac = SSH_HMAC_SHA1_SIZE;

			mac_size = _ssh_hmac_size(snd_hmac);
			while (pssh->len_snd_mac < mac_size)
			{
				sha1_starts(&sha1);
				sha1_update(&sha1, K, len_K);
				sha1_update(&sha1, pssh->kex_H, pssh->len_H);
				sha1_update(&sha1, pssh->snd_mac, pssh->len_snd_mac);
				sha1_finish(&sha1, pssh->snd_mac + pssh->len_snd_mac);

				pssh->len_snd_mac += SSH_HMAC_SHA1_SIZE;
			}
			pssh->len_snd_mac = mac_size;
		}
		else
		{
			//client->server: HASH(K || H || "E" || session_id)
			sha1_starts(&sha1);
			sha1_update(&sha1, K, len_K);
			sha1_update(&sha1, pssh->kex_H, pssh->len_H);
			sha1_update(&sha1, "E", 1);
			sha1_update(&sha1, pssh->pid_sess, pssh->len_sess);
			sha1_finish(&sha1, pssh->snd_mac);
			pssh->len_snd_mac = SSH_HMAC_SHA1_SIZE;

			mac_size = _ssh_hmac_size(snd_hmac);
			while (pssh->len_snd_mac < mac_size)
			{
				sha1_starts(&sha1);
				sha1_update(&sha1, K, len_K);
				sha1_update(&sha1, pssh->kex_H, pssh->len_H);
				sha1_update(&sha1, pssh->snd_mac, pssh->len_snd_mac);
				sha1_finish(&sha1, pssh->snd_mac + pssh->len_snd_mac);

				pssh->len_snd_mac += SSH_HMAC_SHA1_SIZE;
			}
			pssh->len_snd_mac = mac_size;

			//server->client: HASH(K || H || "F" || session_id)
			sha1_starts(&sha1);
			sha1_update(&sha1, K, len_K);
			sha1_update(&sha1, pssh->kex_H, pssh->len_H);
			sha1_update(&sha1, "F", 1);
			sha1_update(&sha1, pssh->pid_sess, pssh->len_sess);
			sha1_finish(&sha1, pssh->rcv_mac);
			pssh->len_rcv_mac = SSH_HMAC_SHA1_SIZE;

			mac_size = _ssh_hmac_size(rcv_hmac);
			while (pssh->len_rcv_mac < mac_size)
			{
				sha1_starts(&sha1);
				sha1_update(&sha1, K, len_K);
				sha1_update(&sha1, pssh->kex_H, pssh->len_H);
				sha1_update(&sha1, pssh->rcv_mac, pssh->len_rcv_mac);
				sha1_finish(&sha1, pssh->rcv_mac + pssh->len_rcv_mac);

				pssh->len_rcv_mac += SSH_HMAC_SHA1_SIZE;
			}
			pssh->len_rcv_mac = mac_size;
		}
	}
	else if (a_xsicmp(kex_dh, SSH_KEXDH_GROUP16_SHA512) == 0 || a_xsicmp(kex_dh, SSH_KEXDH_GROUP18_SHA512) == 0)
	{
		//new crypt IV
		if (pssh->type == SSH_TYPE_SERVER)
		{
			//client->server: HASH(K || H || "A" || session_id)
			sha4_starts(&sha4, 0);
			sha4_update(&sha4, K, len_K);
			sha4_update(&sha4, pssh->kex_H, pssh->len_H);
			sha4_update(&sha4, "A", 1);
			sha4_update(&sha4, pssh->pid_sess, pssh->len_sess);
			sha4_finish(&sha4, pssh->rcv_blk);
			pssh->len_rcv_blk = SSH_HMAC_SHA2_512_SIZE;

			blk_size = _ssh_ciph_blk_size(rcv_ciph);
			while (pssh->len_rcv_blk < blk_size)
			{
				sha4_starts(&sha4, 0);
				sha4_update(&sha4, K, len_K);
				sha4_update(&sha4, pssh->kex_H, pssh->len_H);
				sha4_update(&sha4, pssh->rcv_blk, pssh->len_rcv_blk);
				sha4_finish(&sha4, pssh->rcv_blk + pssh->len_rcv_blk);

				pssh->len_rcv_blk += SSH_HMAC_SHA2_512_SIZE;
			}
			pssh->len_rcv_blk = blk_size;

			//server->client: HASH(K || H || "B" || session_id)
			sha4_starts(&sha4, 0);
			sha4_update(&sha4, K, len_K);
			sha4_update(&sha4, pssh->kex_H, pssh->len_H);
			sha4_update(&sha4, "B", 1);
			sha4_update(&sha4, pssh->pid_sess, pssh->len_sess);
			sha4_finish(&sha4, pssh->snd_blk);
			pssh->len_snd_blk = SSH_HMAC_SHA2_512_SIZE;

			blk_size = _ssh_ciph_blk_size(snd_ciph);
			while (pssh->len_snd_blk < blk_size)
			{
				sha4_starts(&sha4, 0);
				sha4_update(&sha4, K, len_K);
				sha4_update(&sha4, pssh->kex_H, pssh->len_H);
				sha4_update(&sha4, pssh->snd_blk, pssh->len_snd_blk);
				sha4_finish(&sha4, pssh->snd_blk + pssh->len_snd_blk);

				pssh->len_snd_blk += SSH_HMAC_SHA2_512_SIZE;
			}
			pssh->len_snd_blk = blk_size;
		}
		else
		{
			//client->server: HASH(K || H || "A" || session_id)
			sha4_starts(&sha4, 0);
			sha4_update(&sha4, K, len_K);
			sha4_update(&sha4, pssh->kex_H, pssh->len_H);
			sha4_update(&sha4, "A", 1);
			sha4_update(&sha4, pssh->pid_sess, pssh->len_sess);
			sha4_finish(&sha4, pssh->snd_blk);
			pssh->len_snd_blk = SSH_HMAC_SHA2_512_SIZE;

			blk_size = _ssh_ciph_blk_size(snd_ciph);
			while (pssh->len_snd_blk < blk_size)
			{
				sha4_starts(&sha4, 0);
				sha4_update(&sha4, K, len_K);
				sha4_update(&sha4, pssh->kex_H, pssh->len_H);
				sha4_update(&sha4, pssh->snd_blk, pssh->len_snd_blk);
				sha4_finish(&sha4, pssh->snd_blk + pssh->len_snd_blk);

				pssh->len_snd_blk += SSH_HMAC_SHA2_512_SIZE;
			}
			pssh->len_snd_blk = blk_size;

			//server->client: HASH(K || H || "B" || session_id)
			sha4_starts(&sha4, 0);
			sha4_update(&sha4, K, len_K);
			sha4_update(&sha4, pssh->kex_H, pssh->len_H);
			sha4_update(&sha4, "B", 1);
			sha4_update(&sha4, pssh->pid_sess, pssh->len_sess);
			sha4_finish(&sha4, pssh->rcv_blk);
			pssh->len_rcv_blk = SSH_HMAC_SHA2_512_SIZE;

			blk_size = _ssh_ciph_blk_size(rcv_ciph);
			while (pssh->len_rcv_blk < blk_size)
			{
				sha4_starts(&sha4, 0);
				sha4_update(&sha4, K, len_K);
				sha4_update(&sha4, pssh->kex_H, pssh->len_H);
				sha4_update(&sha4, pssh->rcv_blk, pssh->len_rcv_blk);
				sha4_finish(&sha4, pssh->rcv_blk + pssh->len_rcv_blk);

				pssh->len_rcv_blk += SSH_HMAC_SHA2_512_SIZE;
			}
			pssh->len_rcv_blk = blk_size;
		}

		//crypt key
		if (pssh->type == SSH_TYPE_SERVER)
		{
			//client->server: HASH(K || H || "C" || session_id)
			sha4_starts(&sha4, 0);
			sha4_update(&sha4, K, len_K);
			sha4_update(&sha4, pssh->kex_H, pssh->len_H);
			sha4_update(&sha4, "C", 1);
			sha4_update(&sha4, pssh->pid_sess, pssh->len_sess);
			sha4_finish(&sha4, pssh->rcv_key);
			pssh->len_rcv_key = SSH_HMAC_SHA2_512_SIZE;

			key_size = _ssh_ciph_key_size(rcv_ciph);
			while (pssh->len_rcv_key < key_size)
			{
				sha4_starts(&sha4, 0);
				sha4_update(&sha4, K, len_K);
				sha4_update(&sha4, pssh->kex_H, pssh->len_H);
				sha4_update(&sha4, pssh->rcv_key, pssh->len_rcv_key);
				sha4_finish(&sha4, pssh->rcv_key + pssh->len_rcv_key);

				pssh->len_rcv_key += SSH_HMAC_SHA2_512_SIZE;
			}
			pssh->len_rcv_key = key_size;

			//server->client: HASH(K || H || "D" || session_id)
			sha4_starts(&sha4, 0);
			sha4_update(&sha4, K, len_K);
			sha4_update(&sha4, pssh->kex_H, pssh->len_H);
			sha4_update(&sha4, "D", 1);
			sha4_update(&sha4, pssh->pid_sess, pssh->len_sess);
			sha4_finish(&sha4, pssh->snd_key);
			pssh->len_snd_key = SSH_HMAC_SHA2_512_SIZE;

			key_size = _ssh_ciph_key_size(snd_ciph);
			while (pssh->len_snd_key < key_size)
			{
				sha4_starts(&sha4, 0);
				sha4_update(&sha4, K, len_K);
				sha4_update(&sha4, pssh->kex_H, pssh->len_H);
				sha4_update(&sha4, pssh->snd_key, pssh->len_snd_key);
				sha4_finish(&sha4, pssh->snd_key + pssh->len_snd_key);

				pssh->len_snd_key += SSH_HMAC_SHA2_512_SIZE;
			}
			pssh->len_snd_key = key_size;
		}
		else
		{
			//client->server: HASH(K || H || "C" || session_id)
			sha4_starts(&sha4, 0);
			sha4_update(&sha4, K, len_K);
			sha4_update(&sha4, pssh->kex_H, pssh->len_H);
			sha4_update(&sha4, "C", 1);
			sha4_update(&sha4, pssh->pid_sess, pssh->len_sess);
			sha4_finish(&sha4, pssh->snd_key);
			pssh->len_snd_key = SSH_HMAC_SHA2_512_SIZE;

			key_size = _ssh_ciph_key_size(snd_ciph);
			while (pssh->len_snd_key < key_size)
			{
				sha4_starts(&sha4, 0);
				sha4_update(&sha4, K, len_K);
				sha4_update(&sha4, pssh->kex_H, pssh->len_H);
				sha4_update(&sha4, pssh->snd_key, pssh->len_snd_key);
				sha4_finish(&sha4, pssh->snd_key + pssh->len_snd_key);

				pssh->len_snd_key += SSH_HMAC_SHA2_512_SIZE;
			}
			pssh->len_snd_key = key_size;

			//server->client: HASH(K || H || "D" || session_id)
			sha4_starts(&sha4, 0);
			sha4_update(&sha4, K, len_K);
			sha4_update(&sha4, pssh->kex_H, pssh->len_H);
			sha4_update(&sha4, "D", 1);
			sha4_update(&sha4, pssh->pid_sess, pssh->len_sess);
			sha4_finish(&sha4, pssh->rcv_key);
			pssh->len_rcv_key = SSH_HMAC_SHA2_512_SIZE;

			key_size = _ssh_ciph_key_size(rcv_ciph);
			while (pssh->len_rcv_key < key_size)
			{
				sha4_starts(&sha4, 0);
				sha4_update(&sha4, K, len_K);
				sha4_update(&sha4, pssh->kex_H, pssh->len_H);
				sha4_update(&sha4, pssh->rcv_key, pssh->len_rcv_key);
				sha4_finish(&sha4, pssh->rcv_key + pssh->len_rcv_key);

				pssh->len_rcv_key += SSH_HMAC_SHA2_512_SIZE;
			}
			pssh->len_rcv_key = key_size;
		}

		//mac key
		if (pssh->type == SSH_TYPE_SERVER)
		{
			//client->server: HASH(K || H || "E" || session_id)
			sha4_starts(&sha4, 0);
			sha4_update(&sha4, K, len_K);
			sha4_update(&sha4, pssh->kex_H, pssh->len_H);
			sha4_update(&sha4, "E", 1);
			sha4_update(&sha4, pssh->pid_sess, pssh->len_sess);
			sha4_finish(&sha4, pssh->rcv_mac);
			pssh->len_rcv_mac = SSH_HMAC_SHA2_512_SIZE;

			mac_size = _ssh_hmac_size(rcv_hmac);
			while (pssh->len_rcv_mac < mac_size)
			{
				sha4_starts(&sha4, 0);
				sha4_update(&sha4, K, len_K);
				sha4_update(&sha4, pssh->kex_H, pssh->len_H);
				sha4_update(&sha4, pssh->rcv_mac, pssh->len_rcv_mac);
				sha4_finish(&sha4, pssh->rcv_mac + pssh->len_rcv_mac);

				pssh->len_rcv_mac += SSH_HMAC_SHA2_512_SIZE;
			}
			pssh->len_rcv_mac = mac_size;

			//server->client: HASH(K || H || "F" || session_id)
			sha4_starts(&sha4, 0);
			sha4_update(&sha4, K, len_K);
			sha4_update(&sha4, pssh->kex_H, pssh->len_H);
			sha4_update(&sha4, "F", 1);
			sha4_update(&sha4, pssh->pid_sess, pssh->len_sess);
			sha4_finish(&sha4, pssh->snd_mac);
			pssh->len_snd_mac = SSH_HMAC_SHA2_512_SIZE;

			mac_size = _ssh_hmac_size(snd_hmac);
			while (pssh->len_snd_mac < mac_size)
			{
				sha4_starts(&sha4, 0);
				sha4_update(&sha4, K, len_K);
				sha4_update(&sha4, pssh->kex_H, pssh->len_H);
				sha4_update(&sha4, pssh->snd_mac, pssh->len_snd_mac);
				sha4_finish(&sha4, pssh->snd_mac + pssh->len_snd_mac);

				pssh->len_snd_mac += SSH_HMAC_SHA2_512_SIZE;
			}
			pssh->len_snd_mac = mac_size;
		}
		else
		{
			//client->server: HASH(K || H || "E" || session_id)
			sha4_starts(&sha4, 0);
			sha4_update(&sha4, K, len_K);
			sha4_update(&sha4, pssh->kex_H, pssh->len_H);
			sha4_update(&sha4, "E", 1);
			sha4_update(&sha4, pssh->pid_sess, pssh->len_sess);
			sha4_finish(&sha4, pssh->snd_mac);
			pssh->len_snd_mac = SSH_HMAC_SHA2_512_SIZE;

			mac_size = _ssh_hmac_size(snd_hmac);
			while (pssh->len_snd_mac < mac_size)
			{
				sha4_starts(&sha4, 0);
				sha4_update(&sha4, K, len_K);
				sha4_update(&sha4, pssh->kex_H, pssh->len_H);
				sha4_update(&sha4, pssh->snd_mac, pssh->len_snd_mac);
				sha4_finish(&sha4, pssh->snd_mac + pssh->len_snd_mac);

				pssh->len_snd_mac += SSH_HMAC_SHA2_512_SIZE;
			}
			pssh->len_snd_mac = mac_size;

			//server->client: HASH(K || H || "F" || session_id)
			sha4_starts(&sha4, 0);
			sha4_update(&sha4, K, len_K);
			sha4_update(&sha4, pssh->kex_H, pssh->len_H);
			sha4_update(&sha4, "F", 1);
			sha4_update(&sha4, pssh->pid_sess, pssh->len_sess);
			sha4_finish(&sha4, pssh->rcv_mac);
			pssh->len_rcv_mac = SSH_HMAC_SHA2_512_SIZE;

			mac_size = _ssh_hmac_size(rcv_hmac);
			while (pssh->len_rcv_mac < mac_size)
			{
				sha4_starts(&sha4, 0);
				sha4_update(&sha4, K, len_K);
				sha4_update(&sha4, pssh->kex_H, pssh->len_H);
				sha4_update(&sha4, pssh->rcv_mac, pssh->len_rcv_mac);
				sha4_finish(&sha4, pssh->rcv_mac + pssh->len_rcv_mac);

				pssh->len_rcv_mac += SSH_HMAC_SHA2_512_SIZE;
			}
			pssh->len_rcv_mac = mac_size;
		}
	}

	//new crypt context
	if (a_xsicmp(snd_ciph, SSH_CIPH_ARC4) == 0 || a_xsicmp(snd_ciph, SSH_CIPH_ARC4_128) == 0 || a_xsicmp(snd_ciph, SSH_CIPH_ARC4_256) == 0)
	{
		pssh->snd_ciph = xmem_alloc(sizeof(arc4_context));
		arc4_setup((arc4_context*)pssh->snd_ciph, pssh->snd_key, pssh->len_snd_key);
	}
	else if (a_xsicmp(snd_ciph, SSH_CIPH_AES128_CBC) == 0)
	{
		pssh->snd_ciph = xmem_alloc(sizeof(aes_context));
		aes_setkey_enc((aes_context*)pssh->snd_ciph, pssh->snd_key, pssh->len_snd_key);
	}
	else if (a_xsicmp(snd_ciph, SSH_CIPH_AES192_CBC) == 0)
	{
		pssh->snd_ciph = xmem_alloc(sizeof(aes_context));
		aes_setkey_enc((aes_context*)pssh->snd_ciph, pssh->snd_key, pssh->len_snd_key);
	}
	else if (a_xsicmp(snd_ciph, SSH_CIPH_AES256_CBC) == 0)
	{
		pssh->snd_ciph = xmem_alloc(sizeof(aes_context));
		aes_setkey_enc((aes_context*)pssh->snd_ciph, pssh->snd_key, pssh->len_snd_key);
	}
	else if (a_xsicmp(snd_ciph, SSH_CIPH_3DES_CBC) == 0)
	{
		pssh->snd_ciph = xmem_alloc(sizeof(des3_context));
		des3_set3key_enc((des3_context*)pssh->snd_ciph, pssh->snd_key);
	}

	if (a_xsicmp(rcv_ciph, SSH_CIPH_ARC4) == 0 || a_xsicmp(rcv_ciph, SSH_CIPH_ARC4_128) == 0 || a_xsicmp(rcv_ciph, SSH_CIPH_ARC4_256) == 0)
	{
		pssh->rcv_ciph = xmem_alloc(sizeof(arc4_context));
		arc4_setup((arc4_context*)pssh->rcv_ciph, pssh->rcv_key, pssh->len_rcv_key);
	}
	else if (a_xsicmp(rcv_ciph, SSH_CIPH_AES128_CBC) == 0)
	{
		pssh->rcv_ciph = xmem_alloc(sizeof(aes_context));
		aes_setkey_dec((aes_context*)pssh->rcv_ciph, pssh->rcv_key, pssh->len_rcv_key);
	}
	else if (a_xsicmp(rcv_ciph, SSH_CIPH_AES192_CBC) == 0)
	{
		pssh->rcv_ciph = xmem_alloc(sizeof(aes_context));
		aes_setkey_dec((aes_context*)pssh->rcv_ciph, pssh->rcv_key, pssh->len_rcv_key);
	}
	else if (a_xsicmp(rcv_ciph, SSH_CIPH_AES256_CBC) == 0)
	{
		pssh->rcv_ciph = xmem_alloc(sizeof(aes_context));
		aes_setkey_dec((aes_context*)pssh->rcv_ciph, pssh->rcv_key, pssh->len_rcv_key);
	}
	else if (a_xsicmp(rcv_ciph, SSH_CIPH_3DES_CBC) == 0)
	{
		pssh->rcv_ciph = xmem_alloc(sizeof(des3_context));
		des3_set3key_dec((des3_context*)pssh->rcv_ciph, pssh->rcv_key);
	}

	//new hmac context
	if (a_xsicmp(snd_hmac, SSH_HMAC_SHA1) == 0)
	{
		pssh->snd_hmac = xmem_alloc(sizeof(sha1_context));
	}
	else if (a_xsicmp(snd_hmac, SSH_HMAC_SHA2_256) == 0)
	{
		pssh->snd_hmac = xmem_alloc(sizeof(sha2_context));
	}
	else if (a_xsicmp(snd_hmac, SSH_HMAC_SHA2_512) == 0)
	{
		pssh->snd_hmac = xmem_alloc(sizeof(sha4_context));
	}

	if (a_xsicmp(rcv_hmac, SSH_HMAC_SHA1) == 0)
	{
		pssh->rcv_hmac = xmem_alloc(sizeof(sha1_context));
	}
	else if (a_xsicmp(rcv_hmac, SSH_HMAC_SHA2_256) == 0)
	{
		pssh->rcv_hmac = xmem_alloc(sizeof(sha2_context));
	}
	else if (a_xsicmp(rcv_hmac, SSH_HMAC_SHA2_512) == 0)
	{
		pssh->rcv_hmac = xmem_alloc(sizeof(sha4_context));
	}

	return 1;
}

static dword_t _ssh_format_dhreply(ssh_t* pssh, byte_t* buf, dword_t max)
{
	dword_t n, total = 0;

	//0: packet type
	if (total + 1 > max)
		return total;

	if (buf)
	{
		PUT_BYTE(buf, total, SSH2_MSG_KEXDH_REPLY);
	}
	total++;

	n = pssh->len_KS;

	//1~4: K_S block size
	if (total + 4 > max)
		return total;

	if (buf)
	{
		PUT_DWORD_NET(buf, total, n);
	}
	total += 4;

	//5~n: server public host key and certificates (K_S)
	if (total + n > max)
		return total;

	if (buf)
	{
		xmem_copy((void*)(buf + total), (void*)pssh->kex_KS, n);
	}
	total += n;

	n = pssh->len_F;

	//mpint f size
	if (total + 4 > max)
		return total;

	if (buf)
	{
		PUT_DWORD_NET(buf, total, n);
	}
	total += 4;

	//mpint f data
	if (total + n > max)
		return total;

	if (buf)
	{
		xmem_copy((void*)(buf + total), (void*)pssh->kex_F, pssh->len_F);
	}
	total += n;

	n = _ssh_format_pubsig(pssh, NULL, MAX_LONG);

	//signature of H size
	if (total + 4 > max)
		return total;

	if (buf)
	{
		PUT_DWORD_NET(buf, total, n);
	}
	total += 4;

	//signature of H data
	if (total + n > max)
		return total;

	if (buf)
	{
		_ssh_format_pubsig(pssh, buf + total, n);
	}
	total += n;

	return total;
}

static bool_t _ssh_parse_dhreply(ssh_t* pssh, const byte_t* buf, dword_t len)
{
	byte_t msg;
	dword_t n, total = 0;

	//0: packet type
	if (total + 1 > len)
	{
		set_last_error(_T("_ssh_parse_dhinit"), _T("invalid KEXDHINIT pakcet size"), -1);
		return 0;
	}
	msg = GET_BYTE(buf, total);
	total += 1;

	if (msg != SSH2_MSG_KEXDH_REPLY)
	{
		set_last_error(_T("_ssh_parse_dhinit"), _T("invalid KEXDHINIT message type"), -1);
		return 0;
	}

	//1~4: K_S block size
	if (total + 4 > len)
	{
		set_last_error(_T("_ssh_parse_dhinit"), _T("invalid KEXDHINIT pakcet size"), -1);
		return 0;
	}
	n = GET_DWORD_NET(buf, total);
	total += 4;

	//5~n: server public host key and certificates (K_S)
	pssh->len_KS = n;
	pssh->kex_KS = (byte_t*)xmem_alloc(pssh->len_KS);
	xmem_copy((void*)pssh->kex_KS, (void*)(buf + total), n);

	if (!_ssh_parse_pubkey(pssh, buf + total, n))
	{
		return 0;
	}
	total += n;

	//mpint f size
	if (total + 4 > len)
	{
		set_last_error(_T("_ssh_parse_dhinit"), _T("invalid KEXDHINIT pakcet size"), -1);
		return 0;
	}
	n = GET_DWORD_NET(buf, total);
	total += 4;

	//mpint f data
	pssh->len_F = n;
	pssh->kex_F = (byte_t*)xmem_alloc(pssh->len_F);
	xmem_copy((void*)pssh->kex_F, (void*)(buf + total), n);

	if (C_OK != dhm_read_public(&(pssh->kex_dhm), buf + total, n))
	{
		set_last_error(_T("_ssh_parse_dhinit"), _T("invalid KEXDHINIT mpint f"), -1);
		return 0;
	}
	total += n;

	//signature of H size
	if (total + 4 > len)
	{
		set_last_error(_T("_ssh_parse_dhinit"), _T("invalid KEXDHINIT pakcet size"), -1);
		return 0;
	}
	n = GET_DWORD_NET(buf, total);
	total += 4;

	//signature of H
	if (!_ssh_parse_pubsig(pssh, buf + total, n))
	{
		return 0;
	}
	total += n;

	return 1;
}

static int _ssh_send_dhreply(ssh_t* pssh)
{
	byte_t* payload;
	dword_t n;

	XDL_ASSERT(pssh->type == SSH_TYPE_SERVER);

	//generate server side session id
	if (!_ssh_make_session(pssh))
		return C_ERR;

	n = _ssh_format_dhreply(pssh, NULL, MAX_LONG);

	payload = (byte_t*)xmem_alloc(n);

	n = _ssh_format_dhreply(pssh, payload, n);

	if (C_OK != _ssh_write_packet(pssh, payload, n))
	{
		xmem_free(payload);

		return C_ERR;
	}

	xmem_free(payload);

	return C_OK;
}

static int _ssh_recv_dhreply(ssh_t* pssh)
{
	byte_t** pp = NULL;
	dword_t n = 0;
	byte_t pkt = 0;

	XDL_ASSERT(pssh->type == SSH_TYPE_CLIENT);

	pp = bytes_alloc();

	while (1)
	{
		if (C_OK != _ssh_read_packet(pssh, pp, &n))
		{
			bytes_free(pp);

			return C_ERR;
		}

		pkt = _ssh_packet_msg(*pp, n);
		if (pkt != SSH2_MSG_IGNORE && pkt != SSH2_MSG_UNIMPLEMENTED && pkt != SSH2_MSG_DEBUG)
			break;
	}

	if(!_ssh_parse_dhreply(pssh, *pp, n))
	{
		bytes_free(pp);

		return C_ERR;
	}

	bytes_free(pp);

	//generate client session id
	if (!_ssh_make_session(pssh))
		return C_ERR;

	return C_OK;
}

static int _ssh_send_newkeys(ssh_t* pssh)
{
	byte_t payload[1];
	dword_t n;

	n = 1;
	PUT_BYTE(payload, 0, SSH2_MSG_NEWKEYS);

	if (C_OK != _ssh_write_packet(pssh, payload, n))
	{
		return C_ERR;
	}

	return C_OK;
}

static int _ssh_recv_newkeys(ssh_t* pssh)
{
	byte_t** pp = NULL;
	dword_t n = 0;
	byte_t pkt = 0;

	pp = bytes_alloc();

	while (1)
	{
		if (C_OK != _ssh_read_packet(pssh, pp, &n))
		{
			bytes_free(pp);

			return C_ERR;
		}

		pkt = _ssh_packet_msg(*pp, n);
		if (pkt != SSH2_MSG_IGNORE && pkt != SSH2_MSG_UNIMPLEMENTED && pkt != SSH2_MSG_DEBUG)
			break;
	}

	bytes_free(pp);

	return (pkt == SSH2_MSG_NEWKEYS)? C_OK : C_ERR;
}

static bool_t _ssh_handshake_server(ssh_t* pssh)
{
	while (pssh->state != SSH_SERVER_STATE_ERROR && pssh->state != SSH_SERVER_STATE_OK)
	{
		switch (pssh->state)
		{
		case SSH_SERVER_STATE_INIT:
			pssh->state = SSH_SERVER_STATE_BANNER;
			break;
		case SSH_SERVER_STATE_BANNER:
			if (C_OK != _ssh_send_banner(pssh))
			{
				pssh->state = SSH_SERVER_STATE_ERROR;
				break;
			}

			if (C_OK != _ssh_recv_banner(pssh))
			{
				pssh->state = SSH_SERVER_STATE_ERROR;
				break;
			}
			pssh->state = SSH_SERVER_STATE_KEXINIT;
			break;
		case SSH_SERVER_STATE_KEXINIT:
			if (C_OK != _ssh_recv_kexinit(pssh))
			{
				pssh->state = SSH_SERVER_STATE_ERROR;
				break;
			}
			if (C_OK != _ssh_send_kexinit(pssh))
			{
				pssh->state = SSH_SERVER_STATE_ERROR;
				break;
			}
			pssh->state = SSH_SERVER_STATE_DHREPLY;
			break;
		case SSH_SERVER_STATE_DHREPLY:
			if (C_OK != _ssh_recv_dhinit(pssh))
			{
				pssh->state = SSH_SERVER_STATE_ERROR;
				break;
			}

			if (C_OK != _ssh_send_dhreply(pssh))
			{
				pssh->state = SSH_SERVER_STATE_ERROR;
				break;
			}

			pssh->state = SSH_SERVER_STATE_NEWKEYS;
			break;
		case SSH_SERVER_STATE_NEWKEYS:
			if (C_OK != _ssh_recv_newkeys(pssh))
			{
				pssh->state = SSH_SERVER_STATE_ERROR;
				break;
			}

			if (C_OK != _ssh_send_newkeys(pssh))
			{
				pssh->state = SSH_SERVER_STATE_ERROR;
				break;
			}

			pssh->state = SSH_SERVER_STATE_OK;
			break;
		}

		if (pssh->state == SSH_CLIENT_STATE_ERROR)
		{
			XDL_TRACE_LAST;
		}
	}

	return (pssh->state == SSH_SERVER_STATE_OK)? 1 : 0;
}

static bool_t _ssh_handshake_client(ssh_t* pssh)
{
	while (pssh->state != SSH_CLIENT_STATE_ERROR && pssh->state != SSH_CLIENT_STATE_OK)
	{
		switch (pssh->state)
		{
		case SSH_CLIENT_STATE_INIT:
			pssh->state = SSH_CLIENT_STATE_BANNER;
			break;
		case SSH_CLIENT_STATE_BANNER:
			if (C_OK != _ssh_recv_banner(pssh))
			{
				pssh->state = SSH_CLIENT_STATE_ERROR;
				break;
			}

			if (C_OK != _ssh_send_banner(pssh))
			{
				pssh->state = SSH_CLIENT_STATE_ERROR;
				break;
			}
			pssh->state = SSH_CLIENT_STATE_KEXINIT;
			break;
		case SSH_CLIENT_STATE_KEXINIT:
			if (C_OK != _ssh_send_kexinit(pssh))
			{
				pssh->state = SSH_CLIENT_STATE_ERROR;
				break;
			}

			if (C_OK != _ssh_recv_kexinit(pssh))
			{
				pssh->state = SSH_CLIENT_STATE_ERROR;
				break;
			}

			pssh->state = SSH_CLIENT_STATE_DHINIT;
			break;
		case SSH_CLIENT_STATE_DHINIT:
			if (C_OK != _ssh_send_dhinit(pssh))
			{
				pssh->state = SSH_CLIENT_STATE_ERROR;
				break;
			}

			if (C_OK != _ssh_recv_dhreply(pssh))
			{
				pssh->state = SSH_CLIENT_STATE_ERROR;
				break;
			}

			pssh->state = SSH_CLIENT_STATE_NEWKEYS;
			break;
		case SSH_CLIENT_STATE_NEWKEYS:
			if (C_OK != _ssh_send_newkeys(pssh))
			{
				pssh->state = SSH_CLIENT_STATE_ERROR;
				break;
			}

			if (C_OK != _ssh_recv_newkeys(pssh))
			{
				pssh->state = SSH_CLIENT_STATE_ERROR;
				break;
			}

			pssh->state = SSH_CLIENT_STATE_OK;
			break;
		}

		if (pssh->state == SSH_CLIENT_STATE_ERROR)
		{
			XDL_TRACE_LAST;
		}
	}

	return (pssh->state == SSH_CLIENT_STATE_OK) ? 1 : 0;
}

/*********************************************************************************************************/
static bool_t _ssh_read_data(ssh_t* pssh, byte_t* buf, int* need)
{
	byte_t** pbuf = NULL;
	dword_t size = 0;

	if (!(*need))
		return 1;

	if (pssh->rcv_pdv_pop == pssh->rcv_pdv_len)
	{
		pbuf = bytes_alloc();

		if (C_OK != _ssh_read_packet(pssh, pbuf, &size))
		{
			return 0;
		}

		xmem_copy((void*)pssh->rcv_pdv, (void*)(*pbuf), size);
		pssh->rcv_pdv_len = size;
		pssh->rcv_pdv_pop = 0;

		bytes_free(pbuf);
		pbuf = NULL;
	}

	*need = (*need + pssh->rcv_pdv_pop < pssh->rcv_pdv_len) ? (*need) : (pssh->rcv_pdv_len - pssh->rcv_pdv_pop);

	xmem_copy(buf, pssh->rcv_pdv + pssh->rcv_pdv_pop, *need);
	pssh->rcv_pdv_pop += (*need);

	return 1;
}

static bool_t _ssh_write_data(ssh_t* pssh, byte_t* buf, int* need)
{
	if (!(*need))
		return 1;

	*need = (*need + pssh->snd_pdv_pop < SSH_PDV_SIZE) ? (*need) : (SSH_PDV_SIZE - pssh->snd_pdv_pop);

	xmem_copy(pssh->snd_pdv + pssh->snd_pdv_pop, buf, *need);
	pssh->snd_pdv_pop += (*need);

	if (pssh->snd_pdv_pop == SSH_PDV_SIZE)
	{
		pssh->snd_pdv_len = pssh->snd_pdv_pop;

		if (C_OK != _ssh_write_packet(pssh, pssh->snd_pdv, pssh->snd_pdv_len))
			return 0;

		pssh->snd_pdv_len = SSH_PDV_SIZE;
		pssh->snd_pdv_pop = 0;
	}

	return 1;
}

static bool_t _ssh_flush_data(ssh_t* pssh)
{
	if (!pssh->snd_pdv_pop)
		return 1;

	pssh->snd_pdv_len = pssh->snd_pdv_pop;

	if (C_OK != _ssh_write_packet(pssh, pssh->snd_pdv, pssh->snd_pdv_len))
		return 0;

	pssh->snd_pdv_len = SSH_PDV_SIZE;
	pssh->snd_pdv_pop = 0;

	return 1;
}

/*********************************************************************************************************/

xhand_t xssh_cli(unsigned short port, const tchar_t* addr)
{
	ssh_t* pso = NULL;
	xhand_t tcp;

	tcp = xtcp_cli(port, addr);
	if (!tcp)
		return NULL;

	pso = (ssh_t*)xmem_alloc(sizeof(ssh_t));
	pso->head.tag = _HANDLE_SSH;

	pso->type = SSH_TYPE_CLIENT;

	pso->pif = (if_bio_t*)xmem_alloc(sizeof(if_bio_t));
	get_bio_interface(tcp, pso->pif);

	_ssh_init(pso);

	return &pso->head;
}

xhand_t xssh_srv(res_file_t so)
{
	ssh_t* pso = NULL;
	xhand_t tcp;

	tcp = xtcp_srv(so);
	if (!tcp)
		return NULL;

	pso = (ssh_t*)xmem_alloc(sizeof(ssh_t));
	pso->head.tag = _HANDLE_SSH;

	pso->type = SSH_TYPE_SERVER;

	pso->pif = (if_bio_t*)xmem_alloc(sizeof(if_bio_t));
	get_bio_interface(tcp, pso->pif);

	_ssh_init(pso);

	return &pso->head;
}

void  xssh_close(xhand_t ssh)
{
	ssh_t* pso = TypePtrFromHead(ssh_t, ssh);

	XDL_ASSERT(ssh && ssh->tag == _HANDLE_SSH);

	XDL_ASSERT(pso->type == SSH_TYPE_CLIENT || pso->type == SSH_TYPE_SERVER);

	if (pso->pif)
		xtcp_close(pso->pif->fd);

	_ssh_uninit(pso);

	if (pso->pif)
		xmem_free(pso->pif);

	xmem_free(pso);
}

res_file_t xssh_socket(xhand_t ssh)
{
	ssh_t* pso = TypePtrFromHead(ssh_t, ssh);

	XDL_ASSERT(ssh && ssh->tag == _HANDLE_SSH);

	return (pso->pif) ? xtcp_socket(pso->pif->fd) : INVALID_FILE;
}

int xssh_type(xhand_t ssh)
{
	ssh_t* pso = TypePtrFromHead(ssh_t, ssh);

	XDL_ASSERT(ssh && ssh->tag == _HANDLE_SSH);

	return pso->type;
}

bool_t xssh_write(xhand_t ssh, const byte_t* buf, dword_t* pb)
{
	ssh_t* pso = TypePtrFromHead(ssh_t, ssh);
	int bys, pos;

	XDL_ASSERT(ssh && ssh->tag == _HANDLE_SSH);

	if (pso->type == SSH_TYPE_CLIENT && !_ssh_handshake_client(pso))
	{
		return 0;
	}

	if (pso->type == SSH_TYPE_SERVER && !_ssh_handshake_server(pso))
	{
		return 0;
	}

	pos = 0;
	while (pos < (int)(*pb))
	{
		bys = *pb - pos;
		if (!_ssh_write_data(pso, buf + pos, &bys))
			break;

		if (!bys)
			break;

		pos += bys;
	}

	return (pos == (int)(*pb)) ? 1 : 0;
}

bool_t xssh_flush(xhand_t ssh)
{
	ssh_t* pso = TypePtrFromHead(ssh_t, ssh);

	XDL_ASSERT(ssh && ssh->tag == _HANDLE_SSH);

	return _ssh_flush_data(pso);
}

bool_t xssh_read(xhand_t ssh, byte_t* buf, dword_t* pb)
{
	ssh_t* pso = TypePtrFromHead(ssh_t, ssh);
	int bys, pos;
	bool_t rt = 1;

	XDL_ASSERT(ssh && ssh->tag == _HANDLE_SSH);

	if (pso->type == SSH_TYPE_CLIENT && !_ssh_handshake_client(pso))
	{
		return 0;
	}

	if (pso->type == SSH_TYPE_SERVER && !_ssh_handshake_server(pso))
	{
		return 0;
	}

	pos = 0;
	while (pos < (int)(*pb))
	{
		bys = *pb - pos;
		rt = _ssh_read_data(pso, buf + pos, &bys);
		if (!rt)
			break;

		if (!bys)
			break;

		pos += bys;
	}

	*pb = pos;

	return rt;
}

unsigned short xssh_addr_port(xhand_t ssh, tchar_t* addr)
{
	ssh_t* pso = TypePtrFromHead(ssh_t, ssh);
	net_addr_t na = { 0 };
	unsigned short port;

	XDL_ASSERT(ssh && ssh->tag == _HANDLE_SSH);

	socket_addr(xssh_socket(ssh), &na);
	conv_addr(&na, &port, addr);

	return port;
}

unsigned short xssh_peer_port(xhand_t ssh, tchar_t* addr)
{
	ssh_t* pso = TypePtrFromHead(ssh_t, ssh);
	net_addr_t na = { 0 };
	unsigned short port;

	XDL_ASSERT(ssh && ssh->tag == _HANDLE_SSH);

	socket_peer(xssh_socket(ssh), &na);
	conv_addr(&na, &port, addr);

	return port;
}

bool_t xssh_setopt(xhand_t ssh, int oid, void* opt, int len)
{
	ssh_t* pso = TypePtrFromHead(ssh_t, ssh);

	XDL_ASSERT(ssh && ssh->tag == _HANDLE_SSH);

	switch (oid)
	{
	case SOCK_OPTION_SNDBUF:
		socket_set_sndbuf(xssh_socket(ssh), *(int*)opt);
		return 1;
	case SOCK_OPTION_RCVBUF:
		socket_set_rcvbuf(xssh_socket(ssh), *(int*)opt);
		return 1;
	case SOCK_OPTION_NONBLK:
		socket_set_nonblk(xssh_socket(ssh), *(bool_t*)opt);
		return 1;
	}

	return 0;
}

#endif //XDK_SUPPORT_SOCK
