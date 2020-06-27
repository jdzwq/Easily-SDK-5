/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc xml document

	@module	xmldoc.c | implement file

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

#include "xmldoc.h"
#include "xdlimp.h"

#include "xdlstd.h"
#include "xdldoc.h"

#ifdef XDL_SUPPORT_DOC

typedef struct _xml_doc_t{
	link_t lk;				/* the document self link component */
	link_t_ptr body;		/* xml body data*/

	tchar_t encoding[RES_LEN + 1];
	tchar_t version[RES_LEN + 1];
}xml_doc_t;

/*restore xmldata from self link ptr*/
#define XmlDocFromLink(p) TypePtrFromLink(xml_doc_t,p)

typedef struct _XmlParseParam{
	link_t_ptr ptr;
	link_t_ptr nlk;
}XmlParseParam;

typedef struct _XmlFormatParam{
	link_t_ptr xml;
	link_t_ptr nlk;
	void* pa;
}XmlFormatParam;

link_t_ptr create_xml_doc()
{
	xml_doc_t* ptd;

	ptd = (xml_doc_t*)xmem_alloc(sizeof(xml_doc_t));
	ptd->lk.tag = lkDoc;

	xscpy(ptd->encoding, DEF_XML_ENCODING);
	xscpy(ptd->version, DEF_XML_VERSION);

	ptd->body = create_dom_doc();

	return &(ptd->lk);
}

void destroy_xml_doc(link_t_ptr ptr)
{
	xml_doc_t* ptd;

	XDL_ASSERT(ptr && ptr->tag == lkDoc);

	ptd = XmlDocFromLink(ptr);
	destroy_dom_doc(ptd->body);

	xmem_free(ptd);
}

int get_xml_encode(link_t_ptr ptr)
{
	xml_doc_t* ptd;

	XDL_ASSERT(ptr && ptr->tag == lkDoc);

	ptd = XmlDocFromLink(ptr);

	if (xsicmp(ptd->encoding, CHARSET_GB2312) == 0)
		return _GB2312;
	else if (xsicmp(ptd->encoding, CHARSET_UTF8) == 0)
		return _UTF8;
	else if (xsicmp(ptd->encoding, CHARSET_UTF16) == 0)
		return _UCS2;
	else
		return _UNKNOWN;
}

void set_xml_encode(link_t_ptr ptr, int encode)
{
	xml_doc_t* ptd;

	XDL_ASSERT(ptr && ptr->tag == lkDoc);

	ptd = XmlDocFromLink(ptr);

	switch (encode)
	{
	case _GB2312:
		xscpy(ptd->encoding, CHARSET_GB2312);
		break;
	case _UTF8:
		xscpy(ptd->encoding, CHARSET_UTF8);
		break;
	case _UTF16_LIT:
	case _UTF16_BIG:
		xscpy(ptd->encoding, CHARSET_UTF16);
		break;
	}
}

int get_xml_encoding(link_t_ptr ptr, tchar_t* buf,int max)
{
	xml_doc_t* ptd;
	int len;

	XDL_ASSERT(ptr && ptr->tag == lkDoc);

	ptd = XmlDocFromLink(ptr);

	len = xslen(ptd->encoding);
	len = (len < max) ? len : max;

	if (buf)
	{
		xsncpy(buf, ptd->encoding, len);
	}

	return len;
}

void set_xml_encoding(link_t_ptr ptr, const tchar_t* token,int len)
{
	xml_doc_t* ptd;

	XDL_ASSERT(ptr && ptr->tag == lkDoc);

	ptd = XmlDocFromLink(ptr);

	if (len < 0)
		len = xslen(token);

	len = (len < RES_LEN) ? len : RES_LEN;
	
	xsncpy(ptd->encoding, token,len);
}

int get_xml_version(link_t_ptr ptr, tchar_t* buf, int max)
{
	xml_doc_t* ptd;
	int len;

	XDL_ASSERT(ptr && ptr->tag == lkDoc);

	ptd = XmlDocFromLink(ptr);

	len = xslen(ptd->version);
	len = (len < max) ? len : max;

	if (buf)
	{
		xsncpy(buf, ptd->version, len);
	}
	
	return len;
}

void set_xml_version(link_t_ptr ptr, const tchar_t* token, int len)
{
	xml_doc_t* ptd;

	XDL_ASSERT(ptr && ptr->tag == lkDoc);

	ptd = XmlDocFromLink(ptr);

	if (len < 0)
		len = xslen(token);

	len = (len < RES_LEN) ? len : RES_LEN;

	xsncpy(ptd->version, token, len);
}


link_t_ptr upcast_dom_to_xml(link_t_ptr doc)
{
	xml_doc_t* ptd;

	XDL_ASSERT(doc && doc->tag == lkNode);

	ptd = (xml_doc_t*)xmem_alloc(sizeof(xml_doc_t));
	ptd->lk.tag = lkDoc;

	xscpy(ptd->encoding, DEF_XML_ENCODING);
	xscpy(ptd->version, DEF_XML_VERSION);

	ptd->body = doc;

	return &(ptd->lk);
}

link_t_ptr downcast_xml_to_dom(link_t_ptr xml)
{
	xml_doc_t* ptd;
	link_t_ptr plk;

	XDL_ASSERT(xml && xml->tag == lkDoc);

	ptd = XmlDocFromLink(xml);
	plk = ptd->body;
	xmem_free(ptd);

	return plk;
}

void clear_xml_doc(link_t_ptr ptr)
{
	xml_doc_t* ptd;

	XDL_ASSERT(ptr && ptr->tag == lkDoc);

	ptd = XmlDocFromLink(ptr);

	delete_dom_child_nodes(ptd->body);
}

link_t_ptr get_xml_dom_node(link_t_ptr ptr)
{
	xml_doc_t* ptd;

	XDL_ASSERT(ptr && ptr->tag == lkDoc);

	ptd = XmlDocFromLink(ptr);

	return ptd->body;
}


#endif /*XDL_SUPPORT_DOC*/