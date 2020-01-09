/*************************************************************
	EasySoft xdl v4.0

	(c) 2003-2009 EasySoft Corporation.  All Rights Reserved.

	@doc xdl string function

	@module	xdlstr.c | xdl utility implement file

	@devnote 张文权 2003.01 - 2007.12 v3.0
			 张文权 2008.01 - 2009.12 v4.0
*************************************************************/

/************************************************************
*************************************************************/
#include "xdlstr.h"
#include "xdlmem.h"
#include "xdlcode.h"

schar_t* a_xsalloc(size_t len)
{
	return (schar_t*)xmem_alloc((len + 1) * sizeof(schar_t));
}

wchar_t* w_xsalloc(size_t len)
{
	return (wchar_t*)xmem_alloc((len + 1) * sizeof(wchar_t));
}

schar_t* a_xsrealloc(schar_t* str,size_t len)
{
	return (schar_t*)xmem_realloc(str,(len + 1) * sizeof(schar_t));
}

wchar_t* w_xsrealloc(wchar_t* str,size_t len)
{
	return (wchar_t*)xmem_realloc(str,(len + 1) * sizeof(wchar_t));
}

void a_xszero(schar_t* str,int len)
{
	memset((void*)str,0,len * sizeof(schar_t));
}

void w_xszero(wchar_t* str,int len)
{
	memset((void*)str,0,len * sizeof(wchar_t));
}

void a_xsfree(schar_t* str)
{
	xmem_free(str);
}

void w_xsfree(wchar_t* str)
{
	xmem_free(str);
}

/*test string is null or empty*/
bool_t a_is_null(const schar_t* sz)
{
	if(sz == NULL)
		return 1;
	if(*sz == '\0')
		return 1;
	return 0;
}

bool_t w_is_null(const wchar_t* sz)
{
	if(sz == NULL)
		return 1;
	if(*sz == L'\0')
		return 1;
	return 0;
}

bool_t a_is_numeric(const schar_t* sz)
{
	TCHAR* token;
	int dot_count = 0;
	int blk_count = 0;

	token = (TCHAR*)sz;
	while(*token != '\0' && (*token == ' ' || *token == '\t'))
		token ++;
	if(*token == '-' || *token == '+')
		token ++;

	while(*token != '\0')
	{
		switch(*token)
		{
		case '.':
			dot_count ++;
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			if(blk_count || dot_count > 1)
				return 0;
			break;
		case ' ':
		case '\t':
			if(dot_count > 1)
				return 0;
			blk_count ++;
			break;
		default:
			return 0;
		}
		token ++;
	}
	return 1;
}

bool_t w_is_numeric(const wchar_t* sz)
{
	TCHAR* token;
	int dot_count = 0;
	int blk_count = 0;

	token = (TCHAR*)sz;
	while(*token != L'\0' && (*token == L' ' || *token == L'\t'))
		token ++;
	if(*token == L'-' || *token == L'+')
		token ++;

	while(*token != L'\0')
	{
		switch(*token)
		{
		case L'.':
			dot_count ++;
			break;
		case L'0':
		case L'1':
		case L'2':
		case L'3':
		case L'4':
		case L'5':
		case L'6':
		case L'7':
		case L'8':
		case L'9':
			if(blk_count || dot_count > 1)
				return 0;
			break;
		case L' ':
		case L'\t':
			if(dot_count > 1)
				return 0;
			blk_count ++;
			break;
		default:
			return 0;
		}
		token ++;
	}
	return 1;
}

short a_xstos(const schar_t* sz)
{
	int sign = 0;
	short num = 0;
	schar_t* token = (schar_t*)sz;

	if(!sz)
		return 0;

	while((*token > '9' || *token < '0')  && *token != '\0')
		token ++;
	if(*token == '\0')
		return 0;
	if(token - 1 >= sz && *token == '-')
		sign = 1;

	do{
		num *= 10;
		num += (*token - '0');
		token ++;
	}while(*token >= '0' && *token <= '9');

	return (sign)? -num : num;
}

short w_xstos(const wchar_t* sz)
{
	int sign = 0;
	short num = 0;
	wchar_t* token = (wchar_t*)sz;

	if(!sz)
		return 0;

	while((*token > L'9' || *token < L'0') && *token != L'\0')
		token ++;
	if(*token == L'\0')
		return 0;
	if(token - 1 >= sz && *token == L'-')
		sign = 1;

	do{
		num *= 10;
		num += (*token - L'0');
		token ++;
	}while(*token >= L'0' && *token <= L'9');

	return (sign)? -num : num;
}

long a_xstol(const schar_t* sz)
{
	int sign = 0;
	long num = 0;
	schar_t* token = (schar_t*)sz;

	if(!sz)
		return 0;

	while((*token > '9' || *token < '0')  && *token != '\0')
		token ++;
	if(*token == '\0')
		return 0;
	if(token - 1 >= sz && *token == '-')
		sign = 1;

	do{
		num *= 10;
		num += (*token - '0');
		token ++;
	}while(*token >= '0' && *token <= '9');

	return (sign)? -num : num;
}

long w_xstol(const wchar_t* sz)
{
	int sign = 0;
	long num = 0;
	wchar_t* token = (wchar_t*)sz;

	if(!sz)
		return 0;

	while((*token > L'9' || *token < L'0')  && *token != L'\0')
		token ++;
	if(*token == L'\0')
		return 0;
	if(token - 1 >= sz && *token == L'-')
		sign = 1;

	do{
		num *= 10;
		num += (*token - L'0');
		token ++;
	}while(*token >= L'0' && *token <= L'9');

	return (sign)? -num : num;
}

