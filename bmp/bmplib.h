/*
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MPL as stated above or under the terms of the GNU
 * Library General Public License as published by the Free Software Foundation;
 * either version 2 of the License, or any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Library general Public License for more
 * details.
 */

#ifndef __BMPLIB_H__
#define __BMPLIB_H__


typedef struct _bmp_file_t{
	/*file info header*/
	unsigned short flag; /*file flag*/
	unsigned long fsize; /*file size*/
	unsigned short rev1;
	unsigned short rev2;
	unsigned long offset; /*image bytes position*/
}bmp_file_t;

typedef struct _bmp_info_t{
	/*bmp info header*/
	unsigned long isize; /*bitmap info struct size, set to 40 bytes*/
	unsigned long width; /*bitmap cols*/
	unsigned long height; /*bitmap rows*/
	unsigned short planes; /*number of planes for the target device, set to 1*/
	unsigned short clrbits; /*number of bits-per-pixel*/
	unsigned long compress; /*type of compression, set to 0*/
	unsigned long bytes; /*image bytes*/
	unsigned long xpelsperm; /*the horizontal resolution, in pixels-per-meter*/
	unsigned long ypelsperm; /*the vertical resolution, in pixels-per-meter*/
	unsigned long clrused; /*the number of color indexes in the color table*/
	unsigned long clrimport; /*the number of color indexes that are required for displaying the bitmap*/
}bmp_info_t;

#define BMP_FLAG		0x4d42 /*"B","M"*/
#define BMP_FILEHEADER_SIZE		14
#define BMP_INFOHEADER_SIZE		40
#define BMP_RGBQUAD_SIZE		4

#define BMPFILEHEADERPTR(p)			((unsigned char*)p)
#define BMPINFOHEADERPTR(p)			((unsigned char*)p + BMP_FILEHEADER_SIZE)
#define BMPQUADHEADERPTR(p)			((unsigned char*)p + BMP_FILEHEADER_SIZE + BMP_INFOHEADER_SIZE)

#define BMP_FILL_QUAD(p, a, r, g, b) do{PUT_BYTE(p, 0, b);PUT_BYTE(p, 1, g);PUT_BYTE(p, 2, r);PUT_BYTE(p, 3, a);}while(0);
#define BMP_FILL_RGB(p, r, g, b) do{PUT_BYTE(p, 0, b);PUT_BYTE(p, 1, g);PUT_BYTE(p, 2, r);}while(0);

#ifdef __cplusplus
	extern "C" {
#endif

	extern unsigned long bmp_set_header(const bmp_file_t* pbf, const bmp_info_t* pbi, unsigned char* buf, unsigned long max);

	extern unsigned long bmp_get_header(bmp_file_t* pbf, bmp_info_t* pbi, const unsigned char* src, unsigned long len);

	extern unsigned long bmp_fill_quad(const bmp_info_t* pbi, unsigned char* buf, unsigned long max);

#ifdef __cplusplus
}
#endif

#endif
