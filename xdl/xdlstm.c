/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc stream document

	@module	xdlstm.c | stream implement file

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

#include "xdlstm.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdlnet.h"

typedef struct _radstm_t{
	int encode;
	int mode;
	dword_t opera_bytes;
	dword_t opera_limit;

	if_bio_t inf;
	void* pa;
}radstm_t;

/////////////////////////////////////////////////////////////////////////////////////

bool_t _stream_read_chunk_head(radstm_t* pxt, dword_t* pb)
{
	byte_t bsize[INT_LEN + 1] = { 0 };
	dword_t dw;
	int pos = 0;
	bool_t b_rt = 0;

	while (pos < INT_LEN)
	{
		dw = 1;

		b_rt = (*pxt->inf.pf_read)(pxt->inf.bio, bsize + pos, &dw);

		if (pos && bsize[pos - 1] == '\r' && bsize[pos] == '\n')
		{
			bsize[pos - 1] = '\0';
			b_rt = 1;
			break;
		}

		if (!b_rt || !dw)
			break;

		pos++;
	}

	if (b_rt)
	{
		if (pb)
			*pb = (dword_t)a_hextol((schar_t*)bsize);
	}
	else
	{
		if (pb)
			*pb = 0;
	}

	return b_rt;
}

bool_t _stream_read_chunk_tail(radstm_t* pxt)
{
	byte_t bsize[3] = { 0 };
	dword_t bys, pos = 0;

    while(pos < 2)
    {
        bys = 1;
        if (!(*pxt->inf.pf_read)(pxt->inf.bio, bsize + pos, &bys))
        {
            return 0;
        }
        if(!bys)
            break;
        
        pos += bys;
    }

	//last \r\n
	return (bsize[0] == '\r' && bsize[1] == '\n') ? 1 : 0;
}

bool_t _stream_write_chunk_head(radstm_t* pxt, dword_t dw)
{
	byte_t bsize[INT_LEN + 1] = { 0 };
	dword_t len, bys, pos = 0;

	len = a_ltohex(dw, _T('x'), (schar_t*)bsize, INT_LEN);

	bsize[len] = '\r';
	bsize[len + 1] = '\n';
	len += 2;

    while(pos < len)
    {
        bys = 1;
        if(!(*pxt->inf.pf_write)(pxt->inf.bio, bsize + pos, &bys))
        {
            return 0;
        }
        if(!bys)
            break;
        
        pos += bys;
    }
    
    return (pos == len)? 1 : 0;
}

bool_t _stream_write_chunk_tail(radstm_t* pxt)
{
	byte_t bsize[3] = { 0 };
	dword_t bys, pos = 0;

	bsize[0] = '\r';
	bsize[1] = '\n';

    while(pos < 2)
    {
        bys = 1;
        if(!(*pxt->inf.pf_write)(pxt->inf.bio, bsize, &bys))
        {
            return 0;
        }
        if(!bys)
            break;
        
        pos += bys;
    }
    
    return (pos == 2)? 1 : 0;
}

void _stream_begin_write(radstm_t* pxt, dword_t* pd)
{
	dword_t dw = 0;

	if (pxt->mode == CHUNK_OPERA)
	{
		dw = pxt->opera_limit - pxt->opera_bytes;

		if (!dw)
		{
			if (!_stream_write_chunk_head(pxt, *pd))
			{
				return;
			}
			pxt->opera_limit = *pd;
			pxt->opera_bytes = 0;
		}

		dw = pxt->opera_limit - pxt->opera_bytes;
		*pd = (dw < *pd) ? dw : *pd;

	}
}

void _stream_end_write(radstm_t* pxt, dword_t dw)
{
	if (pxt->opera_limit)
	{
		pxt->opera_bytes += dw;
	}

	if (pxt->mode == CHUNK_OPERA)
	{
		if (pxt->opera_limit == pxt->opera_bytes)
		{
			if (!_stream_write_chunk_tail(pxt))
			{
				return;
			}
			pxt->opera_limit = 0;
			pxt->opera_bytes = 0;
		}
	}
}

void _stream_begin_read(radstm_t* pxt, dword_t* pd)
{
	dword_t dw = 0;

	if (pxt->mode == CHUNK_OPERA)
	{
		dw = pxt->opera_limit - pxt->opera_bytes;

		if (!dw)
		{
			if (!_stream_read_chunk_head(pxt, &pxt->opera_limit))
			{
				return;
			}
			pxt->opera_bytes = 0;
		}

		dw = pxt->opera_limit - pxt->opera_bytes;
		*pd = (dw < *pd) ? dw : *pd;
	}
}

void _stream_end_read(radstm_t* pxt, dword_t dw)
{
	if (pxt->opera_limit)
	{
		pxt->opera_bytes += dw;
	}

	if (pxt->mode == CHUNK_OPERA)
	{
		if (pxt->opera_limit == pxt->opera_bytes)
		{
			if (!_stream_read_chunk_tail(pxt))
			{
				return;
			}
			pxt->opera_limit = 0;
			pxt->opera_bytes = 0;
		}
	}
}

