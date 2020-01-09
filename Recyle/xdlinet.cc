/*************************************************************
	EasySoft xds v3.0

	(c) 2003-2006 EasySoft Corporation.  All Rights Reserved.

	@doc xds.rtf: xhttp

	@module	xhttp.c | xhttp implement file

	@devnote ÕÅÎÄÈ¨ 2003.01 - 2006.12
*************************************************************/

#include "xdlinet.h"
#include "xdlmem.h"
#include "xdlstr.h"

typedef struct _xmlhttp_t{
	HINTERNET h_sess;
	HINTERNET h_conn;
	HINTERNET h_requ;
}xmlhttp_t;

void xhttp_parse_url(const wchar_t* url,wchar_t** proat,int* prolen,wchar_t** addrat,int* addrlen,wchar_t** portat,int* portlen,wchar_t** objat,int* objlen,wchar_t** qryat,int* qrylen)
{
	wchar_t* token = (wchar_t*)url;

	*proat = *addrat = *portat = *objat = *qryat = NULL;
	*prolen = *addrlen = *portlen = *objlen = *qrylen = 0;

	if(!token)
		return ;

	/*skip http://*/
	if(token = xsstr(url,L"://"))
	{
		*proat = (wchar_t*)url;
		*prolen = (int)(token - url);
		token += xslen(L"://");
	}else
	{
		token = (wchar_t*)url;
	}
	
	/*get www.aaa.bbb*/
	*addrat = token;
	while(*token != L'\0' && *token != L':' && *token != L'/')
	{
		*addrlen = *addrlen + 1;
		token ++;
	}
	if(*token == L'\0')
		return;
	
	if(*token == L':')
	{
		/*skip ':'*/
		token ++;
		/*get 80*/
		*portat = token;
		while(*token != L'\0' && *token != L'/')
		{
			*portlen = *portlen + 1;
			token ++;
		}
		if(*token == L'\0')
			return;
	}

	/*get somfile.html*/
	*objat = token;
	while(*token != L'\0' && *token != L'?')
	{
		*objlen = *objlen + 1;
		token ++;
	}
	if(*token == L'\0')
		return;

	/*skip '?'*/
	token ++;
	/*get key=val...*/
	*qryat = token;
	while(*token != L'\0')
	{
		*qrylen = *qrylen + 1;
		token ++;
	}
}

handle_t  xhttp_open(const wchar_t* app,const wchar_t* method,const wchar_t* url)
{
	wchar_t *proat,*addrat,*portat,*objat,*qryat;
	int prolen,addrlen,portlen,objlen,qrylen;
	wchar_t sz_prot[RES_LEN + 1] = {0};
	wchar_t sz_addr[RES_LEN + 1] = {0};
	wchar_t sz_port[NUM_LEN + 1] = {0};
	wchar_t sz_object[MAX_PATH] = {0};
	wchar_t sz_query[4096] = {0};
	DWORD dwType = 0;
	HINTERNET hSession,hConnect,hRequest;
	xmlhttp_t* phttp;

	if(is_null(method) || is_null(url))
		return 0;

	hSession = InternetOpen(app,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
    if(!hSession)
		return 0;	
	
	xhttp_parse_url(url,&proat,&prolen,&addrat,&addrlen,&portat,&portlen,&objat,&objlen,&qryat,&qrylen);

	if(prolen)
		xsncpy(sz_prot,proat,prolen);
	else
		xscpy(sz_prot,L"HTTP");

	if(addrlen)
		xsncpy(sz_addr,addrat,addrlen);

	if(portlen)
		xsncpy(sz_port,portat,portlen);
	else
	{
		if(xsicmp(sz_prot,L"HTTP") == 0)
			xsprintf(sz_port,L"%d",INTERNET_DEFAULT_HTTP_PORT);
		else if(xsicmp(sz_prot,L"FTP") == 0)
			xsprintf(sz_port,L"%d",INTERNET_DEFAULT_FTP_PORT);
		else if(xsicmp(sz_prot,L"GOPHER") == 0)
			xsprintf(sz_port,L"%d",INTERNET_DEFAULT_GOPHER_PORT);
	}

	if(objlen)
		xsncpy(sz_object,objat,objlen);

	if(qrylen)
		xsncpy(sz_query,qryat,qrylen);
	
	if(xsicmp(sz_prot,L"HTTP") == 0)
		dwType = INTERNET_SERVICE_HTTP;
	else if(xsicmp(sz_prot,L"FTP") == 0)
		dwType = INTERNET_SERVICE_FTP;
	else if(xsicmp(sz_prot,L"GOPHER") == 0)
		dwType = INTERNET_SERVICE_GOPHER;
	
	hConnect = InternetConnect(hSession,sz_addr,(INTERNET_PORT)xstol(sz_port),NULL,NULL,dwType,0,0);
	if(!hConnect)
	{
		InternetCloseHandle(hSession);
		return 0;
	}

	hRequest = HttpOpenRequest(hConnect,method,sz_object,HTTP_VERSION,NULL,NULL,0,0);
	if(!hRequest)
	{
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		return 0;
	}
	
	phttp = (xmlhttp_t*)xmem_alloc(sizeof(xmlhttp_t));
	phttp->h_sess = hSession;
	phttp->h_conn = hConnect;
	phttp->h_requ = hRequest;

	return (handle_t)phttp;
}

void xhttp_close(handle_t xhttp)
{
	xmlhttp_t* phttp = (xmlhttp_t*)xhttp;

	if(phttp->h_requ)
		InternetCloseHandle(phttp->h_requ);
	if(phttp->h_conn)
		InternetCloseHandle(phttp->h_conn);
	if(phttp->h_sess)
		InternetCloseHandle(phttp->h_sess);

	xmem_free(phttp);
}

bool_t xhttp_send(handle_t xhttp,void* data,int len)
{
	xmlhttp_t* phttp = (xmlhttp_t*)xhttp;

	assert(phttp && phttp->h_requ);

	return (HttpSendRequest(phttp->h_requ,NULL,0,data,len))? 1 : 0;
}

int xhttp_recv_size(handle_t xhttp)
{
	xmlhttp_t* phttp = (xmlhttp_t*)xhttp;
	DWORD dwSize = 0;
	BOOL rt;

	assert(phttp && phttp->h_requ);

	rt = InternetQueryDataAvailable(phttp->h_requ,&dwSize,0,0);
	return (rt)? dwSize : -1;
}

bool_t xhttp_recv(handle_t xhttp,void* buf,int max)
{
	xmlhttp_t* phttp = (xmlhttp_t*)xhttp;
	DWORD dwSize = 0;
	BOOL rt;

	assert(phttp && phttp->h_requ);
	
	rt = InternetReadFile(phttp->h_requ,buf,max,&dwSize);
	return (rt)? 1 : 0;
}