double a_xstof(const schar_t* sz)
{
	int sign = 0;
	int prec = MAX_PREC;
	int mul = 1;
	long num = 0;
	long dec = 0; 
	schar_t* token = (schar_t*)sz;

	if(!sz)
		return 0;

	while((*token > '9' || *token < '0')  && *token != '-' && *token != '.' && *token != '\0')
		token ++;
	if(*token == '\0')
		return 0;
	if(*token == '-')
	{
		token ++;
		sign = 1;
	}
	if(*token != '.')
	{
		do{
			num *= 10;
			num += (*token - '0');
			token ++;
		}while(*token >= '0' && *token <= '9');
	}

	if(*token == '.')
	{
		token ++; 
		while(*token >= '0' && *token <= '9' && prec)
		{
			dec *= 10;
			dec += (*token - '0');
			token ++;
			prec --;
		}
		while(prec++ < MAX_PREC)
			mul *= 10;
	}

	return (sign)? -((double)num + (double)dec / (double)mul) : ((double)num + (double)dec / (double)mul);
}


double w_xstof(const wchar_t* sz)
{
	int sign = 0;
	int prec = MAX_PREC;
	int mul = 1;
	long num = 0;
	long dec = 0; 
	wchar_t* token = (wchar_t*)sz;

	if(!sz)
		return 0;

	while((*token > L'9' || *token < L'0')  && *token != L'-' && *token != L'.' && *token != L'\0')
		token ++;
	if(*token == L'\0')
		return 0;
	if(*token == L'-')
	{
		token ++;
		sign = 1;
	}
	if(*token != L'.')
	{
		do{
			num *= 10;
			num += (*token - L'0');
			token ++;
		}while(*token >= L'0' && *token <= L'9');
	}

	if(*token == L'.')
	{
		token ++; 
		while(*token >= L'0' && *token <= L'9' && prec)
		{
			dec *= 10;
			dec += (*token - L'0');
			token ++;
			prec --;
		}
		while(prec++ < MAX_PREC)
			mul *= 10;
	}

	return (sign)? -((double)num + (double)dec / (double)mul) : ((double)num + (double)dec / (double)mul);
}

/*trim string left blank*/
void a_xstriml(schar_t* sz)
{
	schar_t* token;

	if(!sz)
		return ;

	token = sz;
	while(*token == ' ' || *token == '\t')
		token ++;

	if(token == sz)
		return;
	
	while(*token)
		*(sz++) = *(token ++);

	*sz = '\0';
}

void w_xstriml(wchar_t* sz)
{
	wchar_t* token;

	if(!sz)
		return ;

	token = sz;
	while(*token == L' ' || *token == L'\t')
		token ++;

	if(token == sz)
		return;
	
	while(*token)
		*(sz++) = *(token ++);

	*sz = L'\0';
}

/*trim string right blank*/
void a_xstrimr(schar_t* sz)
{
	int len = 0;

	if(!sz)
		return;

	len = 0;
	while(*sz)
	{
		if(*sz == ' ' || *sz == '\t')
			len ++;
		else
			len = 0;
		sz ++;
	}
	
	*(sz - len) = '\0';
}

void w_xstrimr(wchar_t* sz)
{
	int len = 0;

	if(!sz)
		return;

	len = 0;
	while(*sz)
	{
		if(*sz == L' ' || *sz == L'\t')
			len ++;
		else
			len = 0;
		sz ++;
	}
	
	*(sz - len) = L'\0';
}

void a_xscut(schar_t* str,int start,int len)
{
	schar_t* token;

	if(!str)
		return;

	token = str + start;
	while(*token && len --)
		*(str ++) = *(token ++);
	*str = '\0';
}

void w_xscut(wchar_t* str,int start,int len)
{
	wchar_t* token;

	if(!str)
		return;

	token = str + start;
	while(*token && len --)
		*(str ++) = *(token ++);
	*str = L'\0';
}

int a_xslen(const schar_t* str)
{
	schar_t* token;

	if(!str)
		return 0;

	token = (schar_t*)str;
	while(*token)
		token ++;

	return (int)(token - str);
}

int w_xslen(const wchar_t* str)
{
	wchar_t* token;

	if(!str)
		return 0;

	token = (wchar_t*)str;
	while(*token)
		token ++;

	return (int)(token - str);
}

void a_xscpy(schar_t* dest,const schar_t* src)
{
	if(!dest)
		return;
	if(!src)
	{
		*dest = '\0';
		return;
	}
	while(*src)
		*(dest++) = *(src++);
	*dest = '\0';
}

void w_xscpy(wchar_t* dest,const wchar_t* src)
{
	if(!dest)
		return;
	if(!src)
	{
		*dest = L'\0';
		return;
	}
	while(*src)
		*(dest++) = *(src++);
	*dest = L'\0';
}

void a_xsncpy(schar_t* dest,const schar_t* src,int n)
{
	if(!dest)
		return;
	if(!src)
	{
		*dest = '\0';
		return;
	}
	while(*src && n--)
		(*(dest++) = *(src++));
	*dest = '\0';
}

void w_xsncpy(wchar_t* dest,const wchar_t* src,int n)
{
	if(!dest)
		return;
	if(!src)
	{
		*dest = L'\0';
		return;
	}
	while(*src && n--)
		(*(dest++) = *(src++));
	*dest = L'\0';
}

