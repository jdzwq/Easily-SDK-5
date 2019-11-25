// xLIC.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

typedef enum {
	WAIT_ORGANIZATION_CODE = 0,
	WAIT_ORGANIZATION_NAME = 1,
	WAIT_VENDER_CODE = 2,
	WAIT_VENDER_NAME = 3,
	WAIT_EXPIRED_DATE = 4,

	WAIT_GENERATE = 5,

	WAIT_EXIT = 100
}WAITSTEP;

void _md5_public(const tchar_t* hid, const tchar_t* hname, const tchar_t* htime, tchar_t* buf)
{
	byte_t *in_buf;
	byte_t enc_buf[MD5_SIZE + 1] = { 0 };
	int len, total = 0;

	XDL_ASSERT(buf != NULL);

#ifdef _UNICODE
	len = ucs_to_utf8(hid, -1, NULL, MAX_LONG) + ucs_to_utf8(hname, -1, NULL, MAX_LONG) + ucs_to_utf8(htime, -1, NULL, MAX_LONG);
#else
	len = mbs_to_utf8(hid, -1, NULL, MAX_LONG) + mbs_to_utf8(hname, -1, NULL, MAX_LONG) + mbs_to_utf8(htime, -1, NULL, MAX_LONG);
#endif

	in_buf = (byte_t*)xmem_alloc(len + 1);

#ifdef _UNICODE
	len = ucs_to_utf8(hid, -1, in_buf + total, MAX_LONG);
#else
	len = mbs_to_utf8(hid, -1, in_buf + total, MAX_LONG);
#endif
	total += len;

#ifdef _UNICODE
	len = ucs_to_utf8(hname, -1, in_buf + total, MAX_LONG);
#else
	len = mbs_to_utf8(hname, -1, in_buf + total, MAX_LONG);
#endif
	total += len;

#ifdef _UNICODE
	len = ucs_to_utf8(htime, -1, in_buf + total, MAX_LONG);
#else
	len = mbs_to_utf8(htime, -1, in_buf + total, MAX_LONG);
#endif
	total += len;

	md5(in_buf, total, enc_buf);

	xmem_free(in_buf);

	md5_token(enc_buf, buf);
}

void _md5_private(const tchar_t* hid, const tchar_t* hname, const tchar_t* htime, const tchar_t* cid, const tchar_t* cname, const tchar_t* ctime, tchar_t* buf)
{
	byte_t *in_buf;
	byte_t enc_buf[MD5_SIZE + 1] = { 0 };
	int len, total = 0;

	XDL_ASSERT(buf != NULL);

#ifdef _UNICODE
	len = ucs_to_utf8(hid, -1, NULL, MAX_LONG) + ucs_to_utf8(hname, -1, NULL, MAX_LONG) + ucs_to_utf8(htime, -1, NULL, MAX_LONG) + ucs_to_utf8(cid, -1, NULL, MAX_LONG) + ucs_to_utf8(cname, -1, NULL, MAX_LONG) + ucs_to_utf8(ctime, -1, NULL, MAX_LONG);
#else
	len = mbs_to_utf8(hid, -1, NULL, MAX_LONG) + mbs_to_utf8(hname, -1, NULL, MAX_LONG) + mbs_to_utf8(htime, -1, NULL, MAX_LONG) + mbs_to_utf8(cid, -1, NULL, MAX_LONG) + mbs_to_utf8(cname, -1, NULL, MAX_LONG) + mbs_to_utf8(ctime, -1, NULL, MAX_LONG);
#endif

	in_buf = (byte_t*)xmem_alloc(len + 1);

#ifdef _UNICODE
	len = ucs_to_utf8(hid, -1, in_buf + total, MAX_LONG);
#else
	len = mbs_to_utf8(hid, -1, in_buf + total, MAX_LONG);
#endif
	total += len;

#ifdef _UNICODE
	len = ucs_to_utf8(hname, -1, in_buf + total, MAX_LONG);
#else
	len = mbs_to_utf8(hname, -1, in_buf + total, MAX_LONG);
#endif
	total += len;

#ifdef _UNICODE
	len = ucs_to_utf8(htime, -1, in_buf + total, MAX_LONG);
#else
	len = mbs_to_utf8(htime, -1, in_buf + total, MAX_LONG);
#endif
	total += len;

#ifdef _UNICODE
	len = ucs_to_utf8(cid, -1, in_buf + total, MAX_LONG);
#else
	len = mbs_to_utf8(cid, -1, in_buf + total, MAX_LONG);
#endif
	total += len;

#ifdef _UNICODE
	len = ucs_to_utf8(cname, -1, in_buf + total, MAX_LONG);
#else
	len = mbs_to_utf8(cname, -1, in_buf + total, MAX_LONG);
#endif
	total += len;

#ifdef _UNICODE
	len = ucs_to_utf8(ctime, -1, in_buf + total, MAX_LONG);
#else
	len = mbs_to_utf8(ctime, -1, in_buf + total, MAX_LONG);
#endif
	total += len;

	md5(in_buf, total, enc_buf);

	xmem_free(in_buf);

	md5_token(enc_buf, buf);
}