bool_t _stream_read(radstm_t* pxt, byte_t* buf, dword_t* pb)
{
	dword_t size, dw, pos = 0;

	if (pxt->opera_limit && pxt->opera_bytes == pxt->opera_limit)
	{
		*pb = 0;
		return 1;
	}

	size = *pb;
	while (pos < size)
	{
		dw = size - pos;

		_stream_begin_read(pxt, &dw);

		if(!(*pxt->inf.pf_read)(pxt->inf.bio, buf + pos, &dw))
		{
			break;
		}
		if (pxt->inf.pf_decrypt)
		{
			(*pxt->inf.pf_decrypt)(buf + pos, dw, pxt->pa);
		}

		_stream_end_read(pxt, dw);

		if (!dw)
		{
			*pb = pos;
			return 1;
		}

		pos += dw;
	}
	*pb = pos;

	return (size == pos)? 1 : 0;
}

bool_t _stream_write(radstm_t* pxt, byte_t* buf, dword_t* pb)
{
	dword_t size, dw, pos = 0;

	if (pxt->opera_limit && pxt->opera_bytes == pxt->opera_limit)
	{
		*pb = 0;
		return 1;
	}

	size = *pb;
	while (pos < size)
	{
		dw = size - pos;

		_stream_begin_write(pxt, &dw);

		if (pxt->inf.pf_encrypt)
		{
			(*pxt->inf.pf_encrypt)(buf + pos, dw, pxt->pa);
		}
		if(!(*pxt->inf.pf_write)(pxt->inf.bio, buf + pos, &dw))
		{
			break;
		}

		_stream_end_write(pxt, dw);
	
		if (!dw)
			break;

		pos += dw;
	}
	*pb = pos;

	return (size == pos)? 1 : 0;
}

/************************************************************************************/
stream_t stream_alloc(xhand_t io)
{
	radstm_t* pxt;

	pxt = (radstm_t*)xmem_alloc(sizeof(radstm_t));

	pxt->inf.bio = io;
	pxt->mode = 0;

	pxt->opera_limit = 0;
	pxt->opera_bytes = 0;

	if (!io)
		return (stream_t)pxt;

	switch (io->tag)
	{
	case _HANDLE_PNP:
#ifdef XDK_SUPPORT_SOCK
		pxt->inf.pf_read = xpnp_read;
		pxt->inf.pf_write = xpnp_write;
		pxt->inf.pf_flush = xpnp_flush;
#endif
		break;
	case _HANDLE_TCP:
#ifdef XDK_SUPPORT_SOCK
		pxt->inf.pf_read = xtcp_read;
		pxt->inf.pf_write = xtcp_write;
#endif
		break;
	case _HANDLE_SSL:
#if defined(XDK_SUPPORT_SOCK) && defined(XDL_SUPPORT_CRYPT)
		pxt->inf.pf_read = xssl_read;
		pxt->inf.pf_write = xssl_write;
		pxt->inf.pf_flush = xssl_flush;
#endif
		break;
	case _HANDLE_XSL:
#if defined(XDK_SUPPORT_SOCK) && defined(XDL_SUPPORT_CRYPT)
		pxt->inf.pf_read = xxsl_read;
		pxt->inf.pf_write = xxsl_write;
#endif
		break;
#ifdef XDK_SUPPORT_PIPE
	case _HANDLE_PIPE:
		pxt->inf.pf_read = xpipe_read;
		pxt->inf.pf_write = xpipe_write;
		pxt->inf.pf_flush = xpipe_flush;
		break;
#endif
#ifdef XDK_SUPPORT_COMM
	case _HANDLE_COMM:
		pxt->inf.pf_read = xcomm_read;
		pxt->inf.pf_write = xcomm_write;
		pxt->inf.pf_flush = xcomm_flush;
		break;
#endif
#ifdef XDK_SUPPORT_CONS
	case _HANDLE_CONS:
		pxt->inf.pf_read = xcons_read;
		pxt->inf.pf_write = xcons_write;
		pxt->inf.pf_flush = xcons_flush;
		break;
#endif
#ifdef XDK_SUPPORT_SHARE
	case _HANDLE_SHARE:
		pxt->inf.pf_read = xshare_read;
		pxt->inf.pf_write = xshare_write;
		break;
#endif
#ifdef XDK_SUPPORT_MEMO_CACHE
	case _HANDLE_CACHE:
		pxt->inf.pf_read = vmem_read;
		pxt->inf.pf_write = vmem_write;
		break;
#endif
	case _HANDLE_UNC:
#ifdef XDK_SUPPORT_FILE
		pxt->inf.pf_read = xuncf_read_file;
		pxt->inf.pf_write = xuncf_write_file;
		pxt->inf.pf_flush = xuncf_flush_file;
#endif
		break;
	case _HANDLE_INET:
#if defined(XDK_SUPPORT_SOCK) && defined(XDL_SUPPORT_DOC)
		pxt->inf.pf_read = inet_read_file;
		pxt->inf.pf_write = inet_write_file;
#endif
		break;
	}

	return (stream_t)pxt;
}