void a_xscat(schar_t* dest,const schar_t* src)
{
	if(!dest || !src)
		return;

	while(*dest)
		dest++;
	while(*src)
		*(dest++) = *(src++);
	*dest = '\0';
}

void w_xscat(wchar_t* dest,const wchar_t* src)
{
	if(!dest || !src)
		return;

	while(*dest)
		dest++;
	while(*src)
		*(dest++) = *(src++);
	*dest = L'\0';
}

void a_xsncat(schar_t* dest,const schar_t* src,int n)
{
	if(!dest || !src)
		return;

	while(*dest)
		dest++;
	while(n--  && *src)
		(*(dest++) = *(src++));
	*dest = '\0';
}

void w_xsncat(wchar_t* dest,const wchar_t* src,int n)
{
	if(!dest || !src)
		return;

	while(*dest)
		dest++;
	while(n-- && *src)
		(*(dest++) = *(src++));
	*dest = L'\0';
}

void a_xsmid(schar_t* dest,const schar_t* src,int from,int len)
{
	if(!dest || !src)
		return;

	while(*(src + from) && len --)
	{
		*dest = *(src + from);
		from ++;
	}
	*dest = '\0';
}

void w_xsmid(wchar_t* dest,const wchar_t* src,int from,int len)
{
	if(!dest || !src)
		return;

	while(*(src + from) && len --)
	{
		*(dest++) = *(src + from);
		from ++;
	}
	*dest = L'\0';
}

int a_xscmp(const schar_t* dest,const schar_t* src)
{
	int rt = 0 ;

	if(!dest && !src)
		return 0;
	else if(!dest && src)
		return (*src)? -1 : 0;
	else if(dest && !src)
		return (*dest)? 1 : 0;

	while(*dest || *src)
	{
		if(rt = *(dest++) - *(src++))
			break;
	}

	if(rt < 0)
		rt = -1 ;
	else if(rt > 0)
		rt = 1 ;

	return rt;
}

int w_xscmp(const wchar_t* dest,const wchar_t* src)
{
	int rt = 0 ;

	if(!dest && !src)
		return 0;
	else if(!dest && src)
		return (*src)? -1 : 0;
	else if(dest && !src)
		return (*dest)? 1 : 0;

	while(*dest || *src)
	{
		if(rt = *(dest++) - *(src++))
			break;
	}

	if(rt < 0)
		rt = -1 ;
	else if(rt > 0)
		rt = 1 ;

	return rt;
}

int a_xsncmp(const schar_t* dest,const schar_t* src,int n)
{
	int rt = 0 ;

	if(!dest && !src)
		return 0;
	else if(!dest && src)
		return (*src)? -1 : 0;
	else if(dest && !src)
		return (*dest)? 1 : 0;

	while(n-- && *src)
	{
		if(rt = *(dest++) - *(src++))
			break;
	}

	if(rt < 0)
		rt = -1 ;
	else if(rt > 0)
		rt = 1 ;

	return rt;
}

int w_xsncmp(const wchar_t* dest,const wchar_t* src,int n)
{
	int rt = 0 ;

	if(!dest && !src)
		return 0;
	else if(!dest && src)
		return (*src)? -1 : 0;
	else if(dest && !src)
		return (*dest)? 1 : 0;

	while(n-- && *src)
	{
		if(rt = *(dest++) - *(src++))
			break;
	}

	if(rt < 0)
		rt = -1 ;
	else if(rt > 0)
		rt = 1 ;

	return rt;
}

schar_t ach_upper(schar_t ch)
{
	if(ch >= 'a' && ch <= 'z') 
		return ch + ('A' - 'a');
	else 
		return ch;
}

schar_t ach_lower(schar_t ch)
{
	if(ch >= 'A' && ch <= 'Z')
		return ch + ('a' - 'A');
	else 
		return ch;
}

wchar_t wch_upper(wchar_t ch)
{
	if(ch >= L'a' && ch <= L'z') 
		return ch + (L'A' - L'a'); 
	else 
		return ch;
}

wchar_t wch_lower(wchar_t ch)
{
	if(ch >= L'A' && ch <= L'Z') 
		return ch + (L'a' - L'A'); 
	else 
		return ch;
}

void a_xsupr(schar_t* sz)
{
	if(!sz)
		return;

	while(*sz)
	{
		if(*sz >= 'a' && *sz <= 'z') 
			*sz = *sz + ('A' - 'a'); 
		sz ++;
	}
}

void w_xsupr(wchar_t* sz)
{
	if(!sz)
		return;

	while(*sz)
	{
		if(*sz >= L'a' && *sz <= L'z') 
			*sz = *sz + (L'A' - L'a'); 
		sz ++;
	}
}

void a_xslwr(schar_t* sz)
{
	if(!sz)
		return;

	while(*sz)
	{
		if(*sz >= 'A' && *sz <= 'Z') 
			*sz = *sz + ('a' - 'A'); 
		sz ++;
	}
}

void w_xslwr(wchar_t* sz)
{
	if(!sz)
		return;

	while(*sz)
	{
		if(*sz >= L'A' && *sz <= L'Z') 
			*sz = *sz + (L'a' - L'A'); 
		sz ++;
	}
}

int a_xsicmp(const schar_t* dest,const schar_t* src)
{
	int rt = 0 ;

	if(!dest && !src)
		return 0;
	else if(!dest && src)
		return (*src)? -1 : 0;
	else if(dest && !src)
		return (*dest)? 1 : 0;

	while(*src || *dest)
	{
		if(rt = ach_upper(*dest) - ach_upper(*src))
			break;
		dest++;
		src ++;
	}

	if(rt < 0)
		rt = -1 ;
	else if(rt > 0)
		rt = 1 ;

	return rt;
}