void generate_license(const tchar_t* organ_code, const tchar_t* organ_name, const tchar_t* vender_code, const tchar_t* vender_name, const tchar_t* expired_date, tchar_t* sz_file)
{
	tchar_t md5_public[MD5_LEN + 1] = { 0 };
	tchar_t md5_private[MD5_LEN + 1] = { 0 };

	_md5_public(organ_code, organ_name, expired_date, md5_public);
	_md5_private(organ_code, organ_name, expired_date, vender_code, vender_name, expired_date, md5_private);

	LINKPTR ptr_xml = create_xml_doc();
	LINKPTR nlk_root, nlk;

	tchar_t sz_token[1024] = { 0 };

	nlk_root = get_xml_dom_node(ptr_xml);
	set_dom_node_name(nlk_root, _T("config"), -1);

	nlk = insert_dom_node(nlk_root, LINK_LAST);
	set_dom_node_name(nlk, _T("licence"), -1);

	nlk = insert_dom_node(nlk_root, LINK_LAST);
	set_dom_node_name(nlk, _T("ORGAINIZATION"), -1);
	xsprintf(sz_token, _T("[%s]%s"), organ_code, organ_name);
	set_dom_node_text(nlk, sz_token, -1);

	nlk = insert_dom_node(nlk_root, LINK_LAST);
	set_dom_node_name(nlk, _T("VENDORS"), -1);
	xsprintf(sz_token, _T("[%s]%s"), vender_code, vender_name);
	set_dom_node_text(nlk, sz_token, -1);

	nlk = insert_dom_node(nlk_root, LINK_LAST);
	set_dom_node_name(nlk, _T("EXIPRED"), -1);
	set_dom_node_text(nlk, expired_date, -1);

	nlk = insert_dom_node(nlk_root, LINK_LAST);
	set_dom_node_name(nlk, _T("PUBLIC-KEY"), -1);
	set_dom_node_text(nlk, md5_public, -1);

	nlk = insert_dom_node(nlk_root, LINK_LAST);
	set_dom_node_name(nlk, _T("PRIVATE-KEY"), -1);
	set_dom_node_text(nlk, md5_private, -1);

	xsprintf(sz_file, _T("%s.lic"), md5_public);

	save_xml_doc_to_file(ptr_xml, NULL, sz_file);

	destroy_xml_doc(ptr_xml);
	ptr_xml = NULL;
}

