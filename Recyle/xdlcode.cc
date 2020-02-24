#include "xdlcode.h"
#include "xdlstr.h"

bool_t is_ascii(byte_t b)
{
	return ((b & ~0x7F) == 0)? 1 : 0;
}

int gb2312_sequence(const byte_t* bs)
{
	byte_t b = bs[0];

	if(b < 0x80)
		return 1;

	//head
	if(0xa1 <= b && b <= 0xf7)
		return 2;
	//tail
	if(0xa1 <= b && b <= 0xfe)
		return 1;
	return 0;
}

int gb2312_bytes(wchar_t ch)
{
	return WideCharToMultiByte(CP_ACP,0,&ch,1,NULL,0,NULL,NULL);
}

int utf8_sequence(const byte_t* bs)
{
	byte_t b = bs[0];

	if((b & ~0x7F) == 0) {
		return 1;
	}
	if ((b & 0xC0) != 0xC0) {
		return 0;
	}
	if ((b & 0xE0) == 0xC0) {
		return 2;
	}
	if ((b & 0xF0) == 0xE0) {
		return 3;
	}
	if ((b & 0xF8) == 0xF0) {
		return 4;
	}
	return 0;
}

int utf8_bytes(wchar_t ch)
{
	int c = (int)ch;
	
	if(c >= 0x00 && c <=0x7F){
		return 1;
	}

	if(c >= 0x80 && c <= 0x7FF){
		return 2;
	}

	if(c >= 0x800 && c <= 0xFFFF){
		return 3;
	}

	if(c >= 0x10000 && c <= 0xFFFFF){
		return 4;
	}
	return 0;
}

int gb2312_decode_byte(const byte_t* src,wchar_t* dest)
{
	int len = gb2312_sequence(src);
	return MultiByteToWideChar(CP_ACP,0,(LPCSTR)src,len,dest,1);
}

int gb2312_decode(const byte_t* src,int slen,wchar_t* dest,int dlen)
{
	return MultiByteToWideChar(CP_ACP,0,(LPCSTR)src,slen,dest,dlen);
}

int utf8_decode_byte(const byte_t* src,wchar_t* dest)
{
	byte_t b0,b1,b2,b3;
	int c,len;
	
	len = utf8_sequence(src);

	if(len == 1)
	{
		// Handle 1-byte sequences (plain ASCII). 
		//UNICODE 00000000 - 0000007F.
		//UTF8 0xxxxxxx 
		b0 = src[0];
		c = (int)b0;
	}else if(len == 2)
	{
		// Handle 2-byte sequences.
		//UNICODE 00000080-000007FF
		//UTF8 110xxxxx 10xxxxxx 

		//((110xxxxx & 00011111) << 6 = 0000 0xxx xx00 0000
		//(10xxxxxx & 00111111)       = 0000 0000 00xx xxxx
		b0 = src[0];
		b1 = src[1];
		c = ((b0 & 0x1F) << 6) | (b1 & 0x3F);
		if (c < 0x80)
			c = ALTER_CHAR;
	}else if(len == 3)
	{
		// Handle 3-byte sequences.
		//UNICODE 00000800-0000FFFF
		//UTF8 1110xxxx 10xxxxxx 10xxxxxx

		//(1110xxxx & 00001111) << 12 = xxxx 0000 0000 0000
		//(10xxxxxx & 00111111) << 6  = 0000 xxxx xx00 0000
		//(10xxxxxx & 00111111)       = 0000 0000 00xx xxxx
		b0 = src[0];
		b1 = src[1];
		b2 = src[2];
		c = ((b0 & 0xF) << 12) | ((b1 & 0x3F) << 6) | (b2 & 0x3F);
		if (c < 0x800)
			c = ALTER_CHAR;
		else if (c >= 0xD800 && c <= 0xDFFF)
			c = ALTER_CHAR;
	}else if(len == 4)
	{
		// Handle 4-byte sequences.
		//UNICODE 00010000-001FFFFF
		//11110xxx 10xxxxxx 10xxxxxx 10xxxxxx

		//(11110xxx & 00000111) << 18 = 000x xx00 0000 0000 0000 0000
		//(10xxxxxx & 00111111) << 12 = 0000 00xx xxxx 0000 0000 0000
		//(10xxxxxx & 00111111) << 6  = 0000 0000 0000 xxxx xx00 0000
		//(10xxxxxx & 00111111)       = 0000 0000 0000 0000 00xx xxxx
		b0 = src[0];
		b1 = src[1];
		b2 = src[2];
		b3 = src[3];
		c = ((b0 & 0x7) << 18) | ((b1 & 0x3F) << 12) | ((b2 & 0x3F) << 6) | (b3 & 0x3F);
		if (c < 0x10000 || c > 0x10FFFF)
			c = ALTER_CHAR;
	}else
	{
		// Handle other byte sequences (never valid).
		c = ALTER_CHAR;
	}

	if(dest)
		dest[0] = (wchar_t)c;

	return len;
}