int w_xsicmp(const wchar_t* dest,const wchar_t* src)
{
	int rt = 0 ;

	if(!dest && !src)
		return 0;
	else if(!dest && src)
		return (*src)? -1 : 0;
	else if(dest && !src)
		return (*dest)? 1 : 0;

	while(*src || *dest)
	{
		if(rt = wch_upper(*dest) - wch_upper(*src))
			break;
		dest++;
		src ++;
	}

	if(rt < 0)
		rt = -1 ;
	else if(rt > 0)
		rt = 1 ;

	return rt;
}

int a_xsnicmp(const schar_t* dest,const schar_t* src,int n)
{
	int rt = 0 ;

	if(!dest && !src)
		return 0;
	else if(!dest && src)
		return (*src)? -1 : 0;
	else if(dest && !src)
		return (*dest)? 1 : 0;

	while(n-- && *src)
	{
		if(rt = ach_upper(*dest) - ach_upper(*src))
			break;
		dest++;
		src ++;
	}

	if(rt < 0)
		rt = -1 ;
	else if(rt > 0)
		rt = 1 ;

	return rt;
}

int w_xsnicmp(const wchar_t* dest,const wchar_t* src,int n)
{
	int rt = 0 ;

	if(!dest && !src)
		return 0;
	else if(!dest && src)
		return (*src)? -1 : 0;
	else if(dest && !src)
		return (*dest)? 1 : 0;

	while(n-- && *src)
	{
		if(rt = wch_upper(*dest) - wch_upper(*src))
			break;
		dest++;
		src ++;
	}

	if(rt < 0)
		rt = -1 ;
	else if(rt > 0)
		rt = 1 ;

	return rt;
}

schar_t* a_xsstr(const schar_t* str,const schar_t* sub)
{
	schar_t *tk = (schar_t *)str;
	schar_t *s1, *s2;

	if (!sub || !*sub)
		return((schar_t *)str);

	while(*tk)
	{
		s1 = tk;
		s2 = (schar_t *)sub;

		while (*s1 && *s2 && !(*s1-*s2) )
			s1++, s2++;

		if (!*s2)
			return tk;

		tk++;
	}

	return NULL;
}


wchar_t* w_xsstr(const wchar_t* str,const wchar_t* sub)
{
	wchar_t *tk = (wchar_t *)str;
	wchar_t *s1, *s2;

	if (!sub || !*sub)
		return((wchar_t *)str);

	while(*tk)
	{
		s1 = tk;
		s2 = (wchar_t *)sub;

		while (*s1 && *s2 && !(*s1-*s2) )
			s1++, s2++;

		if (!*s2)
			return tk;

		tk++;
	}

	return NULL;
}

//%[flag] [width] [.precision] [{h | l | I64 | L}]type
//flags: -,+,0,' ',#
//width:
//precision
//type:c,C,d,i,o,u,x,X,e,E,f,g,s,S,T,t

typedef enum{
	XS_SKIP = 0,
	XS_PROC = 1,
	XS_FLAG = 2,
	XS_WIDTH = 3,
	XS_PREC = 4,
	XS_SIZE= 5,
	XS_TYPE = 6,
	XS_TERM = 7
}XF_STATUS;

typedef enum{
	XO_PAUSE = 0,
	XO_CONTINUE = 1
}XF_OPERA;

#define s_is_flag(ch)	((ch == '0' || ch == ' ' || ch == '#')? 1 : 0)
#define s_is_digit(ch)	((ch >= '0' && ch <= '9')? 1 : 0)
#define s_is_size(ch)	((*token == 'h' || *token == 'l')? 1 : 0)
#define s_is_type(ch)	((ch == 'c' || ch == 'C' || ch == 'd' || ch == 'u' || ch == 'x' || ch == 'X' || ch == 'f'|| ch == 's' || ch == 'S' || ch == 't' || ch == 'T')? 1 : 0)

#define w_is_flag(ch)	((ch == L'0' || ch == L' ' || ch == L'#')? 1 : 0)
#define w_is_digit(ch)	((ch >= L'0' && ch <= L'9')? 1 : 0)
#define w_is_size(ch)	((*token == L'h' || *token == L'l')? 1 : 0)
#define w_is_type(ch)	((ch == L'c' || ch == L'C' || ch == L'd' || ch == L'u' || ch == L'x' || ch == L'X' || ch == L'f'|| ch == L's' || ch == L'S' || ch == L't' || ch == L'T')? 1 : 0)