bool_t stream_flush(stream_t xs)
{
	radstm_t* pxt = (radstm_t*)xs;

	if (pxt->inf.bio && pxt->inf.pf_flush)
		return (*pxt->inf.pf_flush)(pxt->inf.bio);
	else
		return (pxt->inf.bio)? 1 : 0;
}

xhand_t stream_free(stream_t xs)
{
	radstm_t* pxt = (radstm_t*)xs;
	xhand_t io;

	io = pxt->inf.bio;

	xmem_free(pxt);
	return io;
}

xhand_t stream_fetch(stream_t xs)
{
	radstm_t* pxt = (radstm_t*)xs;

	return pxt->inf.bio;
}

xhand_t stream_attach(stream_t xs, xhand_t io, PF_BIO_READ pf_read, PF_BIO_WRITE pf_write, PF_BIO_FLUSH pf_flush)
{
	radstm_t* pxt = (radstm_t*)xs;
	xhand_t io_org;

	stream_opera_reset(xs);

	io_org = pxt->inf.bio;
	pxt->inf.bio = io;
	pxt->inf.pf_read = pf_read;
	pxt->inf.pf_write = pf_write;
	pxt->inf.pf_flush = pf_flush;

	return io_org;
}

xhand_t stream_detach(stream_t xs)
{
	radstm_t* pxt = (radstm_t*)xs;
	xhand_t io_org;

	stream_opera_reset(xs);

	io_org = pxt->inf.bio;
	pxt->inf.bio = NULL;
	pxt->inf.pf_read = NULL;
	pxt->inf.pf_write = NULL;

	return io_org;
}

void stream_set_crypt(stream_t xs, PF_BIO_ENCRYPT pf_encrypt, PF_BIO_DECRYPT pf_decrypt, void* pa)
{
	radstm_t* pxt = (radstm_t*)xs;

	pxt->inf.pf_encrypt = pf_encrypt;
	pxt->inf.pf_decrypt = pf_decrypt;
	pxt->pa = pa;
}

void stream_set_encode(stream_t xs, int encode)
{
	radstm_t* pxt = (radstm_t*)xs;

	pxt->encode = encode;
}

int stream_get_encode(stream_t xs)
{
	radstm_t* pxt = (radstm_t*)xs;

	return pxt->encode;
}

void stream_set_mode(stream_t xs, int mode)
{
	radstm_t* pxt = (radstm_t*)xs;

	pxt->mode = mode;

	pxt->opera_limit = 0;
	pxt->opera_bytes = 0;
}

int stream_get_mode(stream_t xs)
{
	radstm_t* pxt = (radstm_t*)xs;

	return pxt->mode;
}

void stream_set_size(stream_t xs, dword_t dw)
{
	radstm_t* pxt = (radstm_t*)xs;

	if (pxt->mode != CHUNK_OPERA)
	{
		pxt->opera_limit = dw;
		pxt->opera_bytes = 0;
	}
}

dword_t stream_get_size(stream_t xs)
{
	radstm_t* pxt = (radstm_t*)xs;

	return (pxt->mode == CHUNK_OPERA)? 0 : (pxt->opera_limit - pxt->opera_bytes);
}

void stream_opera_reset(stream_t xs)
{
	radstm_t* pxt = (radstm_t*)xs;

	pxt->opera_bytes = 0;
	pxt->opera_limit = 0;
}

bool_t stream_write_char(stream_t xs, const tchar_t* buf, dword_t* pch)
{
	radstm_t* pxt = (radstm_t*)xs;

	byte_t ba[4] = { 0 };
	dword_t bs;
	bool_t rt = 1;
	int pos = 0;

	XDL_ASSERT(pxt->inf.bio != NULL);

	if (pxt->encode == _UTF8)
	{
#ifdef _UNICODE
		bs = ucs_byte_to_utf8(*(buf), ba);
#else
		bs = mbs_byte_to_utf8(buf, ba);

#endif
	}
#if defined(GPL_SUPPORT_ACP) || defined(XDK_SUPPORT_MBCS)
	else if (pxt->encode == _GB2312)
	{
#ifdef _UNICODE
		bs = ucs_byte_to_gb2312(*(buf), ba);
#else
		bs = mbs_byte_to_gb2312(buf, ba);
#endif
	}
#endif
	else if (pxt->encode == _UTF16_LIT)
	{
#ifdef _UNICODE
		bs = ucs_byte_to_utf16lit(*(buf), ba);
#else
		bs = mbs_byte_to_utf16lit(buf, ba);
#endif
	}
	else if (pxt->encode == _UTF16_BIG)
	{
#ifdef _UNICODE
		bs = ucs_byte_to_utf16big(*(buf), ba);
#else
		bs = mbs_byte_to_utf16big(buf, ba);
#endif
	}
	else
	{
#ifdef _UNICODE
		bs = ucs_byte_to_unn(*(buf), ba);
#else
		bs = mbs_byte_to_unn(buf, ba);
#endif
	}

	if (bs)
		rt = _stream_write(pxt, ba, &bs);
	else
		rt = 1;
	
	if (pch)
	{
		*pch = (rt) ? bs : 0;
	}

#ifdef _UNICODE
	pos = ucs_sequence(*buf);
#else
	pos = mbs_sequence(*buf);
#endif

	return rt;
}