int utf8_decode(const byte_t* src,int slen,wchar_t* dest,int dlen)
{
	int c,len,index = 0;
	byte_t b0,b1,b2,b3;
	byte_t *token;

	token = (byte_t*)src;
	if(slen < 0)
		slen = a_xslen((schar_t*)src);

	while(!a_is_null((schar_t*)token))
	{
		len = utf8_sequence(token);

		if(len == 1)
		{
			// Handle 1-byte sequences (plain ASCII). 
			//UNICODE 00000000 - 0000007F.
			//UTF8 0xxxxxxx 
			b0 = token[0];
			c = (int)b0;
		}else if(len == 2)
		{
			// Handle 2-byte sequences.
			//UNICODE 00000080-000007FF
			//UTF8 110xxxxx 10xxxxxx 

			//((110xxxxx & 00011111) << 6 = 0000 0xxx xx00 0000
			//(10xxxxxx & 00111111)       = 0000 0000 00xx xxxx
			b0 = token[0];
			b1 = token[1];
			c = ((b0 & 0x1F) << 6) | (b1 & 0x3F);
			if (c < 0x80)
				c = ALTER_CHAR;
		}else if(len == 3)
		{
			// Handle 3-byte sequences.
			//UNICODE 00000800-0000FFFF
			//UTF8 1110xxxx 10xxxxxx 10xxxxxx

			//(1110xxxx & 00001111) << 12 = xxxx 0000 0000 0000
			//(10xxxxxx & 00111111) << 6  = 0000 xxxx xx00 0000
			//(10xxxxxx & 00111111)       = 0000 0000 00xx xxxx
			b0 = token[0];
			b1 = token[1];
			b2 = token[2];
			c = ((b0 & 0xF) << 12) | ((b1 & 0x3F) << 6) | (b2 & 0x3F);
			if (c < 0x800)
				c = ALTER_CHAR;
			else if (c >= 0xD800 && c <= 0xDFFF)
				c = ALTER_CHAR;
		}else if(len == 4)
		{
			// Handle 4-byte sequences.
			//UNICODE 00010000-001FFFFF
			//11110xxx 10xxxxxx 10xxxxxx 10xxxxxx

			//(11110xxx & 00000111) << 18 = 000x xx00 0000 0000 0000 0000
			//(10xxxxxx & 00111111) << 12 = 0000 00xx xxxx 0000 0000 0000
			//(10xxxxxx & 00111111) << 6  = 0000 0000 0000 xxxx xx00 0000
			//(10xxxxxx & 00111111)       = 0000 0000 0000 0000 00xx xxxx
			b0 = token[0];
			b1 = token[1];
			b2 = token[2];
			b3 = token[3];
			c = ((b0 & 0x7) << 18) | ((b1 & 0x3F) << 12) | ((b2 & 0x3F) << 6) | (b3 & 0x3F);
			if (c < 0x10000 || c > 0x10FFFF)
				c = ALTER_CHAR;
		}else
		{
			// Handle other byte sequences (never valid).
			break;
		}

		if(dest)
			dest[index] = (wchar_t)c;

		index ++;
		if(dest && index >= dlen)
			break;

		token += len;

		if(token >= src + len)
			break;
	}

	return index;
}

int gb2312_encode(const wchar_t* src,int slen,byte_t* dest,int dlen)
{
	return WideCharToMultiByte(CP_ACP,0,src,slen,(LPSTR)dest,dlen,NULL,NULL);
}