int a_tk_printf(schar_t* buf,schar_t flag,int width,int prec,schar_t size,schar_t type,va_list* parg)
{
	wchar_t wch;
	schar_t ch;
	schar_t tmp[NUM_LEN + 1] = {0};
	int len,pos;
	schar_t sign;
	short s;
	int i;
	unsigned short us;
	unsigned int ui,xi;
	double dbl;
	schar_t* sz;
	wchar_t* wsz;

	switch(type)
	{
	case 'c':
		ch = va_arg(*parg,schar_t);
		if(buf)
		{
			buf[0] = ch;
			buf[1] = '\0';
		}
		return 1;
	case 'C':
		wch = va_arg(*parg,wchar_t);
		len = gb2312_encode(&wch,1,NULL,0);
		if(buf)
		{
			gb2312_encode(&wch,1,(byte_t*)buf,len);
			buf[len] = '\0';
		}
		return len;
	case 'd':
		len = 0;
		sign = 0;
		pos = 0;
		if(size == 'h')
		{
			s = va_arg(*parg,short);
			if(s < 0)
			{
				sign = '-';
				s = 0 - s;
			}
			
			while(s)
			{
				tmp[len ++] = s % 10 + 48;
				s /= 10;
			}
		}else
		{
			i = va_arg(*parg,int);
			if(i < 0)
			{
				sign = '-';
				i = 0 - i;
			}

			while(i)
			{
				tmp[len ++] = i % 10 + 48;
				i /= 10;
			}
		}
		if(sign)
		{
			if(buf)
				buf[pos] = sign;
			pos ++;
			width --;
		}
		width -= len;
		while(width > 0)
		{
			if(buf)
				buf[pos] = '0';
			pos ++;
			width --;
		}
		while(len--)
		{
			if(buf)
				buf[pos] = tmp[len];
			pos ++;
		}
		if(buf)
			buf[pos] = '\0';
		return pos;
	case 'u':
		len = 0;
		sign = 0;
		pos = 0;
		if(size == 'h')
		{
			us = va_arg(*parg,unsigned short);
			while(us)
			{
				tmp[len ++] = us % 10 + 48;
				us /= 10;
			}
		}else
		{
			ui = va_arg(*parg,unsigned int);
			while(ui)
			{
				tmp[len ++] = ui % 10 + 48;
				ui /= 10;
			}
		}
		width -= len;
		while(width > 0)
		{
			if(buf)
				buf[pos] = '0';
			pos ++;
			width --;
		}
		while(len--)
		{
			if(buf)
				buf[pos] = tmp[len];
			pos ++;
		}
		if(buf)
			buf[pos] = '\0';
		return pos;
	case 'x':
	case 'X':
		len = 0;
		sign = 0;
		pos = 0;

		ui = va_arg(*parg,unsigned int);
		while(ui)
		{
			us = ui % 16;
			if(type == 'x')
				tmp[len ++] = (us < 9)? (us + 48) : (us + 87);
			else
				tmp[len ++] = (us < 9)? (us + 48) : (us + 55);
			ui /= 16;
		}

		width -= len;
		while(width > 0)
		{
			if(buf)
				buf[pos] = '0';
			pos ++;
			width --;
		}
		while(len--)
		{
			if(buf)
				buf[pos] = tmp[len];
			pos ++;
		}
		if(buf)
			buf[pos] = '\0';
		return pos;
	case 'f':
		len = 0;
		sign = 0;
		pos = 0;

		dbl = va_arg(*parg,double);
		if(dbl < 0)
		{
			sign = '-';
			dbl = 0 - dbl;
		}

		xi = (int)dbl;

		dbl -= (double)xi;
		if(prec)
			s = (short)prec;
		else
			s = MAX_PREC;
		while(s-- && dbl)
			dbl *= 10.0;

		ui = (int)floor(dbl + 0.5);

		while(ui && !(ui % 10))
			ui /= 10;

		while(ui)
		{
			tmp[len ++] = ui % 10 + 48;
			ui /= 10;
		}
		if(len)
			tmp[len ++] = '.';

		do{
			tmp[len ++] = xi % 10 + 48;
			xi /= 10;
			width --;
		}while(xi);

		while(len > 0 && tmp[len-1] == '0')
			len --;
		tmp[len] = '\0';

		if(sign)
		{
			if(buf)
				buf[pos] = sign;
			pos ++;
			width --;
		}

		/*while(width > 0)
		{
			if(buf)
				buf[pos] = '0';
			pos ++;
			width --;
		}*/

		while(len--)
		{
			if(buf)
				buf[pos] = tmp[len];
			pos ++;
		}
		if(buf)
			buf[pos] = '\0';
		return pos;
	case 's':
		len = 0;
		sign = 0;
		pos = 0;

		sz = va_arg(*parg,schar_t*);
		if(!sz)
		{
			if(buf)
				buf[pos] = '\0';
			return pos;
		}
		
		len = (int)strlen(sz);
		width -= len;
		while(width > 0)
		{
			if(buf)
				buf[pos] = flag;
			pos ++;
			width --;
		}

		if(buf)
			memcpy((void*)(buf + pos),(void*)sz,len * sizeof(schar_t));
		pos += len;
		if(buf)
			buf[pos] = '\0';
		return pos;
	case 'S':
		len = 0;
		sign = 0;
		pos = 0;

		wsz = va_arg(*parg,wchar_t*);
		if(!wsz)
		{
			if(buf)
				buf[pos] = '\0';
			return pos;
		}

		ui = (int)wcslen(wsz);
		len = gb2312_encode(wsz,ui,NULL,0);
		width -= len;
		while(width > 0)
		{
			if(buf)
				buf[pos] = flag;
			pos ++;
			width --;
		}
		
		if(buf)
			gb2312_encode(wsz,ui,(byte_t*)(buf + pos),len);
		pos += len;
		if(buf)
			buf[pos] = '\0';
		return pos;
	}
	return 0;
}