bool_t	stream_write(stream_t xs,const tchar_t* buf,int len,dword_t* pch)
{
	radstm_t* pxt = (radstm_t*)xs;

	byte_t ba[4] = { 0 };
	dword_t bs;
	dword_t total = 0;
	int pos = 0;
	bool_t rt = 1;;

	XDL_ASSERT(pxt->inf.bio != NULL);

	if (len < 0)
		len = xslen(buf);

	total = 0;
	pos = 0;
	while (pos < len)
	{
		if (pxt->encode == _UTF8)
		{
#ifdef _UNICODE
			bs = ucs_byte_to_utf8(*(buf + pos), ba);
			pos += ucs_sequence(*(buf + pos));
#else
			bs = mbs_byte_to_utf8(buf + pos, ba);
			pos += mbs_sequence(*(buf + pos));
#endif
		}
#if defined(GPL_SUPPORT_ACP) || defined(XDK_SUPPORT_MBCS)
		else if (pxt->encode == _GB2312)
		{
#ifdef _UNICODE
			bs = ucs_byte_to_gb2312(*(buf + pos), ba);
			pos += ucs_sequence(*(buf + pos));
#else
			bs = mbs_byte_to_gb2312(buf + pos, ba);
			pos += mbs_sequence(*(buf + pos));
#endif
		}
#endif
		else if (pxt->encode == _UTF16_LIT)
		{
#ifdef _UNICODE
			bs = ucs_byte_to_utf16lit(*(buf + pos), ba);
			pos += ucs_sequence(*(buf + pos));
#else
			bs = mbs_byte_to_utf16lit(buf + pos, ba);
			pos += mbs_sequence(*(buf + pos));
#endif
		}
		else if (pxt->encode == _UTF16_BIG)
		{
#ifdef _UNICODE
			bs = ucs_byte_to_utf16big(*(buf + pos), ba);
			pos += ucs_sequence(*(buf + pos));
#else
			bs = mbs_byte_to_utf16big(buf + pos, ba);
			pos += mbs_sequence(*(buf + pos));
#endif
		}
		else
		{
#ifdef _UNICODE
			bs = ucs_byte_to_unn(*(buf + pos), ba);
			pos += ucs_sequence(*(buf + pos));
#else
			bs = mbs_byte_to_unn(buf + pos, ba);
			pos += mbs_sequence(*(buf + pos));
#endif
		}

		if (bs)
			rt = _stream_write(pxt, ba, &bs);
		else
			rt = 1;

		if (!rt)
			break;

		total += bs;
	}

	if (pch)
	{
		*pch = (rt) ? total : 0;
	}

	return rt;
}

bool_t stream_read_char(stream_t xs, tchar_t* buf, dword_t* pb)
{
	radstm_t* pxt = (radstm_t*)xs;

	byte_t ba[4] = { 0 };
	dword_t bs;
	bool_t rt = 1;
	int pos = 0;

	XDL_ASSERT(pxt->inf.bio != NULL);

	bs = 1;
	rt = _stream_read(pxt, ba, &bs);

	if (!rt || !bs)
		goto errret;

	if (pxt->encode == _UTF8)
	{
		bs = utf8_sequence(ba[0]);
	}
#if defined(GPL_SUPPORT_ACP) || defined(XDK_SUPPORT_MBCS)
	else if (pxt->encode == _GB2312)
	{
		bs = gb2312_sequence(ba[0]);
	}
#endif
	else if (pxt->encode == _UTF16_LIT || pxt->encode == _UTF16_BIG)
	{
		bs = utf16_sequence(ba[0]);
	}
	else
	{
		bs = unn_sequence(ba[0]);
	}

	bs--;

	if (bs > 0)
		rt = _stream_read(pxt, ba + 1, &bs);
	else
		rt = 1;

	if (!rt)
		goto errret;

	bs++;

	if (pxt->encode == _UTF8)
	{
#ifdef _UNICODE
		pos = utf8_byte_to_ucs(ba, buf);
#else
		pos = utf8_byte_to_mbs(ba, buf);
#endif
	}
#if defined(GPL_SUPPORT_ACP) || defined(XDK_SUPPORT_MBCS)
	else if (pxt->encode == _GB2312)
	{
#ifdef _UNICODE
		pos = gb2312_byte_to_ucs(ba, buf);
#else
		pos = gb2312_byte_to_mbs(ba, buf);
#endif
	}
#endif
	else if (pxt->encode == _UTF16_LIT)
	{
#ifdef _UNICODE
		pos = utf16lit_byte_to_ucs(ba, buf);
#else
		pos = utf16lit_byte_to_mbs(ba, buf);
#endif
	}
	else if (pxt->encode == _UTF16_BIG)
	{
#ifdef _UNICODE
		pos = utf16big_byte_to_ucs(ba, buf);
#else
		pos = utf16big_byte_to_mbs(ba, buf);
#endif
	}
	else
	{
#ifdef _UNICODE
		pos = unn_byte_to_ucs(ba, buf);
#else
		pos = unn_byte_to_mbs(ba, buf);
#endif
	}

errret:

	if (pb)
	{
		*pb = (rt) ? bs : 0;
	}

	return rt;
}

