/***********************************************************************
	Easily xdl v5.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc stream document

	@module	xdlstm.c | stream implement file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
	@devnote 张文权 2017.01 - 2017.12	v5.0
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
#include "xdlstd.h"


typedef struct _stream_t{
	object_head head;
	xhand_t io;

	PF_STREAM_READ pf_read;
	PF_STREAM_WRITE pf_write;

	int encode;
	int mode;

	dword_t opera_bytes;
	dword_t opera_limit;
}stream_t;

/////////////////////////////////////////////////////////////////////////////////////

bool_t _stream_read_chunk_head(stream_t* pxt, dword_t* pb)
{
	byte_t bsize[INT_LEN + 1] = { 0 };
	dword_t dw;
	int pos = 0;
	bool_t b_rt = 0;

	while (pos < INT_LEN)
	{
		dw = 1;

		b_rt = (*pxt->pf_read)(pxt->io, bsize + pos, &dw);

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

bool_t _stream_read_chunk_tail(stream_t* pxt)
{
	byte_t bsize[3] = { 0 };
	dword_t dw = 2;

	if (!(*pxt->pf_read)(pxt->io, bsize, &dw))
	{
		return 0;
	}

	//last \r\n
	return (bsize[0] == '\r' && bsize[1] == '\n') ? 1 : 0;
}

bool_t _stream_write_chunk_head(stream_t* pxt, dword_t dw)
{
	byte_t bsize[INT_LEN + 1] = { 0 };
	dword_t len = 0;

	len = a_ltohex(dw, _T('x'), (schar_t*)bsize, INT_LEN);

	bsize[len] = '\r';
	bsize[len + 1] = '\n';
	len += 2;

	return (*pxt->pf_write)(pxt->io, bsize, &len);
}

bool_t _stream_write_chunk_tail(stream_t* pxt)
{
	byte_t bsize[3] = { 0 };
	dword_t dw = 0;

	bsize[0] = '\r';
	bsize[1] = '\n';
	dw = 2;

	return (*pxt->pf_write)(pxt->io, bsize, &dw);
}

void _stream_begin_write(stream_t* pxt, dword_t* pd)
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

void _stream_end_write(stream_t* pxt, dword_t dw)
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

void _stream_begin_read(stream_t* pxt, dword_t* pd)
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

void _stream_end_read(stream_t* pxt, dword_t dw)
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

bool_t _stream_read(stream_t* pxt, byte_t* buf, dword_t* pb)
{
	dword_t size, pos = 0;

	if (pxt->opera_limit && pxt->opera_bytes == pxt->opera_limit)
	{
		*pb = 0;
		return 1;
	}

	size = *pb;

	while (pos < size)
	{
		*pb = size - pos;

		_stream_begin_read(pxt, pb);

		if(!(*pxt->pf_read)(pxt->io, buf + pos, pb))
		{
			*pb = 0;
			return 0;
		}

		_stream_end_read(pxt, *pb);

		if (!(*pb))
			break;

		pos += *pb;
	}

	*pb = pos;

	return 1;
}

bool_t _stream_write(stream_t* pxt, byte_t* buf, dword_t* pb)
{
	dword_t size, pos = 0;

	if (pxt->opera_limit && pxt->opera_bytes == pxt->opera_limit)
	{
		*pb = 0;
		return 1;
	}

	size = *pb;

	while (pos < size)
	{
		*pb = size - pos;

		_stream_begin_write(pxt, pb);

		if(!(*pxt->pf_write)(pxt->io, buf + pos, pb))
		{
			*pb = 0;
			return 0;
		}

		_stream_end_write(pxt, *pb);
	
		if (!(*pb))
			break;

		pos += *pb;
	}

	*pb = pos;

	return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////
object_t stream_alloc(xhand_t io)
{
	stream_t* pxt;

	pxt = (stream_t*)xmem_alloc(sizeof(stream_t));
	pxt->head.obj = _OBJECT_STREAM;
	pxt->head.cbs = sizeof(stream_t);

	pxt->io = io;
	pxt->mode = 0;

	pxt->opera_limit = 0;
	pxt->opera_bytes = 0;

	if (io)
	{
		switch (io->tag)
		{
		case _HANDLE_FILE:
#ifdef XDL_SUPPORT_FILE
			pxt->pf_read = xfile_read;
			pxt->pf_write = xfile_write;
#endif
			break;
		case _HANDLE_SOCKET:
#ifdef XDL_SUPPORT_SOCK
			pxt->pf_read = xsocket_read;
			pxt->pf_write = xsocket_write;
#endif
			break;
#ifdef XDL_SUPPORT_PIPE
		case _HANDLE_PIPE:
			pxt->pf_read = xpipe_read;
			pxt->pf_write = xpipe_write;
			break;
#endif
#ifdef XDL_SUPPORT_CONS
		case _HANDLE_CONSOLE:
			pxt->pf_read = xcons_read;
			pxt->pf_write = xcons_write;
			break;
#endif
#ifdef XDL_SUPPORT_BLOCK
		case _HANDLE_BLOCK:
			pxt->pf_read = xblock_read;
			pxt->pf_write = xblock_write;
			break;
#endif
#ifdef XDL_SUPPORT_CACHE
		case _HANDLE_CACHE:
			pxt->pf_read = xcache_read;
			pxt->pf_write = xcache_write;
			break;
#endif
		}
	}

	return (object_t)pxt;
}

xhand_t stream_free(object_t xs)
{
	stream_t* pxt = (stream_t*)xs;
	xhand_t io;

	XDL_ASSERT(xs && xs->obj == _OBJECT_STREAM);

	io = pxt->io;

	xmem_free(pxt);
	return io;
}

xhand_t stream_fetch(object_t xs)
{
	stream_t* pxt = (stream_t*)xs;

	XDL_ASSERT(xs && xs->obj == _OBJECT_STREAM);

	return pxt->io;
}

xhand_t stream_attach(object_t xs, xhand_t io, PF_STREAM_READ pf_read, PF_STREAM_WRITE pf_write)
{
	stream_t* pxt = (stream_t*)xs;
	xhand_t io_org;

	XDL_ASSERT(xs && xs->obj == _OBJECT_STREAM);

	stream_opera_reset(xs);

	io_org = pxt->io;
	pxt->io = io;
	pxt->pf_read = pf_read;
	pxt->pf_write = pf_write;

	return io_org;
}

xhand_t stream_detach(object_t xs)
{
	stream_t* pxt = (stream_t*)xs;
	xhand_t io_org;

	XDL_ASSERT(xs && xs->obj == _OBJECT_STREAM);

	stream_opera_reset(xs);

	io_org = pxt->io;
	pxt->io = NULL;
	pxt->pf_read = NULL;
	pxt->pf_write = NULL;

	return io_org;
}

void stream_set_encode(object_t xs, int encode)
{
	stream_t* pxt = (stream_t*)xs;

	XDL_ASSERT(xs && xs->obj == _OBJECT_STREAM);

	pxt->encode = encode;
}

int stream_get_encode(object_t xs)
{
	stream_t* pxt = (stream_t*)xs;

	XDL_ASSERT(xs && xs->obj == _OBJECT_STREAM);

	return pxt->encode;
}

void stream_set_mode(object_t xs, int mode)
{
	stream_t* pxt = (stream_t*)xs;

	XDL_ASSERT(xs && xs->obj == _OBJECT_STREAM);

	pxt->mode = mode;

	pxt->opera_limit = 0;
	pxt->opera_bytes = 0;
}

int stream_get_mode(object_t xs)
{
	stream_t* pxt = (stream_t*)xs;

	XDL_ASSERT(xs && xs->obj == _OBJECT_STREAM);

	return pxt->mode;
}

void stream_set_size(object_t xs, dword_t dw)
{
	stream_t* pxt = (stream_t*)xs;

	XDL_ASSERT(xs && xs->obj == _OBJECT_STREAM);

	if (pxt->mode != CHUNK_OPERA)
	{
		pxt->opera_limit = dw;
		pxt->opera_bytes = 0;
	}
}

dword_t stream_get_size(object_t xs)
{
	stream_t* pxt = (stream_t*)xs;

	XDL_ASSERT(xs && xs->obj == _OBJECT_STREAM);

	return (pxt->mode == CHUNK_OPERA)? 0 : (pxt->opera_limit - pxt->opera_bytes);
}

void stream_opera_reset(object_t xs)
{
	stream_t* pxt = (stream_t*)xs;

	XDL_ASSERT(xs && xs->obj == _OBJECT_STREAM);

	pxt->opera_bytes = 0;
	pxt->opera_limit = 0;
}

bool_t stream_write_char(object_t xs, const tchar_t* buf, dword_t* pch)
{
	stream_t* pxt = (stream_t*)xs;

	byte_t ba[4];
	dword_t bs;
	bool_t rt = 1;
	int pos = 0;

	XDL_ASSERT(xs && xs->obj == _OBJECT_STREAM);
	XDL_ASSERT(pxt->io != NULL);

	if (pxt->encode == _GB2312)
	{
#ifdef _UNICODE
		bs = ucs_byte_to_gb2312(*(buf), ba);
#else
		bs = mbs_byte_to_gb2312(buf, ba);
#endif
	}
	else if (pxt->encode == _UTF8)
	{
#ifdef _UNICODE
		bs = ucs_byte_to_utf8(*(buf), ba);
#else
		bs = mbs_byte_to_utf8(buf, ba);

#endif
	}
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

bool_t	stream_write(object_t xs,const tchar_t* buf,int len,dword_t* pch)
{
	stream_t* pxt = (stream_t*)xs;

	byte_t ba[4];
	dword_t bs;
	dword_t total = 0;
	int pos = 0;
	bool_t rt = 1;;

	XDL_ASSERT(xs && xs->obj == _OBJECT_STREAM);
	XDL_ASSERT(pxt->io != NULL);

	if (len < 0)
		len = xslen(buf);

	total = 0;
	pos = 0;
	while (pos < len)
	{
		if (pxt->encode == _GB2312)
		{
#ifdef _UNICODE
			bs = ucs_byte_to_gb2312(*(buf + pos), ba);
			pos += ucs_sequence(*(buf + pos));
#else
			bs = mbs_byte_to_gb2312(buf + pos, ba);
			pos += mbs_sequence(*(buf + pos));
#endif
		}
		else if (pxt->encode == _UTF8)
		{
#ifdef _UNICODE
			bs = ucs_byte_to_utf8(*(buf + pos), ba);
			pos += ucs_sequence(*(buf + pos));
#else
			bs = mbs_byte_to_utf8(buf + pos, ba);
			pos += mbs_sequence(*(buf + pos));
#endif
		}
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

bool_t stream_read_char(object_t xs, tchar_t* buf, dword_t* pb)
{
	stream_t* pxt = (stream_t*)xs;

	byte_t ba[4];
	dword_t bs;
	bool_t rt = 1;
	int pos = 0;

	XDL_ASSERT(xs && xs->obj == _OBJECT_STREAM);
	XDL_ASSERT(pxt->io != NULL);

	bs = 1;
	rt = _stream_read(pxt, ba, &bs);

	if (!rt)
		goto errret;

	if (bs)
	{
		if (pxt->encode == _GB2312)
		{
			bs = gb2312_sequence(ba[0]);
		}
		else if (pxt->encode == _UTF8)
		{
			bs = utf8_sequence(ba[0]);
		}
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

		bs++;

		if (!rt)
			goto errret;

		if (pxt->encode == _GB2312)
		{
#ifdef _UNICODE
			pos = gb2312_byte_to_ucs(ba, buf);
#else
			pos = gb2312_byte_to_mbs(ba, buf);
#endif
		}
		else if (pxt->encode == _UTF8)
		{
#ifdef _UNICODE
			pos = utf8_byte_to_ucs(ba, buf);
#else
			pos = utf8_byte_to_mbs(ba, buf);
#endif
		}
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
	}

errret:

	if (pb)
	{
		*pb = (rt) ? bs : 0;
	}

	return rt;
}

bool_t	stream_read(object_t xs, tchar_t* buf, int len, dword_t* pch)
{
	stream_t* pxt = (stream_t*)xs;

	byte_t ba[4];
	dword_t bs, total = 0;
	int pos;
	bool_t rt = 1;

	XDL_ASSERT(xs && xs->obj == _OBJECT_STREAM);
	XDL_ASSERT(pxt->io != NULL);

	total = 0;
	pos = 0;
	while (pos < len)
	{
		bs = 1;
		rt = _stream_read(pxt, ba, &bs);

		if (!rt || !bs)
			break;

		if (pxt->encode == _GB2312)
		{
			bs = gb2312_sequence(ba[0]);
		}
		else if (pxt->encode == _UTF8)
		{
			bs = utf8_sequence(ba[0]);
		}
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

		bs++;

		if (!rt)
			break;

		if (pxt->encode == _GB2312)
		{
#ifdef _UNICODE
			pos += gb2312_byte_to_ucs(ba, buf + pos);
#else
			pos += gb2312_byte_to_mbs(ba, buf + pos);
#endif
		}
		else if (pxt->encode == _UTF8)
		{
#ifdef _UNICODE
			pos += utf8_byte_to_ucs(ba, buf + pos);
#else
			pos += utf8_byte_to_mbs(ba, buf + pos);
#endif
		}
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

bool_t stream_write_escape(object_t xs, tchar_t ch, dword_t* pb)
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

bool_t stream_read_escape(object_t xs, tchar_t* buf, dword_t* pb)
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

bool_t stream_write_bytes(object_t xs, const byte_t* buf, dword_t len, dword_t* pb)
{
	stream_t* pxt = (stream_t*)xs;
	dword_t pos;
	bool_t rt = 1;

	XDL_ASSERT(xs && xs->obj == _OBJECT_STREAM);
	XDL_ASSERT(pxt->io != NULL);

	if (!buf || !len)
	{
		if (pb)
			*pb = 0;

		return 1;
	}

	pos = len;
	rt = _stream_write(pxt, (byte_t*)buf, &pos);

	if (pb)
	{
		*pb = (rt) ? pos : 0;
	}

	return rt;
}

bool_t stream_read_bytes(object_t xs, byte_t* buf, dword_t len,dword_t* pb)
{
	stream_t* pxt = (stream_t*)xs;
	dword_t pos;
	bool_t rt = 1;

	XDL_ASSERT(xs && xs->obj == _OBJECT_STREAM);
	XDL_ASSERT(pxt->io != NULL);

	if (!buf || !len)
	{
		if (pb)
			*pb = 0;

		return 1;
	}

	pos = len;
	rt = _stream_read(pxt, buf, &pos);

	if (pb)
	{
		*pb = (rt) ? pos : 0;
	}

	return rt;
}

bool_t stream_read_chunk_size(object_t xs, dword_t* pb)
{
	stream_t* pxt = (stream_t*)xs;

	XDL_ASSERT(xs && xs->obj == _OBJECT_STREAM);
	XDL_ASSERT(pxt->io != NULL);

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
		pxt->opera_bytes = 0;
	}

	if (!pxt->opera_limit)
	{
		_stream_read_chunk_tail(pxt);
	}

	*pb = pxt->opera_limit - pxt->opera_bytes;

	return 1;
}

bool_t stream_write_chunk_size(object_t xs, dword_t dw)
{
	stream_t* pxt = (stream_t*)xs;

	XDL_ASSERT(xs && xs->obj == _OBJECT_STREAM);
	XDL_ASSERT(pxt->io != NULL);

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

bool_t stream_write_utfbom(object_t xs, dword_t* pb)
{
	stream_t* pxt = (stream_t*)xs;

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

bool_t stream_read_utfbom(object_t xs, dword_t* pb)
{
	stream_t* pxt = (stream_t*)xs;

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

bool_t stream_write_zero(object_t xs, dword_t* pb)
{
	stream_t* pxt = (stream_t*)xs;

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

bool_t stream_read_line(object_t xs, object_t vs, dword_t* pb)
{
	tchar_t sch[CHS_LEN + 1] = { 0 };
	byte_t* buf = NULL;
	dword_t dw,total = 0;
	bool_t rt = 1;
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
			rt = stream_read_bytes(xs, buf, dw, &total);

			if (rt)
			{
				varstr_decode(vs, encode, buf, total);
			}

			xmem_free(buf);
			buf = NULL;
		}
	}
	else if (mode == LINE_OPERA)
	{
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
			
			if (sch[0] == _T('\n') || sch[0] == _T('\0'))
				break;
		}
	}
	else
	{
		dw = stream_get_size(xs);
		if (dw)
		{
			buf = (byte_t*)xmem_alloc(dw);
			rt = stream_read_bytes(xs, buf, dw, &total);

			if (rt)
			{
				varstr_decode(vs, encode, buf, total);
			}

			xmem_free(buf);
			buf = NULL;
		}
	}

	if (pb)
	{
		*pb = (rt) ? total : 0;
	}

	return rt;
}

bool_t stream_write_line(object_t xs, object_t vs, dword_t* pb)
{
	dword_t size,dw = 0;
	byte_t* buf = NULL;
	int mode,encode;
	tchar_t ch = 0;
	bool_t rt = 1;

	encode = stream_get_encode(xs);
	mode = stream_get_mode(xs);

	size = varstr_encode(vs, encode, NULL, MAX_LONG);

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
		rt = stream_write_bytes(xs, buf, size, &dw);

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

dword_t stream_printf(object_t xs, const tchar_t* fmt, ...)
{
	tchar_t *buf;
	int len;
	dword_t dw = 0;
	va_list arg;

	va_start(arg, fmt);

	len = xsprintf_arg(NULL, fmt, arg);
	buf = xsalloc(len + 1);
	xsprintf_arg(buf, fmt, arg);

	stream_write(xs, buf, len, &dw);

	xsfree(buf);

	va_end(arg);

	return dw;
}

bool_t _stream_copy_chars(object_t xs_src, object_t xs_dst)
{
	object_t vs = NULL;
	
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

	return 1;
}

bool_t _stream_copy_bytes(object_t xs_src, object_t xs_dst)
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
			if (!stream_read_bytes(xs_src, buf, size, &size))
			{
				xmem_free(buf);
				return 0;
			}

			if (!stream_write_bytes(xs_dst, buf, size, &size))
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
			if (!stream_read_bytes(xs_src, buf, size, &size))
			{
				xmem_free(buf);
				return 0;
			}

			if (!stream_write_bytes(xs_dst, buf, size, &size))
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

	return 1;
}

bool_t stream_copy(object_t xs_src, object_t xs_dst)
{
	if (stream_get_mode(xs_src) != LINE_OPERA && stream_get_encode(xs_src) == stream_get_encode(xs_dst))
		return _stream_copy_bytes(xs_src, xs_dst);
	else
		return _stream_copy_chars(xs_src, xs_dst);
}

