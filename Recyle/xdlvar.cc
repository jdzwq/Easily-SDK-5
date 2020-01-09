/*************************************************************
	EasySoft xdl v4.0

	(c) 2003-2009 EasySoft Corporation.  All Rights Reserved.

	@doc variant string

	@module	varingstr.c | variant string implement file

	@devnote 张文权 2003.01 - 2007.12 v3.0
	@devnote 张文权 2008.01 - 2009.12 v4.0
*************************************************************/

/************************************************************
*************************************************************/
#include "xdlvar.h"
#include "xdlmem.h"
#include "xdlstr.h"

typedef struct _varstr_t{
	int size;
	int inc;
	int count;
	union{
	schar_t* astr;
	wchar_t* wstr;
	};
}varstr_t;

handle_t a_varstr_alloc(int size,int inc)
{
	varstr_t* pvs;

	if(size <= 0)
		size = 1024;
	if(inc <= 0)
		inc = 256;

	pvs = (varstr_t*)xmem_alloc(sizeof(varstr_t));
	pvs->size = size;
	pvs->inc = inc;
	pvs->astr = (schar_t*)xmem_alloc(pvs->size * sizeof(schar_t));
	pvs->count = 0;

	return (handle_t)pvs;
}

handle_t w_varstr_alloc(int size,int inc)
{
	varstr_t* pvs;

	if(size <= 0)
		size = 1024;
	if(inc <= 0)
		inc = 256;
	pvs = (varstr_t*)xmem_alloc(sizeof(varstr_t));
	pvs->size = size;
	pvs->inc = inc;
	pvs->wstr = (wchar_t*)xmem_alloc(pvs->size * sizeof(wchar_t));
	pvs->count = 0;

	return (handle_t)pvs;
}

void a_varstr_free(handle_t vs)
{
	varstr_t* pvs;

	assert(vs);
	pvs = (varstr_t*)vs;

	xmem_free(pvs->astr);
	xmem_free(pvs);
}

void w_varstr_free(handle_t vs)
{
	varstr_t* pvs;

	assert(vs);
	pvs = (varstr_t*)vs;

	xmem_free(pvs->wstr);
	xmem_free(pvs);
}

void a_varstr_incre(handle_t vs,int len)
{
	varstr_t* pvs;

	assert(vs);
	pvs = (varstr_t*)vs;

	while(len + pvs->count + 1 > pvs->size)
		pvs->size += pvs->inc;
	pvs->astr = (schar_t*)xmem_realloc(pvs->astr,pvs->size * sizeof(schar_t));
}

void w_varstr_incre(handle_t vs,int len)
{
	varstr_t* pvs;

	assert(vs);
	pvs = (varstr_t*)vs;

	while(len + pvs->count + 1 > pvs->size)
		pvs->size += pvs->inc;
	pvs->wstr = (wchar_t*)xmem_realloc(pvs->astr,pvs->size * sizeof(wchar_t));
}

int	a_varstr_cat(handle_t vs,const schar_t* str,int len)
{
	varstr_t* pvs;

	assert(vs);
	pvs = (varstr_t*)vs;
	if(str == NULL)
		return pvs->count;

	if(len < 0)
		len = a_xslen(str);

	if(len + pvs->count + 1 > pvs->size)
	{
		a_varstr_incre(vs,len);
	}
	a_xsncat(pvs->astr,str,len);
	pvs->count += len;

	return pvs->count;
}

int	w_varstr_cat(handle_t vs,const wchar_t* str,int len)
{
	varstr_t* pvs;

	assert(vs);
	pvs = (varstr_t*)vs;
	if(str == NULL)
		return pvs->count;

	if(len < 0)
		len = w_xslen(str);

	if(len + pvs->count + 1 > pvs->size)
	{
		w_varstr_incre(vs,len);
	}
	w_xsncat(pvs->wstr,str,len);
	pvs->count += len;

	return pvs->count;
}

int	a_varstr_copy(handle_t vs,const schar_t* str,int len)
{
	varstr_t* pvs;

	assert(vs);
	pvs = (varstr_t*)vs;
	if(str == NULL || len == 0)
	{
		a_varstr_empty(vs);
		return pvs->count;
	}

	if(len < 0)
		len = a_xslen(str);

	if(len + 1 > pvs->size)
	{
		a_varstr_incre(vs,len);
	}
	memcpy((void*)(pvs->astr),(void*)str,len * sizeof(schar_t));
	pvs->count = len;

	return pvs->count;
}

int	w_varstr_copy(handle_t vs,const wchar_t* str,int len)
{
	varstr_t* pvs;

	assert(vs);
	pvs = (varstr_t*)vs;
	if(str == NULL || len == 0)
	{
		w_varstr_empty(vs);
		return pvs->count;
	}

	if(len < 0)
		len = w_xslen(str);

	if(len + 1 > pvs->size)
	{
		w_varstr_incre(vs,len);
	}
	memcpy((void*)(pvs->wstr),(void*)str,len * sizeof(wchar_t));
	pvs->count = len;

	return pvs->count;
}

const schar_t* a_varstr_ptr(handle_t vs)
{
	varstr_t* pvs;

	assert(vs);
	pvs = (varstr_t*)vs;
	return pvs->astr;
}

const wchar_t* w_varstr_ptr(handle_t vs)
{
	varstr_t* pvs;

	assert(vs);
	pvs = (varstr_t*)vs;
	return pvs->wstr;
}