bool_t	stream_read(stream_t xs, tchar_t* buf, int len, dword_t* pch)
{
	radstm_t* pxt = (radstm_t*)xs;

	byte_t ba[4] = { 0 };
	dword_t bs, total = 0;
	int pos;
	bool_t rt = 1;

	XDL_ASSERT(pxt->inf.bio != NULL);

	total = 0;
	pos = 0;
	while (pos < len)
	{
		bs = 1;
		rt = _stream_read(pxt, ba, &bs);

		if (!rt || !bs)
			break;

		if (pxt->encode == _UTF8)
		{
			bs = utf8_sequence(ba[0]);
		}
#if defined(GPL_SUPPORT_ACP) || defined(XDK_SUPPORT_MBCS)
		else if (pxt->encode == _GB2312)
		{
			bs = gb2312_sequence(ba[0]);
		}
#endif
		else if (pxt->encode == _UTF16_LIT || pxt->encode == _UTF16_BIG)
		{
			bs = utf16_sequence(ba[0]);
		}
		else
		{
			bs = unn_sequence(ba[0]);
		}

		bs--;

		if (bs > 0)
			rt = _stream_read(pxt, ba + 1, &bs);
		else
			rt = 1;

		if (!rt)
			break;

		bs++;

		if (pxt->encode == _UTF8)
		{
#ifdef _UNICODE
			pos += utf8_byte_to_ucs(ba, buf + pos);
#else
			pos += utf8_byte_to_mbs(ba, buf + pos);
#endif
		}
#if defined(GPL_SUPPORT_ACP) || defined(XDK_SUPPORT_MBCS)
		else if (pxt->encode == _GB2312)
		{
#ifdef _UNICODE
			pos += gb2312_byte_to_ucs(ba, buf + pos);
#else
			pos += gb2312_byte_to_mbs(ba, buf + pos);
#endif
		}
#endif
		else if (pxt->encode == _UTF16_LIT)
		{
#ifdef _UNICODE
			pos += utf16lit_byte_to_ucs(ba, buf + pos);
#else
			pos += utf16lit_byte_to_mbs(ba, buf + pos);
#endif
		}
		else if (pxt->encode == _UTF16_BIG)
		{
#ifdef _UNICODE
			pos += utf16big_byte_to_ucs(ba, buf + pos);
#else
			pos += utf16big_byte_to_mbs(ba, buf + pos);
#endif
		}
		else
		{
#ifdef _UNICODE
			pos += unn_byte_to_ucs(ba, buf + pos);
#else
			pos += unn_byte_to_mbs(ba, buf + pos);
#endif
		}

		total += bs;
	}

	if (pch)
	{
		*pch = (rt) ? total : 0;
	}

	return rt;
}

bool_t stream_write_escape(stream_t xs, tchar_t ch, dword_t* pb)
{
	bool_t rt = 1;
	dword_t pos = 0;

	if (ch == CH_LT)
	{
		rt = stream_write(xs, LT, LT_LEN, &pos);
	}
	else if (ch == CH_GT)
	{
		rt = stream_write(xs, GT, GT_LEN, &pos);
	}
	else if (ch == CH_AMP)
	{
		rt = stream_write(xs, AMP, AMP_LEN, &pos);
	}
	else if (ch == CH_APOS)
	{
		rt = stream_write(xs, APOS, APOS_LEN, &pos);
	}
	else if (ch == CH_QUOT)
	{
		rt = stream_write(xs, QUOT, QUOT_LEN, &pos);
	}
	/*else if (ch == CH_QDIV)
	{
	rt = stream_write(xs,QDIV,QDIV_LEN,&pos);
	}
	else if (ch == CH_PAGE)
	{
		rt = stream_write(xs, PAGE, PAGE_LEN, &pos);
	}*/
	else
	{
		rt = 1;
		pos = 0;
	}

	if (pb)
	{
		*pb = (rt) ? 0 : pos;
	}

	return rt;
}