int w_tk_printf(wchar_t* buf,wchar_t flag,int width,int prec,wchar_t size,wchar_t type,va_list* parg)
{
	schar_t ch;
	wchar_t wch;
	wchar_t tmp[NUM_LEN + 1] = {0};
	int len,pos;
	wchar_t sign;
	short s;
	int i;
	unsigned short us;
	unsigned int ui,xi;
	double dbl;
	schar_t* sz;
	wchar_t* wsz;

	switch(type)
	{
	case L'c':
		wch = va_arg(*parg,wchar_t);
		if(buf)
		{
			buf[0] = wch;
			buf[1] = L'\0';
		}
		return 1;
	case L'C':
		ch = va_arg(*parg,schar_t);
		len = gb2312_decode((byte_t*)&ch,1,NULL,0);
		if(buf)
		{
			gb2312_decode((byte_t*)&ch,1,buf,len);
			buf[len] = L'\0';
		}
		return len;
	case L'd':
		len = 0;
		sign = 0;
		pos = 0;
		if(size == L'h')
		{
			s = va_arg(*parg,short);
			if(s < 0)
			{
				sign = L'-';
				s = 0 - s;
			}
			
			do
			{
				tmp[len ++] = s % 10 + 48;
				s /= 10;
			}while(s);
		}else
		{
			i = va_arg(*parg,int);
			if(i < 0)
			{
				sign = L'-';
				i = 0 - i;
			}

			do
			{
				tmp[len ++] = i % 10 + 48;
				i /= 10;
			}while(i);
		}
		if(sign)
		{
			if(buf)
				buf[pos] = sign;
			pos ++;
			width --;
		}
		width -= len;
		while(width > 0)
		{
			if(buf)
				buf[pos] = L'0';
			pos ++;
			width --;
		}
		while(len--)
		{
			if(buf)
				buf[pos] = tmp[len];
			pos ++;
		}
		if(buf)
			buf[pos] = L'\0';
		return pos;
	case L'u':
		len = 0;
		sign = 0;
		pos = 0;
		if(size == L'h')
		{
			us = va_arg(*parg,unsigned short);
			while(us)
			{
				tmp[len ++] = us % 10 + 48;
				us /= 10;
			}
		}else
		{
			ui = va_arg(*parg,unsigned int);
			do
			{
				tmp[len ++] = ui % 10 + 48;
				ui /= 10;
			}while(ui);
		}
		width -= len;
		while(width > 0)
		{
			if(buf)
				buf[pos] = L'0';
			pos ++;
			width --;
		}
		while(len--)
		{
			if(buf)
				buf[pos] = tmp[len];
			pos ++;
		}
		if(buf)
			buf[pos] = L'\0';
		return pos;
	case L'x':
	case L'X':
		len = 0;
		sign = 0;
		pos = 0;

		ui = va_arg(*parg,unsigned int);
		while(ui)
		{
			us = ui % 16;
			if(type == L'x')
				tmp[len ++] = (us < 9)? (us + 48) : (us + 87);
			else
				tmp[len ++] = (us < 9)? (us + 48) : (us + 55);
			ui /= 16;
		}

		width -= len;
		while(width > 0)
		{
			if(buf)
				buf[pos] = L'0';
			pos ++;
			width --;
		}
		while(len--)
		{
			if(buf)
				buf[pos] = tmp[len];
			pos ++;
		}
		if(buf)
			buf[pos] = L'\0';
		return pos;
	case L'f':
		len = 0;
		sign = 0;
		pos = 0;

		dbl = va_arg(*parg,double);
		if(dbl < 0)
		{
			sign = L'-';
			dbl = 0 - dbl;
		}

		xi = (int)dbl;

		dbl -= (double)xi;
		if(prec)
			s = (short)prec;
		else
			s = MAX_PREC;
		while(s-- && dbl)
			dbl *= 10.0;

		ui = (int)floor(dbl + 0.5);

		while(ui && !(ui % 10))
			ui /= 10;

		while(ui)
		{
			tmp[len ++] = ui % 10 + 48;
			ui /= 10;
		}
		if(len)
			tmp[len ++] = L'.';

		do{
			tmp[len ++] = xi % 10 + 48;
			xi /= 10;
			width --;
		}while(xi);

		if(sign)
		{
			if(buf)
				buf[pos] = sign;
			pos ++;
			width --;
		}

		/*while(width > 0)
		{
			if(buf)
				buf[pos] = L'0';
			pos ++;
			width --;
		}*/

		while(len-- && tmp[len])
		{
			if(buf)
				buf[pos] = tmp[len];
			pos ++;
		}
		if(buf)
			buf[pos] = L'\0';
		return pos;
	case L's':
		len = 0;
		sign = 0;
		pos = 0;

		wsz = va_arg(*parg,wchar_t*);
		if(!wsz)
		{
			if(buf)
				buf[pos] = L'\0';
			return pos;
		}

		len = w_xslen(wsz);
		width -= len;
		while(width > 0)
		{
			if(buf)
				buf[pos] = flag;
			pos ++;
			width --;
		}

		if(buf)
			memcpy((void*)(buf + pos),(void*)wsz,len * sizeof(wchar_t));
		pos += len;
		if(buf)
			buf[pos] = L'\0';
		return pos;
	case L'S':
		len = 0;
		sign = 0;
		pos = 0;

		sz = va_arg(*parg,schar_t*);
		if(!sz)
		{
			if(buf)
				buf[pos] = L'\0';
			return pos;
		}

		ui = a_xslen(sz);
		len = gb2312_decode((byte_t*)sz,ui,NULL,0);
		width -= len;
		while(width > 0)
		{
			if(buf)
				buf[pos] = flag;
			pos ++;
			width --;
		}
		
		if(buf)
			gb2312_decode((byte_t*)sz,ui,buf + pos,len);
		pos += len;
		if(buf)
			buf[pos] = L'\0';
		return pos;
	}
	return 0;
}