int utf8_encode(const wchar_t* src,int slen,byte_t* dest,int dlen)
{
	int c,index = 0;
	wchar_t* token = (wchar_t*)src;

	if(slen < 0)
		slen = w_xslen(src);

	while(!w_is_null(token))
	{
		c = (int)(*token);
	
		if(c >= 0x00 && c <=0x7F)
		{
			// Handle 1-byte character (plain ASCII).
			//UNICODE 00000000 â€?0000007F
			//UTF8 0xxxxxxx
			if(dest)
			{
				dest[index] = (byte_t)c;
			}
			index ++;
		}else if(c >= 0x80 && c <= 0x7FF)
		{
			// Handle 2-byte character.
			//UNICODE 00000080 â€?000007FF
			//UTF8 110xxxxx 10xxxxxx 

			//xxxx xxxx xxxx xxxx & 0000 0111 1100 0000
			//0000 0xxx xx00 0000 >> 6
			//0000 0000 000x xxxx | 0000 0000 1100 0000
			//0000 0000 110x xxxx
			if(dest)
			{
				dest[index] = (byte_t)(((c & 0x7C0) >> 6) | 0xC0);
			}
			index ++;
			//xxxx xxxx xxxx xxxx & 0000 0000 0011 1111
			//0000 0000 00xx xxxx | 0000 0000 1000 0000
			//0000 0000 10xx xxxx
			if(dest)
			{
				dest[index] = (byte_t)((c & 0x3F) | 0x80);
			}
			index ++;
		}else if(c >= 0x800 && c <= 0xFFFF)
		{
			// Handle 3-byte character.
			//UNICODE 00000800 â€?0000FFFF
			//UTF8 1110xxxx 10xxxxxx 10xxxxxx

			//xxxx xxxx xxxx xxxx & 1111 0000 0000 0000
			//xxxx 0000 0000 0000 >> 12
			//0000 0000 0000 xxxx | 0000 0000 1110 0000
			//0000 0000 1110 xxxx
			if(dest)
			{
				dest[index] = (byte_t)(((c & 0xF000) >> 12) | 0xE0);
			}
			index ++;
			
			//xxxx xxxx xxxx xxxx & 0000 1111 1100 0000
			//0000 xxxx xx00 0000 >> 6
			//0000 0000 00xx xxxx | 0000 0000 1000 0000
			//0000 0000 10xx xxxx
			if(dest)
			{
				dest[index] = (byte_t)(((c & 0xFC0) >> 6) | 0x80);
			}
			index ++;
			
			//xxxx xxxx xxxx xxxx & 0000 0000 0011 1111
			//0000 0000 00xx xxxx | 0000 0000 1000 0000
			//0000 0000 10xx xxxx
			if(dest)
			{
				dest[index] = (byte_t)((c & 0x3F) | 0x80);
			}
			index ++;
		}else if(c >= 0x10000 && c <= 0xFFFFF)
		{
			// Handle 4-bytes character.
			//UNICODE 00010000 â€?001FFFFF
			//UTF8 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx

			//xxxx xxxx xxxx xxxx xxxx & 1100 0000 0000 0000 0000
			//xx00 0000 0000 0000 0000 >> 18
			//0000 0000 0000 0000 00xx | 0000 0000 0000 1111 0000
			//0000 0000 0000 1111 00xx
			if(dest)
			{
				dest[index] = (byte_t)(((c & 0xC0000) >> 18) | 0xF0);
			}
			index ++;
			//xxxx xxxx xxxx xxxx xxxx & 0011 1111 0000 0000 0000
			//00xx xxxx 0000 0000 0000 >> 12
			//0000 0000 0000 00xx xxxx | 0000 0000 0000 1000 0000
			//0000 0000 0000 10xx xxxx
			if(dest)
			{
				dest[index] = (byte_t)(((c & 0x3F000) >> 12) | 0x80);
			}
			index ++;
			//xxxx xxxx xxxx xxxx xxxx & 0000 0000 1111 1100 0000
			//0000 0000 xxxx xx00 0000 >> 6
			//0000 0000 0000 00xx xxxx | 0000 0000 0000 1000 0000
			//0000 0000 0000 10xx xxxx
			if(dest)
			{
				dest[index] = (byte_t)(((c & 0xFC0) >> 6) | 0x80);
			}
			index ++;
			//xxxx xxxx xxxx xxxx xxxx & 0000 0000 0000 0011 1111
			//0000 0000 0000 00xx xxxx | 0000 0000 0000 1000 0000
			//0000 0000 0000 10xx xxxx
			if(dest)
			{
				dest[index] = (byte_t)((c & 0x3F) | 0x80);
			}
			index ++;
		}else
		{
			break;
		}

		if(dest && index >= dlen)
			break;
	
		token += 1;
		if(token > src + slen)
			break;

	}

	return index;
}