bool_t stream_read_escape(stream_t xs, tchar_t* buf, dword_t* pb)
{
	tchar_t pch[ESC_LEN + 1] = { 0 };
	int index = 0;
	bool_t rt = 1;
	dword_t pos = 0;
	dword_t total = 0;
	 
	while (index < ESC_LEN)
	{
		rt = stream_read_char(xs, pch + index, &pos);

		if (!rt)
		{
			break;
		}

		index++;
		total += pos;

		if (!xsncmp(pch, LT, LT_LEN))
		{
			if (buf)
				*buf = CH_LT;
			if (pb)
				*pb = total;
			return 1;
		}
		else if (!xsncmp(pch, GT, GT_LEN))
		{
			if (buf)
				*buf = CH_GT;
			if (pb)
				*pb = total;
			return 1;
		}
		else if (!xsncmp(pch, AMP, AMP_LEN))
		{
			if (buf)
				*buf = CH_AMP;
			if (pb)
				*pb = total;
			return 1;
		}
		else if (!xsncmp(pch, APOS, APOS_LEN))
		{
			if (buf)
				*buf = CH_APOS;
			if (pb)
				*pb = total;
			return 1;
		}
		else if (!xsncmp(pch, QUOT, QUOT_LEN))
		{
			if (buf)
				*buf = CH_QUOT;
			if (pb)
				*pb = total;
			return 1;
		}
		/*else if (!xsncmp(pch, QDIV, QDIV_LEN))
		{
		if (buf)
		*buf = CH_QDIV;
		if(pb)
		*pb = total;
		return 1;
		}
		else if (!xsncmp(pch, PAGE, PAGE_LEN))
		{
			if (buf)
				*buf = CH_PAGE;
			if (pb)
				*pb = total;
			return 1;
		}*/
	}

	if (buf)
		xsncpy(buf, pch, index);

	if (pb)
		*pb = total;

	return 1;
}

bool_t stream_write_bytes(stream_t xs, const byte_t* buf, dword_t len)
{
	radstm_t* pxt = (radstm_t*)xs;

	XDL_ASSERT(pxt->inf.bio != NULL);

	if (!buf || !len)
	{
		return 1;
	}

	return _stream_write(pxt, (byte_t*)buf, &len);
}

bool_t stream_read_bytes(stream_t xs, byte_t* buf, dword_t *pb)
{
	radstm_t* pxt = (radstm_t*)xs;

	XDL_ASSERT(pxt->inf.bio != NULL);

	if (!buf || !(*pb))
	{
		return 1;
	}

	return _stream_read(pxt, buf, pb);
}

bool_t stream_write_sword_lit(stream_t xs, sword_t sw)
{
	radstm_t* pxt = (radstm_t*)xs;
	dword_t dw;

	XDL_ASSERT(pxt->inf.bio != NULL);

	dw = 2;
	if (!_stream_write(pxt, (byte_t*)&sw, &dw))
		return 0;

	return (dw == 2) ? 1 : 0;
}

bool_t stream_write_sword_big(stream_t xs, sword_t sw)
{
	radstm_t* pxt = (radstm_t*)xs;
	dword_t dw;
	byte_t ba[2] = { 0 };

	XDL_ASSERT(pxt->inf.bio != NULL);

	PUT_SWORD_BIG(ba, 0, sw);
	dw = 2;

	if (!_stream_write(pxt, ba, &dw))
		return 0;

	return (dw == 2) ? 1 : 0;
}

bool_t stream_read_sword_lit(stream_t xs, sword_t* psw)
{
	radstm_t* pxt = (radstm_t*)xs;
	dword_t dw;

	XDL_ASSERT(pxt->inf.bio != NULL);

	dw = 2;

	if (!_stream_read(pxt, (byte_t*)psw, &dw))
		return 0;

	return (dw == 2) ? 1 : 0;
}

bool_t stream_read_sword_big(stream_t xs, sword_t* psw)
{
	radstm_t* pxt = (radstm_t*)xs;
	dword_t dw;
	byte_t ba[2] = { 0 };

	XDL_ASSERT(pxt->inf.bio != NULL);

	dw = 2;
	if (!_stream_read(pxt, ba, &dw))
		return 0;

	*psw = GET_SWORD_BIG(ba, 0);

	return (dw == 2) ? 1 : 0;
}

bool_t stream_write_dword_lit(stream_t xs, dword_t dw)
{
	radstm_t* pxt = (radstm_t*)xs;
	dword_t n;

	XDL_ASSERT(pxt->inf.bio != NULL);

	n = 4;
	if (!_stream_write(pxt, (byte_t*)&dw, &n))
		return 0;

	return (n == 4) ? 1 : 0;
}

bool_t stream_write_dword_big(stream_t xs, dword_t dw)
{
	radstm_t* pxt = (radstm_t*)xs;
	dword_t n;
	byte_t ba[4] = { 0 };

	XDL_ASSERT(pxt->inf.bio != NULL);

	PUT_DWORD_BIG(ba, 0, dw);

	n = 4;
	if (!_stream_write(pxt, ba, &n))
		return 0;

	return (n == 4) ? 1 : 0;
}

bool_t stream_read_dword_lit(stream_t xs, dword_t* pdw)
{
	radstm_t* pxt = (radstm_t*)xs;
	dword_t n;

	XDL_ASSERT(pxt->inf.bio != NULL);

	n = 4;
	if (!_stream_read(pxt, (byte_t*)pdw, &n))
		return 0;

	return (n == 4) ? 1 : 0;
}