int a_xsprintf(schar_t* buf,const schar_t* fmt,...)
{
	int rt;
	va_list arg;
	
	va_start(arg,fmt);
	rt = a_xsprintf_arg(buf,fmt,arg);
	va_end(arg);

	return rt;
}


int a_xsprintf_arg(schar_t* buf,const schar_t* fmt,va_list arg)
{
	int total = 0;
	schar_t xf_flag = 0;
	int xf_width = 0;
	int xf_prec = 0;
	schar_t xf_size = 0;
	schar_t xf_type = 0;
	schar_t tk_width[NUM_LEN + 1],tk_prec[NUM_LEN + 1];
	int width_count = 0;
	int prec_count = 0;
	int tk_count = 0;


	XF_STATUS xs = XS_SKIP;
	XF_OPERA xo = XO_PAUSE;

	schar_t* token = (schar_t*)fmt;

	while(xs != XS_TERM)
	{
		switch(xs)
		{
		case XS_SKIP:
			if(*token == '%' && *(token + 1) != '%')
			{
				if(!tk_count)
				{
					xs = XS_FLAG;
					xo = XO_CONTINUE;
				}else
				{
					xs = XS_PROC;
					xo = XO_PAUSE;
				}
			}else if(*token == '\0')
			{
				xs = XS_PROC;
				xo = XO_PAUSE; 
			}else
			{
				xs = XS_SKIP;
				xo = XO_CONTINUE; 
			}
			break;
		case XS_FLAG:
			if(s_is_flag(*token)) 
			{
				xf_flag = *token;
				xs = XS_FLAG;
				xo = XO_CONTINUE;
			}else
			{
				xs = XS_WIDTH;
				xo = XO_PAUSE;
			}
			break;
		case XS_WIDTH:
			if(s_is_digit(*token))
			{
				tk_width[width_count ++] = *token;
				xs = XS_WIDTH;
				xo = XO_CONTINUE;
			}else if(*token == '.')
			{
				xs = XS_PREC;
				xo = XO_CONTINUE;
			}else
			{
				xs = XS_SIZE;
				xo = XO_PAUSE;
			}
			break;
		case XS_PREC:
			if(s_is_digit(*token))
			{
				tk_prec[prec_count ++] = *token;
				xs = XS_PREC;
				xo = XO_CONTINUE;
			}else
			{
				xs = XS_SIZE;
				xo = XO_PAUSE;
			}
			break;
		case XS_SIZE:
			if(s_is_size(*token))
			{
				xf_size = *token;
				xs = XS_TYPE;
				xo = XO_CONTINUE;
			}else
			{
				xs = XS_TYPE;
				xo = XO_PAUSE;
			}
			break;
		case XS_TYPE:
			if(s_is_type(*token))
			{
				xf_type = *token;
				xs = XS_PROC;
				xo = XO_CONTINUE;
			}else
			{
				xs = XS_PROC;
				xo = XO_CONTINUE;
			}
			break;
		case XS_PROC:
			if(xf_type)
			{
				tk_width[width_count] = '\0';
				xf_width = atoi(tk_width);
				tk_prec[prec_count] = '\0';
				xf_prec = atoi(tk_prec);
				if(!xf_flag)
					xf_flag = ' ';
				if(xf_prec && xf_prec > MAX_PREC)
					xf_prec = MAX_PREC;
				total += a_tk_printf((buf)? (buf + total) : NULL,xf_flag,xf_width,xf_prec,xf_size,xf_type,&arg);

				if(*token == '\0')
					xs = XS_TERM;
				else
					xs = XS_SKIP;
				xo = XO_PAUSE;
			}else
			{
				if(buf)
				{
					memcpy((void*)(buf + total),(void*)(token - tk_count),tk_count * sizeof(schar_t));
					buf[total + tk_count] = '\0';
				}
				total += tk_count;

				if(*token == '\0')
					xs = XS_TERM;
				else
					xs = XS_SKIP;
				xo = XO_PAUSE;
			}
			xf_flag = 0;
			xf_width = 0;
			xf_prec = 0;
			xf_size = 0;
			xf_type = 0;
			width_count = prec_count = tk_count = 0;

			break;
		}

		if(xo == XO_CONTINUE)
		{
			token ++;
			tk_count ++;
		}
	}

	return total;
}

int w_xsprintf(wchar_t* buf,const wchar_t* fmt,...)
{
	int rt;
	va_list arg;
	
	va_start(arg,fmt);
	rt = w_xsprintf_arg(buf,fmt,arg);
	va_end(arg);

	return rt;
}