int _tmain(int argc, _TCHAR* argv[])
{
	xdl_process_init(0);

	tchar_t sz_err[ERR_LEN + 1] = { 0 };
	tchar_t sz_verb[RES_LEN] = { 0 };

	tchar_t sz_organization_code[NUM_LEN] = { 0 };
	tchar_t sz_organization_name[RES_LEN] = { 0 };
	tchar_t sz_vender_code[NUM_LEN] = { 0 };
	tchar_t sz_vender_name[RES_LEN] = { 0 };
	tchar_t sz_expired_date[DATE_LEN] = { 0 };

	tchar_t sz_file[PATH_LEN] = { 0 };

	int step;

	if (argc > 5)
	{
		xscpy(sz_organization_code, argv[1]);
		xscpy(sz_organization_name, argv[2]);
		xscpy(sz_vender_code, argv[3]);
		xscpy(sz_vender_name, argv[4]);
		xscpy(sz_expired_date, argv[5]);

		step = WAIT_GENERATE;
	}
	else if (argc > 4)
	{
		xscpy(sz_organization_code, argv[1]);
		xscpy(sz_organization_name, argv[2]);
		xscpy(sz_vender_code, argv[3]);
		xscpy(sz_vender_name, argv[4]);

		step = WAIT_EXPIRED_DATE;
	}
	else if (argc > 3)
	{
		xscpy(sz_organization_code, argv[1]);
		xscpy(sz_organization_name, argv[2]);
		xscpy(sz_vender_code, argv[3]);

		step = WAIT_VENDER_NAME;
	}
	else if (argc > 2)
	{
		xscpy(sz_organization_code, argv[1]);
		xscpy(sz_organization_name, argv[2]);

		step = WAIT_VENDER_CODE;
	}
	else if (argc > 1)
	{
		xscpy(sz_organization_code, argv[1]);

		step = WAIT_ORGANIZATION_NAME;
	}
	else
	{
		_tprintf(_T("xLIC [organization code | organization name | vender code | vender name | expired date]\n"));

		step = WAIT_ORGANIZATION_CODE;
	}

	while (step != WAIT_EXIT)
	{
		xscpy(sz_verb, _T(""));

		switch (step)
		{
		case WAIT_ORGANIZATION_CODE:
			_tprintf(_T("xLIC need organization code: "));
			_tscanf(_T("%s"), sz_verb);

			if (compare_text(sz_verb, -1, _T("exit"), -1, 1) == 0)
			{
				step = WAIT_EXIT;
			}
			else
			{
				xscpy(sz_organization_code, sz_verb);
				step = WAIT_ORGANIZATION_NAME;
			}
			break;
		case WAIT_ORGANIZATION_NAME:
			_tprintf(_T("xLIC need organization name: "));
			_tscanf(_T("%s"), sz_verb);

			if (compare_text(sz_verb, -1, _T("exit"), -1, 1) == 0)
			{
				step = WAIT_EXIT;
			}
			else
			{
				xscpy(sz_organization_name, sz_verb);
				step = WAIT_VENDER_CODE;
			}
			break;
		case WAIT_VENDER_CODE:
			_tprintf(_T("xLIC need vender code: "));
			_tscanf(_T("%s"), sz_verb);

			if (compare_text(sz_verb, -1, _T("exit"), -1, 1) == 0)
			{
				step = WAIT_EXIT;
			}
			else
			{
				xscpy(sz_vender_code, sz_verb);
				step = WAIT_VENDER_NAME;
			}
			break;
		case WAIT_VENDER_NAME:
			_tprintf(_T("xLIC need vender name: "));
			_tscanf(_T("%s"), sz_verb);

			if (compare_text(sz_verb, -1, _T("exit"), -1, 1) == 0)
			{
				step = WAIT_EXIT;
			}
			else
			{
				xscpy(sz_vender_name, sz_verb);
				step = WAIT_EXPIRED_DATE;
			}
			break;
		case WAIT_EXPIRED_DATE:
			_tprintf(_T("xLIC need expired date: "));
			_tscanf(_T("%s"), sz_verb);

			if (compare_text(sz_verb, -1, _T("exit"), -1, 1) == 0)
			{
				step = WAIT_EXIT;
			}
			else
			{
				xscpy(sz_expired_date, sz_verb);
				step = WAIT_GENERATE;
			}
			break;
		case WAIT_GENERATE:
			generate_license(sz_organization_code, sz_organization_name, sz_vender_code, sz_vender_name, sz_expired_date, sz_file);

			_tprintf(_T("License file exported: %s\n"), sz_file);

			step = WAIT_EXIT;
			break;
		}
	}

	xdl_process_uninit();

	return 0;
}