bool_t stream_read_dword_big(stream_t xs, dword_t* pdw)
{
	radstm_t* pxt = (radstm_t*)xs;
	dword_t n;
	byte_t ba[4] = { 0 };

	XDL_ASSERT(pxt->inf.bio != NULL);
	
	n = 4;
	if (!_stream_read(pxt, ba, &n))
		return 0;

	*pdw = GET_DWORD_BIG(ba, 0);
	
	return (n == 4) ? 1 : 0;
}

bool_t stream_read_chunk_size(stream_t xs, dword_t* pb)
{
	radstm_t* pxt = (radstm_t*)xs;

	XDL_ASSERT(pxt->inf.bio != NULL);

	if (pxt->mode != CHUNK_OPERA)
	{
		*pb = 0;
		return 0;
	}

	if (pxt->opera_limit == pxt->opera_bytes)
	{
		if (!_stream_read_chunk_head(pxt, &pxt->opera_limit))
		{
			*pb = 0;
			return 0;
		}
        
        if(pxt->opera_limit > MAX_LONG)
            pxt->opera_limit = 0;
        
		pxt->opera_bytes = 0;
	}

	if (!pxt->opera_limit)
	{
		_stream_read_chunk_tail(pxt);
	}

	*pb = pxt->opera_limit - pxt->opera_bytes;

	return 1;
}

bool_t stream_write_chunk_size(stream_t xs, dword_t dw)
{
	radstm_t* pxt = (radstm_t*)xs;

	XDL_ASSERT(pxt->inf.bio != NULL);

	if (pxt->mode != CHUNK_OPERA)
	{
		return 0;
	}

	if (pxt->opera_limit == pxt->opera_bytes)
	{
		if (!_stream_write_chunk_head(pxt, dw))
		{
			return 0;
		}

		pxt->opera_limit = dw;
		pxt->opera_bytes = 0;
	}

	if (!pxt->opera_limit)
		_stream_write_chunk_tail(pxt);

	return 1;
}

bool_t stream_write_utfbom(stream_t xs, dword_t* pb)
{
	radstm_t* pxt = (radstm_t*)xs;

	byte_t ba[4] = { 0 };
	dword_t len = 0;
	bool_t rt = 1;
	
	len = format_utfbom(pxt->encode, ba);
	if (!len)
	{
		if (pb)
			*pb = 0;

		return 0;
	}

	rt = _stream_write(pxt, ba, &len);
	
	if (pb)
	{
		*pb = (rt) ? len : 0;
	}

	return rt;
}

bool_t stream_read_utfbom(stream_t xs, dword_t* pb)
{
	radstm_t* pxt = (radstm_t*)xs;

	byte_t ba[4] = { 0 };
	dword_t len = 0;
	bool_t rt = 1;
	int encode = 0;

	if (pxt->encode == _UTF16_LIT || pxt->encode == _UTF16_BIG)
	{
		len = 2;
	}
	else if (pxt->encode == _UTF8)
	{
		len = 3;
	}
	else
	{
		len = 0;
	}

	if (!len)
	{
		if (pb)
			*pb = 0;

		return 0;
	}

	rt = _stream_read(pxt, ba, &len);

	if (rt)
	{
		encode = parse_utfbom(ba, len);
		if (encode == _UTF16_LIT || encode == _UTF16_BIG)
		{
			pxt->encode = encode;
		}
	}

	if (pb)
	{
		*pb = (rt) ? len : 0;
	}

	return rt;
}

bool_t stream_write_zero(stream_t xs, dword_t* pb)
{
	radstm_t* pxt = (radstm_t*)xs;

	byte_t ba[3] = { 0 };
	dword_t len = 0;
	bool_t rt = 1;

	if (pxt->encode == _UTF16_LIT || pxt->encode == _UTF16_BIG)
	{
		len = 2;
		rt = _stream_write(pxt, ba, &len);
	}
	else
	{
		len = 1;
		rt = _stream_write(pxt, ba, &len);
	}

	if (pb)
		*pb = (rt) ? len : 0;

	return rt;
}

bool_t stream_read_line(stream_t xs, string_t vs, dword_t* pb)
{
	tchar_t sch[CHS_LEN + 1] = { 0 };
	byte_t* buf = NULL;
	dword_t dw,total = 0;
	bool_t glt, rt = 1;
	int mode,encode;

	encode = stream_get_encode(xs);
	mode = stream_get_mode(xs);

	if (mode == CHUNK_OPERA)
	{
		dw = 0;
		rt = stream_read_chunk_size(xs, &dw);

		if (rt && dw)
		{
			buf = (byte_t*)xmem_alloc(dw);
			rt = stream_read_bytes(xs, buf, &dw);

			if (rt)
			{
				varstr_decode(vs, encode, buf, dw);
			}

			xmem_free(buf);
			buf = NULL;

			total = dw;
		}
	}
	else if (mode == LINE_OPERA)
	{
		glt = 0;
		while (1)
		{
			dw = 0;
			xszero(sch, CHS_LEN + 1);

			rt = stream_read_char(xs, sch, &dw);
			if (!rt)
			{
				break;
			}
			total += dw;

			if (!dw)
				break;

			varstr_cat(vs, sch, -1);

			if (sch[0] == _T('\'') || sch[0] == _T('\"'))
			{
				if (glt)
					glt = 0;
				else
					glt = 1;
			}
			
			if ((!glt && sch[0] == _T('\n')) || sch[0] == _T('\0'))
				break;
		}
	}
	else
	{
		dw = stream_get_size(xs);
		if (dw)
		{
			buf = (byte_t*)xmem_alloc(dw);
			rt = stream_read_bytes(xs, buf, &dw);

			if (rt)
			{
				varstr_decode(vs, encode, buf, dw);
			}

			xmem_free(buf);
			buf = NULL;

			total = dw;
		}
	}

	if (pb)
	{
		*pb = (rt) ? total : 0;
	}

	return rt;
}