int w_xsprintf_arg(wchar_t* buf,const wchar_t* fmt,va_list arg)
{
	int total = 0;
	wchar_t xf_flag = 0;
	int xf_width = 0;
	int xf_prec = 0;
	wchar_t xf_size = 0;
	wchar_t xf_type = 0;
	wchar_t tk_width[NUM_LEN + 1],tk_prec[NUM_LEN + 1];
	int width_count = 0;
	int prec_count = 0;
	int tk_count = 0;


	XF_STATUS xs = XS_SKIP;
	XF_OPERA xo = XO_PAUSE;

	wchar_t* token = (wchar_t*)fmt;

	while(xs != XS_TERM)
	{
		switch(xs)
		{
		case XS_SKIP:
			if(*token == L'%' && *(token + 1) != L'%')
			{
				if(!tk_count)
				{
					xs = XS_FLAG;
					xo = XO_CONTINUE;
				}else
				{
					xs = XS_PROC;
					xo = XO_PAUSE;
				}
			}else if(*token == L'\0')
			{
				xs = XS_PROC;
				xo = XO_PAUSE; 
			}else
			{
				xs = XS_SKIP;
				xo = XO_CONTINUE; 
			}
			break;
		case XS_FLAG:
			if(w_is_flag(*token)) 
			{
				xf_flag = *token;
				xs = XS_FLAG;
				xo = XO_CONTINUE;
			}else
			{
				xs = XS_WIDTH;
				xo = XO_PAUSE;
			}
			break;
		case XS_WIDTH:
			if(w_is_digit(*token))
			{
				tk_width[width_count ++] = *token;
				xs = XS_WIDTH;
				xo = XO_CONTINUE;
			}else if(*token == L'.')
			{
				xs = XS_PREC;
				xo = XO_CONTINUE;
			}else
			{
				xs = XS_SIZE;
				xo = XO_PAUSE;
			}
			break;
		case XS_PREC:
			if(w_is_digit(*token))
			{
				tk_prec[prec_count ++] = *token;
				xs = XS_PREC;
				xo = XO_CONTINUE;
			}else
			{
				xs = XS_SIZE;
				xo = XO_PAUSE;
			}
			break;
		case XS_SIZE:
			if(w_is_size(*token))
			{
				xf_size = *token;
				xs = XS_TYPE;
				xo = XO_CONTINUE;
			}else
			{
				xs = XS_TYPE;
				xo = XO_PAUSE;
			}
			break;
		case XS_TYPE:
			if(w_is_type(*token))
			{
				xf_type = *token;
				xs = XS_PROC;
				xo = XO_CONTINUE;
			}else
			{
				xs = XS_PROC;
				xo = XO_CONTINUE;
			}
			break;
		case XS_PROC:
			if(xf_type)
			{
				tk_width[width_count] = L'\0';
				xf_width = _wtoi(tk_width);
				tk_prec[prec_count] = L'\0';
				xf_prec = _wtoi(tk_prec);
				if(!xf_flag)
					xf_flag = L' ';
				if(xf_prec && xf_prec > MAX_PREC)
					xf_prec = MAX_PREC;
				total += w_tk_printf((buf)? (buf + total) : NULL,xf_flag,xf_width,xf_prec,xf_size,xf_type,&arg);

				if(*token == L'\0')
					xs = XS_TERM;
				else
					xs = XS_SKIP;
				xo = XO_PAUSE;
			}else
			{
				if(buf)
				{
					memcpy((void*)(buf + total),(void*)(token - tk_count),tk_count * sizeof(wchar_t));
					buf[total + tk_count] = L'\0';
				}
				total += tk_count;

				if(*token == L'\0')
					xs = XS_TERM;
				else
					xs = XS_SKIP;
				xo = XO_PAUSE;
			}
			xf_flag = 0;
			xf_width = 0;
			xf_prec = 0;
			xf_size = 0;
			xf_type = 0;
			width_count = prec_count = tk_count = 0;

			break;
		}

		if(xo == XO_CONTINUE)
		{
			token ++;
			tk_count ++;
		}
	}

	return total;
}


schar_t* a_next_options_token(const schar_t* options,schar_t itemfeed,schar_t linefeed,schar_t** keyptr,int* keylen,schar_t** valptr,int* vallen)
{
	schar_t* token;

	*keyptr = *valptr = NULL;
	*keylen = *vallen = 0;

	if(a_is_null(options))
		return NULL;

	token = (schar_t*)options;
	/*skip blank*/
	while((*token == ' ' || *token == '\t') && *token != '\0')
	{
		token ++;
	}

	*keyptr = token;
	while(*token != itemfeed && *token != '\0')
	{
		token ++;
	}
	*keylen = (int)(token - *keyptr);
	
	if(*token == '\0')
		return token;
	
	token ++; /*skip item feed*/
	
	/*skip blank*/
	while((*token == ' ' || *token == '\t') && *token != '\0')
	{
		token ++;
	}

	if(*token == '\0')
		return token;

	*valptr = token;
	while(*token != linefeed && *token != '\0')
	{
		token ++;
	}
	*vallen = (int)(token - *valptr);
	
	while(*token == linefeed || *token == '\n')
	{
		token ++;
	}

	return token;
}

wchar_t* w_next_options_token(const wchar_t* options,wchar_t itemfeed,wchar_t linefeed,wchar_t** keyptr,int* keylen,wchar_t** valptr,int* vallen)
{
	wchar_t* token;

	*keyptr = *valptr = NULL;
	*keylen = *vallen = 0;

	if(w_is_null(options))
		return NULL;

	token = (wchar_t*)options;
	/*skip blank*/
	while((*token == L' ' || *token == L'\t') && *token != L'\0')
	{
		token ++;
	}

	*keyptr = token;
	while(*token != itemfeed && *token != L'\0')
	{
		token ++;
	}
	*keylen = (int)(token - *keyptr);
	
	if(*token == L'\0')
		return token;
	
	token ++; /*skip item feed*/
	
	/*skip blank*/
	while((*token == L' ' || *token == L'\t') && *token != L'\0')
	{
		token ++;
	}

	if(*token == L'\0')
		return token;

	*valptr = token;
	while(*token != linefeed && *token != L'\0')
	{
		token ++;
	}
	*vallen = (int)(token - *valptr);
	
	while(*token == linefeed || *token == L'\n')
	{
		token ++;
	}

	return token;
}