int a_varstr_len(handle_t vs)
{
	varstr_t* pvs;

	assert(vs);
	pvs = (varstr_t*)vs;
	return pvs->count;
}

int w_varstr_len(handle_t vs)
{
	varstr_t* pvs;

	assert(vs);
	pvs = (varstr_t*)vs;
	return pvs->count;
}

schar_t a_varstr_get_char(handle_t vs,int index)
{
	varstr_t* pvs;

	assert(vs);
	pvs = (varstr_t*)vs;

	assert(index >= 0 && index < pvs->count);
	return pvs->astr[index];
}

wchar_t w_varstr_get_char(handle_t vs,int index)
{
	varstr_t* pvs;

	assert(vs);
	pvs = (varstr_t*)vs;

	assert(index >= 0 && index < pvs->count);
	return pvs->wstr[index];
}

void a_varstr_set_char(handle_t vs,int index,schar_t ch)
{
	varstr_t* pvs;

	assert(vs);
	pvs = (varstr_t*)vs;

	assert(index >= 0 && index < pvs->count);
	pvs->astr[index] = ch;
}

void w_varstr_set_char(handle_t vs,int index,wchar_t ch)
{
	varstr_t* pvs;

	assert(vs);
	pvs = (varstr_t*)vs;

	assert(index >= 0 && index < pvs->count);
	pvs->wstr[index] = ch;
}

int a_varstr_ins_char(handle_t vs,int index,schar_t ch)
{
	varstr_t* pvs;

	assert(vs);
	pvs = (varstr_t*)vs;

	if(index < 0)
		index = pvs->count;

	if(pvs->count + 2 > pvs->size)
		a_varstr_incre(vs,1);

	if(index < pvs->count)
	{
		memmove((void*)(pvs->astr + index + 1),(void*)(pvs->astr + index),(pvs->count - index + 1) * sizeof(schar_t));
		pvs->astr[index] = ch;
	}else
	{
		pvs->astr[index] = ch;
		pvs->astr[index+1] = '\0';
	}
	pvs->count ++;

	return pvs->count;
}

int w_varstr_ins_char(handle_t vs,int index,wchar_t ch)
{
	varstr_t* pvs;

	assert(vs);
	pvs = (varstr_t*)vs;

	if(index < 0)
		index = pvs->count;

	if(pvs->count + 2 > pvs->size)
		w_varstr_incre(vs,1);

	if(index < pvs->count)
	{
		memmove((void*)(pvs->wstr + index + 1),(void*)(pvs->wstr + index),(pvs->count - index + 1) * sizeof(wchar_t));
		pvs->wstr[index] = ch;
	}else
	{
		pvs->wstr[index] = ch;
		pvs->wstr[index+1] = L'\0';
	}
	pvs->count ++;

	return pvs->count;
}

int a_varstr_del_char(handle_t vs,int index)
{
	varstr_t* pvs;

	assert(vs);
	pvs = (varstr_t*)vs;

	if(!pvs->count)
		return 0;

	if(index < 0)
		index = pvs->count - 1;
	if(index)
		memmove((void*)(pvs->astr + index),(void*)(pvs->astr + index + 1),(pvs->count - index) * sizeof(schar_t));
	else
		pvs->astr[index] = '\0';
	pvs->count --;

	return pvs->count;
}

int w_varstr_del_char(handle_t vs,int index)
{
	varstr_t* pvs;

	assert(vs);
	pvs = (varstr_t*)vs;

	if(!pvs->count)
		return 0;

	if(index < 0)
		index = pvs->count - 1;
	if(index)
		memmove((void*)(pvs->wstr + index),(void*)(pvs->wstr + index + 1),(pvs->count - index) * sizeof(wchar_t));
	else
		pvs->astr[index] = L'\0';
	pvs->count --;

	return pvs->count;
}

void a_varstr_empty(handle_t vs)
{
	varstr_t* pvs;

	assert(vs);
	pvs = (varstr_t*)vs;
	memset((void*)pvs->astr,0,pvs->size * sizeof(schar_t));
	pvs->count = 0;
}

void w_varstr_empty(handle_t vs)
{
	varstr_t* pvs;

	assert(vs);
	pvs = (varstr_t*)vs;
	memset((void*)pvs->wstr,0,pvs->size * sizeof(wchar_t));
	pvs->count = 0;
}


int a_varstr_printf(handle_t vs,const schar_t* fmt,...)
{
	varstr_t* pvs;
	va_list arg;
	int len;

	pvs = (varstr_t*)vs;
	
	va_start(arg,fmt);
	len = a_xsprintf_arg(NULL,fmt,arg);
	va_end(arg);

	if(!len)
		return pvs->count;

	if(pvs->count + len + 1 > pvs->size)
		a_varstr_incre(vs,len);

	va_start(arg,fmt);
	a_xsprintf_arg(pvs->astr + pvs->count,fmt,arg);
	va_end(arg);

	pvs->count += len;

	return pvs->count;
}

int w_varstr_printf(handle_t vs,const wchar_t* fmt,...)
{
	varstr_t* pvs;
	va_list arg;
	int len;

	pvs = (varstr_t*)vs;
	
	va_start(arg,fmt);
	len = w_xsprintf_arg(NULL,fmt,arg);
	va_end(arg);

	if(!len)
		return pvs->count;

	if(pvs->count + len + 1 > pvs->size)
		w_varstr_incre(vs,len);

	va_start(arg,fmt);
	w_xsprintf_arg(pvs->wstr + pvs->count,fmt,arg);
	va_end(arg);

	pvs->count += len;

	return pvs->count;
}