bool_t stream_write_line(stream_t xs, string_t vs, dword_t* pb)
{
	dword_t size,dw = 0;
	byte_t* buf = NULL;
	int mode,encode;
	tchar_t ch = 0;
	bool_t rt = 1;

	encode = stream_get_encode(xs);
	mode = stream_get_mode(xs);

	size = (vs)? varstr_encode(vs, encode, NULL, MAX_LONG) : 0;

	if (size)
	{
		buf = (byte_t*)xmem_alloc(size);

		varstr_encode(vs, encode, buf, size);
	}

	if (mode == CHUNK_OPERA)
	{
		stream_write_chunk_size(xs, size);
	}

	if (size)
	{
		rt = stream_write_bytes(xs, buf, size);
		if (rt)
		{
			dw = size;
		}
		xmem_free(buf);
		buf = NULL;
	}
	else
	{
		if (mode ==	LINE_OPERA)
		{
			rt = stream_write_zero(xs, &dw);
		}
		else
		{
			rt = 1;
		}
	}

	if (pb)
		*pb = (rt)? dw : 0;

	return rt;
}

dword_t stream_printf(stream_t xs, const tchar_t* fmt, ...)
{
	tchar_t *buf;
	int len;
	dword_t dw = 0;
	va_list arg;

	va_start(arg, fmt);

	len = xsprintf_arg(NULL, fmt, &arg);
	buf = xsalloc(len + 1);
	xsprintf_arg(buf, fmt, &arg);

	stream_write(xs, buf, len, &dw);

	xsfree(buf);

	va_end(arg);

	return dw;
}

bool_t _stream_copy_chars(stream_t xs_src, stream_t xs_dst)
{
	stream_t vs = NULL;
	
	vs = varstr_alloc();

	while (stream_read_line(xs_src, vs, NULL))
	{
		if (!stream_write_line(xs_dst, vs, NULL))
		{
			varstr_free(vs);
			return 0;
		}

		if (varstr_len(vs) == 0)
			break;

		varstr_empty(vs);
	}

	varstr_free(vs);

	return stream_flush(xs_dst);
}

bool_t _stream_copy_bytes(stream_t xs_src, stream_t xs_dst)
{
	int src_mode, dst_mode;
	dword_t size = 0;
	byte_t* buf = NULL;

	src_mode = stream_get_mode(xs_src);
	dst_mode = stream_get_mode(xs_dst);

	if (src_mode == CHUNK_OPERA)
	{
		while (stream_read_chunk_size(xs_src, &size))
		{
			if (dst_mode == CHUNK_OPERA)
			{
				stream_write_chunk_size(xs_dst, size);
			}
			else if (dst_mode == LINE_OPERA && !size)
			{
				stream_write_zero(xs_dst, NULL);
			}

			if (!size)
			{
				break;
			}

			buf = (byte_t*)xmem_alloc(size);
			if (!stream_read_bytes(xs_src, buf, &size))
			{
				xmem_free(buf);
				return 0;
			}

			if (!stream_write_bytes(xs_dst, buf, size))
			{
				xmem_free(buf);
				return 0;
			}

			xmem_free(buf);
			buf = NULL;
		}
	}
	else 
	{
		size = stream_get_size(xs_src);

		if (dst_mode == CHUNK_OPERA && size)
		{
			stream_write_chunk_size(xs_dst, size);
		}

		if (size)
		{
			buf = (byte_t*)xmem_alloc(size);
			if (!stream_read_bytes(xs_src, buf, &size))
			{
				xmem_free(buf);
				return 0;
			}

			if (!stream_write_bytes(xs_dst, buf, size))
			{
				xmem_free(buf);
				return 0;
			}

			xmem_free(buf);
			buf = NULL;
		}

		if (dst_mode == CHUNK_OPERA)
		{
			stream_write_chunk_size(xs_dst, 0);
		}
		else if (dst_mode == LINE_OPERA)
		{
			stream_write_zero(xs_dst, NULL);
		}
	}

	return stream_flush(xs_dst);
}

bool_t stream_copy(stream_t xs_src, stream_t xs_dst)
{
	if (stream_get_mode(xs_src) != LINE_OPERA && stream_get_encode(xs_src) == stream_get_encode(xs_dst))
		return _stream_copy_bytes(xs_src, xs_dst);
	else
		return _stream_copy_chars(xs_src, xs_dst);
}

