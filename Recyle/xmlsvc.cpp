// xmlsvc.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "xmlsvc.h"

DWORD WINAPI HttpExtensionProc(LPEXTENSION_CONTROL_BLOCK lpECB)
{
	wchar_t err_text[4096] = {0};
	wchar_t err_code[NUM_LEN] = {0};

	LINKPTR svc_doc = NULL;
	LINKPTR svc_message = NULL;
	LINKPTR svc_message_faultcode = NULL;
	LINKPTR svc_message_faultstring = NULL;
	LINKPTR svc_declare = NULL;
	LINKPTR svc_declare_interface = NULL;
	LINKPTR svc_context = NULL;
	LINKPTR svc_context_service = NULL;
	LINKPTR svc_context_client = NULL;
	LINKPTR svc_package = NULL;

	LINKPTR cfg_doc = NULL;
	LINKPTR cfg_config = NULL;
	LINKPTR cfg_service = NULL;

	handle_t f_cfg = NULL;

	DWORD dwTotal,dw;
	byte_t* recv_buf = NULL;
	byte_t* send_buf = NULL;
	wchar_t path[MAX_PATH] = {0};
	wchar_t file[MAX_PATH] = {0};
	wchar_t sz_function[1024] = {0};
	wchar_t sz_module[MAX_PATH] = {0};
	wchar_t sz_path[1024] = {0};
	char sz_ascfunc[1024] = {0};

	BOOL rt;
	HINSTANCE h_ext = NULL;
	SvcProcPtr pf_svc = NULL;
	wchar_t encode[MAX_ERR] = {0};

	if(lpECB->lpszPathTranslated)
		MultiByteToWideChar(CP_ACP,0,(LPCSTR)lpECB->lpszPathTranslated,-1,path,MAX_PATH);
	else
		w_xscpy(path,L".");

	w_xsprintf(file,L"%s\\%s",path,L"xmlsvc.config");
	f_cfg = xfile_open(file,L"r");
	if(!f_cfg)
	{
		xscpy(err_code,ERR_NETWORK);
		xscpy(err_text,L"not find xml config file!");
		goto err_ret;
	}

	cfg_doc = create_xml_doc();

	if(!xfile_load_xml(f_cfg,cfg_doc))
	{
		xscpy(err_code,ERR_NETWORK);
		xscpy(err_text,L"read xml config file failed！");
		goto err_ret;
	}
	
	xfile_close(f_cfg);
	f_cfg = NULL;

	dwTotal = lpECB->cbTotalBytes;
	if(dwTotal <= 0)
	{
		xscpy(err_code,ERR_NETWORK);
		xscpy(err_text,L"data is empty！");
		goto err_ret;
	}

	recv_buf = (byte_t*)xmem_alloc((dwTotal + 1) * sizeof(byte_t));
	if(lpECB->cbAvailable)
	{
		memcpy((void*)recv_buf,lpECB->lpbData,lpECB->cbAvailable);
	}

	dw = dwTotal - lpECB->cbAvailable;
	if(dwTotal > lpECB->cbAvailable)
	{
		if(!(*lpECB->ReadClient)(lpECB->ConnID,(void*)(recv_buf + lpECB->cbAvailable),&dw))
		{
			xmem_free(recv_buf);
			recv_buf = NULL;

			xscpy(err_code,ERR_NETWORK);
			xscpy(err_text,L"read data failed！");
			goto err_ret;
		}
	}

	svc_doc = create_xml_doc();
	if(!parse_xml_doc(svc_doc,recv_buf,dwTotal))
	{
		xmem_free(recv_buf);
		recv_buf = NULL;

		destroy_xml_doc(svc_doc);
		svc_doc = NULL;

		xscpy(err_code,ERR_DATA);
		xscpy(err_text,L"parse xml document failed！");
		goto err_ret;
	}
	xmem_free(recv_buf);
	recv_buf = NULL;

	svc_declare = get_svc_declare_node(svc_doc);
	svc_message = get_svc_message_node(svc_doc);
	svc_context = get_svc_context_node(svc_doc);
	svc_package = get_svc_package_node(svc_doc);
	
	svc_message_faultcode = find_dom_node_by_name(svc_message,DOC_SVC_MESSAGE_FAULTCODE,-1);
	svc_message_faultstring = find_dom_node_by_name(svc_message,DOC_SVC_MESSAGE_FAULTSTRING,-1);

	if(!svc_declare || !svc_message || !svc_context || !svc_package || !svc_message_faultcode || !svc_message_faultstring)
	{
		destroy_xml_doc(svc_doc);
		svc_doc = NULL;

		xscpy(err_code,ERR_DATA);
		xscpy(err_text,L"not valid svc document！");
		goto err_ret;
	}

	svc_declare_interface = find_dom_node_by_name(svc_declare,DOC_SVC_DECLARE_INTERFACE,-1);
	if(svc_declare_interface)
		get_dom_node_text(svc_declare_interface,sz_function,1024);
	else
		xscpy(sz_function,L"");
	
	if(is_null(sz_function))
	{
		destroy_xml_doc(svc_doc);
		svc_doc = NULL;

		xscpy(err_code,ERR_DATA);
		xscpy(err_text,L"not define interface！");
		goto err_ret;
	}

	cfg_service = NULL;
	cfg_config = get_xml_dom_node(cfg_doc);
	if(cfg_config)
	{
		cfg_service = get_dom_first_child_node(cfg_config);
		while(cfg_service)
		{
			if(compare_text(get_dom_node_name_ptr(cfg_service),-1,CFG_XMLSVC_NODE_SERVICE,-1,1) == 0)
			{
				if(compare_text(get_dom_node_attr_ptr(cfg_service,CFG_XMLSVC_ATTR_INTERFACE,-1),-1,sz_function,-1,1) == 0)
					break;
			}	

			cfg_service = get_dom_next_sibling_node(cfg_service);
		}
	}
	
	if(cfg_service)
	{
		get_dom_node_attr(cfg_service,CFG_XMLSVC_ATTR_MODULE,-1,sz_module,1024);
		get_dom_node_attr(cfg_service,CFG_XMLSVC_ATTR_PATH,-1,sz_path,MAX_PATH);
	}else
	{
		xscpy(sz_module,L"");
		xscpy(sz_path,L"");
	}

	destroy_xml_doc(cfg_doc);
	cfg_doc = NULL;

	if(is_null(sz_module))
	{
		destroy_xml_doc(svc_doc);
		svc_doc = NULL;

		xscpy(err_code,ERR_DATA);
		w_xsprintf(err_text,L"[%s]not define xml service module！",sz_module);
		goto err_ret;
	}

	if(xsstr(sz_path,L":"))
		w_xsprintf(file,L"%s",sz_path);
	else if(xsstr(sz_path,L".."))
		w_xsprintf(file,L"%s\\%s",path,sz_path + 3);
	else if(xsstr(sz_path,L"."))
		w_xsprintf(file,L"%s\\%s",path,sz_path + 2);
	else
		w_xsprintf(file,L"%s\\%s",path,sz_path);

	svc_context_service = find_dom_node_by_name(svc_context,DOC_SVC_CONTEXT_SERVICE,-1);
	if(svc_context_service)
	{
		set_dom_node_attr(svc_context_service,SVC_ATTR_IISPATH,-1,path,-1);
		set_dom_node_attr(svc_context_service,SVC_ATTR_SVCPATH,-1,file,-1);
	}

	xscat(file,L"\\");
	xscat(file,sz_module);
	
	h_ext = LoadLibrary(file);
	if(!h_ext)
	{
		destroy_xml_doc(svc_doc);
		svc_doc = NULL;

		xscpy(err_code,ERR_DATA);
		w_xsprintf(err_text,L"[%s]load service file failed！",file);
		goto err_ret;
	}

	WideCharToMultiByte(CP_ACP,0,sz_function,-1,sz_ascfunc,1024,NULL,NULL);

	pf_svc = (SvcProcPtr)GetProcAddress(h_ext,sz_ascfunc);
	if(!pf_svc)
	{
		destroy_xml_doc(svc_doc);
		svc_doc = NULL;

		xscpy(err_code,ERR_DATA);
		w_xsprintf(err_text,L"[%s]not find %s function！",sz_module,sz_function);
		goto err_ret;
	}

	(*pf_svc)(svc_doc);

	FreeLibrary(h_ext);
	h_ext = NULL;

err_ret:

	if(f_cfg)
		xfile_close(f_cfg);
	f_cfg = NULL;

	if(cfg_doc)
		destroy_xml_doc(cfg_doc);
	cfg_doc = NULL;

	if(h_ext)
		FreeLibrary(h_ext);
	h_ext = NULL;

	if(recv_buf)
		xmem_free(recv_buf);
	recv_buf = NULL;

	if(!svc_doc)
	{
		svc_doc = create_svc_doc();
		svc_message = get_svc_message_node(svc_doc);
		svc_message_faultcode = find_dom_node_by_name(svc_message,DOC_SVC_MESSAGE_FAULTCODE,-1);
		svc_message_faultstring = find_dom_node_by_name(svc_message,DOC_SVC_MESSAGE_FAULTSTRING,-1);

		set_dom_node_text(svc_message_faultcode,err_code,-1);
		set_dom_node_text(svc_message_faultstring,err_text,-1);
	}

	dwTotal = format_xml_doc_length(svc_doc);
	send_buf = (byte_t*)xmem_alloc((dwTotal + 1) * sizeof(char));

	format_xml_doc(svc_doc,send_buf,dwTotal);

	destroy_xml_doc(svc_doc);
	svc_doc = NULL;
	
	dw = dwTotal + 1;
	rt = (*lpECB->WriteClient)(lpECB->ConnID,(void*)send_buf,&dw,HSE_IO_SYNC);

	xmem_free(send_buf);
	send_buf = NULL;

	lpECB->dwHttpStatusCode = ((rt)? HSE_STATUS_SUCCESS : HSE_STATUS_ERROR);
	return (rt)? HSE_STATUS_SUCCESS : HSE_STATUS_ERROR;
}

BOOL WINAPI GetExtensionVersion(HSE_VERSION_INFO* pVer)
{
	pVer->dwExtensionVersion = MAKELONG(HSE_VERSION_MAJOR,HSE_VERSION_MINOR);
	a_xscpy(pVer->lpszExtensionDesc,"XMLSVC Extension Unicode");
	return TRUE;
}

BOOL WINAPI TerminateExtension( DWORD dwFlags)
{
	return TRUE;
}

