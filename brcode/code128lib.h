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

#ifndef __CODE128LIB_H__
#define __CODE128LIB_H__

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _code128param {
	char *inText;
	int lenText;
    char *outBytes;
    int lenBytes;
    int codeColumns;
    int error;
} code128param;

void code128exec(code128param* p);
void code128init(code128param* p);
void code128free(code128param* p);

#ifdef __cplusplus
}
#endif

#endif
