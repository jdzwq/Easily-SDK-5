// xUpdate.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "xUpdate.h"

#include "..\sha\sha1.h"
#include "..\md5\md5.h"
#include "..\bas\base64.h"
#include "..\zlib\zlib.h"

#pragma comment(lib,"sha.lib")
#pragma comment(lib,"md5.lib")
#pragma comment(lib,"bas.lib")
#pragma comment(lib,"zlib.lib")

#pragma comment(lib,"Wininet.lib")


#define XMD5_RESULT_LEN		16
#define XMD5_FORMAT_LEN		32
#define XMD5_BUFFER_LEN		64

#define XSHA1_LEN	20

static char week_day[][7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
static char year_mon[][12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
static int mon_day[13] = { 29, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

HANDLE g_thr[MAXIMUM_WAIT_OBJECTS] = { 0 };
int g_count = 0;

typedef struct _FILEINFO {
	BOOL	is_dir;

	SYSTEMTIME create_time;
	SYSTEMTIME access_time;
	SYSTEMTIME write_time;

	char file_size[48];
	char file_etag[128];
	char file_name[MAX_PATH];
	char path_name[MAX_PATH];
	char dest_path[MAX_PATH];
}FILEINFO;

static int xsha1_hash_bytes(const BYTE* key_buf, int key_len, const BYTE* src_buf, int src_len, BYTE* dig_buf)
{
	sha1_hmac((unsigned char*)key_buf, key_len, (unsigned char*)src_buf, src_len, dig_buf);

	return XSHA1_LEN;
}

static int xmd5_encrypt_bytes(const BYTE* src_buf, int src_len, BYTE* enc_buf)
{
	md5_context mc = { 0 };
	int bytes, len;

	memset((void*)enc_buf, 0, XMD5_RESULT_LEN);
	if (!src_buf || src_len <= 0)
		return 0;

	md5_starts(&mc);

	bytes = 0;
	while (bytes < src_len)
	{
		len = (src_len - bytes < XMD5_BUFFER_LEN) ? src_len - bytes : XMD5_BUFFER_LEN;

		md5_update(&mc, (unsigned char*)(src_buf + bytes), len);
		bytes += XMD5_BUFFER_LEN;
	}

	md5_finish(&mc, enc_buf);

	return XMD5_RESULT_LEN;
}

static int xzlib_compress_bytes(const unsigned char* src_buf, int src_len, unsigned char* zip_buf, int zip_len)
{
	unsigned long zip_size = 0;

	if (!src_buf || src_len <= 0)
		return 0;

	zip_size = zip_len;
	if (Z_OK == compress(zip_buf, &zip_size, src_buf, src_len))
		return zip_size;
	else
		return 0;
}

static int xzlib_uncompress_bytes(const unsigned char* zip_buf, int zip_len, unsigned char* dst_buf, int dst_len)
{
	unsigned long dst_size = 0;

	if (!zip_buf || zip_len <= 0)
		return 0;

	dst_size = dst_len;
	if (Z_OK == uncompress(dst_buf, &dst_size, zip_buf, zip_len))
		return dst_size;
	else
		return 0;
}

static int format_gmttime(const SYSTEMTIME* pdt, char* buf)
{
	return sprintf(buf, "%s, %02d %s %04d %02d:%02d:%02d GMT", week_day[pdt->wDayOfWeek], pdt->wDay, year_mon[pdt->wMonth - 1], pdt->wYear, pdt->wHour, pdt->wMinute, pdt->wSecond);
}

static void parse_gmttime(SYSTEMTIME* pdt, const char* str)
{
	char* token = (char*)str;
	char* key;
	int i, klen;

	key = token;
	klen = 0;
	while (*token != '\0' && *token != ',' && *token != ' ')
	{
		token++;
		klen++;
	}

	for (i = 0; i < 7; i++)
	{
		if (strnicmp(week_day[i], key, klen) == 0)
		{
			pdt->wDayOfWeek = i;
			break;
		}
	}

	while (*token != '\0' && (*token == ',' || *token == ' '))
		token++;

	key = token;
	klen = 0;
	while (*token != '\0' && *token != ',' && *token != ' ')
	{
		token++;
		klen++;
	}

	pdt->wDay = atoi(key);

	while (*token != '\0' && (*token == ',' || *token == ' '))
		token++;

	key = token;
	klen = 0;
	while (*token != '\0' && *token != ',' && *token != ' ')
	{
		token++;
		klen++;
	}

	for (i = 0; i < 12; i++)
	{
		if (strnicmp(year_mon[i], key, klen) == 0)
		{
			pdt->wMonth = i + 1;
			break;
		}
	}

	while (*token != '\0' && (*token == ',' || *token == ' '))
		token++;

	key = token;
	klen = 0;
	while (*token != '\0' && *token != ',' && *token != ' ')
	{
		token++;
		klen++;
	}

	pdt->wYear = atoi(key);

	while (*token != '\0' && (*token == ',' || *token == ' '))
		token++;

	key = token;
	klen = 0;
	while (*token != '\0' && *token != ':')
	{
		token++;
		klen++;
	}

	pdt->wHour = atoi(key);

	if (*token == ':')
		token++;

	key = token;
	klen = 0;
	while (*token != '\0' && *token != ':')
	{
		token++;
		klen++;
	}

	pdt->wMinute = atoi(key);

	if (*token == ':')
		token++;

	key = token;
	klen = 0;
	while (*token != '\0' && *token != ':')
	{
		token++;
		klen++;
	}

	pdt->wSecond = atoi(key);
}

static int compare_systime(const SYSTEMTIME* pmd1, const SYSTEMTIME* pmd2)
{
	if (!pmd1 && !pmd2)
		return 0;
	else if (pmd1 && !pmd2)
		return 1;
	else if (!pmd1 && pmd2)
		return -1;

	if (pmd1->wYear > pmd2->wYear)
		return 1;
	else if (pmd1->wYear < pmd2->wYear)
		return -1;
	else if (pmd1->wMonth > pmd2->wMonth)
		return 1;
	else if (pmd1->wMonth < pmd2->wMonth)
		return -1;
	else if (pmd1->wDay > pmd2->wDay)
		return 1;
	else if (pmd1->wDay < pmd2->wDay)
		return -1;
	else if (pmd1->wHour > pmd2->wHour)
		return 1;
	else if (pmd1->wHour < pmd2->wHour)
		return -1;
	else if (pmd1->wMinute > pmd2->wMinute)
		return 1;
	else if (pmd1->wMinute < pmd2->wMinute)
		return -1;
	else if (pmd1->wSecond > pmd2->wSecond)
		return 1;
	else if (pmd1->wSecond < pmd2->wSecond)
		return -1;
	else
		return 0;
}


//bytes 0-800/801
void ParseRange(const char* szRange, DWORD* pfrom, DWORD* pto, DWORD* ptotal)
{
	char sz_num[50] = { 0 };

	*pfrom = *pto = *ptotal = 0;

	const char* token = szRange;

	while ((*token < '0' || *token > '9') && *token != '\0')
		token++;

	int len = 0;
	while (*token >= '0' && *token <= '9')
	{
		token++;
		len++;
	}

	strncpy(sz_num, token - len, len);
	sz_num[len] = '\0';

	*pfrom = atoi(sz_num);

	while ((*token < '0' || *token > '9') && *token != '\0')
		token++;

	len = 0;
	while (*token >= '0' && *token <= '9')
	{
		token++;
		len++;
	}

	strncpy(sz_num, token - len, len);
	sz_num[len] = '\0';

	*pto = atoi(sz_num);

	while ((*token < '0' || *token > '9') && *token != '\0')
		token++;

	len = 0;
	while (*token >= '0' && *token <= '9')
	{
		token++;
		len++;
	}

	strncpy(sz_num, token - len, len);
	sz_num[len] = '\0';

	*ptotal = atoi(sz_num);
}

DWORD hexntol(const BYTE* token, int n)
{
	unsigned long k = 0;
	long c = 0;
	int pos = 0;

	if (!n)
		return 0;

	if (n && token[0] == '0' && (token[1] == 'x' || token[1] == 'X'))
		pos += 2;

	while (pos < n)
	{
		k *= 16;

		if (token[pos] >= 'a' && token[pos] <= 'z')
			c = (token[pos] - 'a') + 10;
		else if (token[pos] >= 'A' && token[pos] <= 'Z')
			c = (token[pos] - 'A') + 10;
		else if (token[pos] >= '0' && token[pos] <= '9')
			c = (token[pos] - '0');
		else if (token[pos] == '\0')
			break;
		else
			return 0;

		k += c;

		pos++;
	}

	return k;
}

static BOOL ReadChunkHead(HINTERNET hFile, DWORD* pb)
{
	BYTE bsize[16] = { 0 };
	DWORD dw;
	int pos = 0;
	BOOL b_rt = 0;

	while (pos < 16)
	{
		dw = 1;

		b_rt = InternetReadFile(hFile, bsize + pos, dw, &dw);

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
		*pb = hexntol(bsize, 16);
	}
	else
	{
		if (pb)
			*pb = 0;
	}

	return b_rt;
}

BOOL ReadChunkTail(HINTERNET hFile)
{
	BYTE bsize[3] = { 0 };
	DWORD dw = 2;

	if (!InternetReadFile(hFile, bsize, dw, &dw))
	{
		return 0;
	}

	//last \r\n
	return (bsize[0] == '\r' && bsize[1] == '\n') ? 1 : 0;
}

static void parse_fileinfo_from_line(FILEINFO* pfi, const char* token)
{
	int pos = 0;
	char sz_date[36] = { 0 };

	memset((void*)pfi, 0, sizeof(FILEINFO));

	while (*token != '\r' && *token != '\0')
	{
		pos = 0;
		while (*token != '\t' && *token != '\r' && *token != '\0')
		{
			token++;
			pos++;
		}
		pfi->is_dir = (strnicmp(token - pos, "directory", pos) == 0) ? 1 : 0;

		if (*token == '\t')
			token++;

		pos = 0;
		while (*token != '\t' && *token != '\r' && *token != '\0')
		{
			token++;
			pos++;
		}
		strncpy(pfi->file_name, token - pos, pos);

		if (*token == '\t')
			token++;

		pos = 0;
		while (*token != '\t' && *token != '\r' && *token != '\0')
		{
			token++;
			pos++;
		}
		strncpy(sz_date, token - pos, pos);
		parse_gmttime(&pfi->create_time, sz_date);

		if (*token == '\t')
			token++;

		pos = 0;
		while (*token != '\t' && *token != '\r' && *token != '\0')
		{
			token++;
			pos++;
		}
		strncpy(sz_date, token - pos, pos);
		parse_gmttime(&pfi->access_time, sz_date);

		if (*token == '\t')
			token++;

		pos = 0;
		while (*token != '\t' && *token != '\r' && *token != '\0')
		{
			token++;
			pos++;
		}
		strncpy(sz_date, token - pos, pos);
		parse_gmttime(&pfi->write_time, sz_date);

		if (*token == '\t')
			token++;

		pos = 0;
		while (*token != '\t' && *token != '\r' && *token != '\0')
		{
			token++;
			pos++;
		}
		strncpy(pfi->file_size, token - pos, pos);

		if (*token == '\t')
			token++;

		pos = 0;
		while (*token != '\t' && *token != '\r' && *token != '\0')
		{
			token++;
			pos++;
		}
		strncpy(pfi->file_etag, token - pos, pos);

		if (*token == '\t')
			token++;
	}
}

unsigned __stdcall UpdateFile(void* param)
{
	FILEINFO* pfq = (FILEINFO*)param;

	char szAddr[20] = { 0 };
	char szPort[10] = { 0 };
	char szPath[MAX_PATH] = { 0 };
	char szPublic[50] = { 0 };
	char szPrivate[50] = { 0 };

	char szFile[MAX_PATH] = { 0 };

	if (stricmp(pfq->dest_path, ".") == 0)
		printf("Update File: %s\n", pfq->file_name);
	else
		printf("Update Files: %s/%s\n", pfq->dest_path, pfq->file_name);

	sprintf(szFile, "%s\\xUpdate.ini", szRunPath);

	GetPrivateProfileString("server", "addr", "", szAddr, 20, szFile);
	GetPrivateProfileString("server", "port", "", szPort, 10, szFile);
	GetPrivateProfileString("server", "path", "", szPath, MAX_PATH, szFile);
	GetPrivateProfileString("secret", "public", "", szPublic, 50, szFile);
	GetPrivateProfileString("secret", "private", "", szPrivate, 50, szFile);

	HINTERNET hSession = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (!hSession)
	{
		free(pfq);
		return FALSE;
	}

	HINTERNET hConnect = InternetConnect(hSession, szAddr, atoi(szPort), NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	if (!hConnect)
	{
		InternetCloseHandle(hSession);

		free(pfq);
		return FALSE;
	}

	if (strlen(pfq->path_name))
		sprintf(szFile, "%s/%s/%s", szPath, pfq->path_name, pfq->file_name);
	else
		sprintf(szFile, "%s/%s", szPath, pfq->file_name);

	HINTERNET hRequest = HttpOpenRequest(hConnect, "GET", szFile, HTTP_VERSION, NULL, NULL, 0, 0);
	if (!hRequest)
	{
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);

		free(pfq);
		return FALSE;
	}

	char szType[50] = { 0 };
	char szTody[60] = { 0 };
	char szDate[60] = { 0 };
	char szToken[1024] = { 0 };

	unsigned char szSignature[80] = { 0 };
	int len = 0;

	SYSTEMTIME st = { 0 };

	GetSystemTime(&st);
	format_gmttime(&st, szTody);

	WIN32_FILE_ATTRIBUTE_DATA ad = { 0 };

	if (stricmp(pfq->dest_path, ".") == 0)
		sprintf(szFile, "%s\\%s", szRunPath, pfq->file_name);
	else
		sprintf(szFile, "%s\\%s\\%s", szRunPath, pfq->dest_path, pfq->file_name);

	if (GetFileAttributesEx(szFile, GetFileExInfoStandard, &ad))
	{
		FileTimeToSystemTime(&ad.ftLastWriteTime, &st);
		format_gmttime(&st, szDate);
	}

	unsigned char dig[20] = { 0 };

	strcat(szToken, "GET");
	strcat(szToken, "\n");
	strcat(szToken, szType);
	strcat(szToken, "\n");
	strcat(szToken, szTody);
	strcat(szToken, "\n");
	strcat(szToken, szPath);
	if (strlen(pfq->path_name))
	{
		strcat(szToken, "/");
		strcat(szToken, pfq->path_name);

		strcat(szToken, "/");
		strcat(szToken, pfq->file_name);
	}
	else
	{
		strcat(szToken, "/");
		strcat(szToken, pfq->file_name);
	}
	
	xsha1_hash_bytes((unsigned char*)szPrivate, strlen(szPrivate), (unsigned char*)szToken, strlen(szToken), dig);
	len = 64;
	base64_encode((unsigned char*)szSignature, &len, dig, 20);

	sprintf(szToken, "Authorization: XDS %s:%s\r\n", szPublic, szSignature);
	HttpAddRequestHeaders(hRequest, szToken, -1, HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);

	sprintf(szToken, "Accept-Encoding: deflate\r\n");
	HttpAddRequestHeaders(hRequest, szToken, -1, HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);

	sprintf(szToken, "Date: %s\r\n", szTody);
	HttpAddRequestHeaders(hRequest, szToken, -1, HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);

	if (strlen(szDate) > 0)
	{
		sprintf(szToken, "If-Modified-Since: %s\r\n", szDate);
		HttpAddRequestHeaders(hRequest, szToken, -1, HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);
	}

	if (!HttpSendRequest(hRequest, NULL, 0, NULL, 0))
	{
		DWORD dw = GetLastError();

		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);

		free(pfq);
		return FALSE;
	}

	char szSize[24] = { 0 };
	char szCode[10] = { 0 };
	char szTime[60] = { 0 };
	char szEncode[20] = { 0 };
	char szRange[60] = { 0 };
	DWORD dwSize;

	dwSize = 10;
	HttpQueryInfo(hRequest, HTTP_QUERY_STATUS_CODE, (LPVOID)szCode, &dwSize, NULL);

	dwSize = 24;
	HttpQueryInfo(hRequest, HTTP_QUERY_CONTENT_LENGTH, (LPVOID)szSize, &dwSize, NULL);

	dwSize = 20;
	HttpQueryInfo(hRequest, HTTP_QUERY_CONTENT_ENCODING, (LPVOID)szEncode, &dwSize, NULL);

	dwSize = 60;
	HttpQueryInfo(hRequest, HTTP_QUERY_LAST_MODIFIED, (LPVOID)szTime, &dwSize, NULL);

	dwSize = 60;
	HttpQueryInfo(hRequest, HTTP_QUERY_CONTENT_RANGE, (LPVOID)szRange, &dwSize, NULL);

	dwSize = atoi(szSize);
	unsigned char* szBuf = (unsigned char*)calloc(dwSize, sizeof(unsigned char));

	DWORD dwPos = 0;
	DWORD dwLen = 0;
	while (InternetReadFile(hRequest, szBuf + dwPos, dwSize - dwPos, &dwLen))
	{
		if (!dwLen)
			break;

		dwPos += dwLen;
	}

	InternetCloseHandle(hRequest);
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hSession);

	if (szCode[0] != '2')
	{
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);

		free(pfq);

		return FALSE;
	}

	if (stricmp(szEncode, "deflate") == 0)
	{
		DWORD dwFrom,dwTo,dwTotal = 0;

		ParseRange(szRange, &dwFrom, &dwTo, &dwTotal);

		unsigned char* szTmp = (unsigned char*)calloc(dwTotal, sizeof(unsigned char));

		dwSize = xzlib_uncompress_bytes((unsigned char*)szBuf, dwSize, szTmp, dwTotal);
		free(szBuf);
		szBuf = szTmp;
	}

	TCHAR* tk;
	int tklen;

	if (stricmp(pfq->dest_path, ".") == 0)
	{
		sprintf(szFile, "%s\\%s", szRunPath, pfq->file_name);
	}
	else
	{
		strcpy(szFile, szRunPath);

		tk = pfq->dest_path;
		while (*tk != '\0')
		{
			tklen = 0;
			while (*tk != '\\' && *tk != '/' && *tk != '\0')
			{
				tk++;
				tklen++;
			}
			
			if (tklen)
			{
				strcat(szFile, "\\");
				strncat(szFile, tk - tklen, tklen);
				CreateDirectory(szFile, NULL);
			}

			if (*tk == '\\' || *tk == '/')
				tk++;
		}

		sprintf(szFile, "%s\\%s\\%s", szRunPath, pfq->dest_path, pfq->file_name);
	}

	HANDLE hFile = CreateFile(szFile, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		free(szBuf);

		free(pfq);
		return FALSE;
	}

	dwPos = 0;
	WriteFile(hFile, (void*)szBuf, dwSize, &dwPos, NULL);
	free(szBuf);

	if (strlen(szTime) > 0)
	{
		FILETIME ft = { 0 };
		parse_gmttime(&st, szTime);
		SystemTimeToFileTime(&st, &ft);

		SetFileTime(hFile, NULL, NULL, &ft);
	}

	CloseHandle(hFile);

	free(pfq);

	return TRUE;
}

BOOL UpdateList(const char* szDestDir, const char* szDestFile)
{
	char szAddr[20] = { 0 };
	char szPort[10] = { 0 };
	char szPath[MAX_PATH] = { 0 };
	char szPublic[50] = { 0 };
	char szPrivate[50] = { 0 };

	char szFile[MAX_PATH] = { 0 };

	if (stricmp(szDestDir, ".") == 0)
		printf("List Files: %s--------", szDestFile);
	else
		printf("List Files: %s/%s--------", szDestDir, szDestFile);


	sprintf(szFile, "%s\\xUpdate.ini", szRunPath);

	GetPrivateProfileString("server", "addr", "", szAddr, 20, szFile);
	GetPrivateProfileString("server", "port", "", szPort, 10, szFile);
	GetPrivateProfileString("server", "path", "", szPath, MAX_PATH, szFile);
	GetPrivateProfileString("secret", "public", "", szPublic, 50, szFile);
	GetPrivateProfileString("secret", "private", "", szPrivate, 50, szFile);

	HINTERNET hSession = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (!hSession)
	{
		printf("[failed]\n");

		return FALSE;
	}

	HINTERNET hConnect = InternetConnect(hSession, szAddr, atoi(szPort), NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	if (!hConnect)
	{
		InternetCloseHandle(hSession);

		printf("[failed]\n");
		return FALSE;
	}

	sprintf(szFile, "%s/%s", szPath, szDestFile);

	HINTERNET hRequest = HttpOpenRequest(hConnect, "LIST", szFile, HTTP_VERSION, NULL, NULL, 0, 0);
	if (!hRequest)
	{
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);

		printf("[failed]\n");
		return FALSE;
	}

	char szType[50] = { 0 };
	char szTody[60] = { 0 };
	char szToken[1024] = { 0 };

	unsigned char szSignature[80] = { 0 };
	int len;

	SYSTEMTIME st = { 0 };

	GetSystemTime(&st);
	format_gmttime(&st, szTody);

	unsigned char dig[20] = { 0 };

	strcat(szToken, "LIST");
	strcat(szToken, "\n");
	strcat(szToken, szType);
	strcat(szToken, "\n");
	strcat(szToken, szTody);
	strcat(szToken, "\n");
	strcat(szToken, szPath);
	strcat(szToken, "/");
	strcat(szToken, szDestFile);

	xsha1_hash_bytes((unsigned char*)szPrivate, strlen(szPrivate), (unsigned char*)szToken, strlen(szToken), dig);

	len = 64;
	base64_encode((unsigned char*)szSignature, &len, dig, 20);

	sprintf(szToken, "Authorization: XDS %s:%s\r\n", szPublic, szSignature);
	HttpAddRequestHeaders(hRequest, szToken, -1, HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);

	sprintf(szToken, "Accept-Encoding: deflate\r\n");
	HttpAddRequestHeaders(hRequest, szToken, -1, HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);

	sprintf(szToken, "Date: %s\r\n", szTody);
	HttpAddRequestHeaders(hRequest, szToken, -1, HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);

	if (!HttpSendRequest(hRequest, NULL, 0, NULL, 0))
	{
		DWORD dw = GetLastError();

		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);

		printf("[failed]\n");
		return FALSE;
	}

	char szSize[24] = { 0 };
	char szCode[10] = { 0 };
	char szTime[60] = { 0 };
	char szEncode[20] = { 0 };
	char szRange[60] = { 0 };
	DWORD dwSize;

	dwSize = 10;
	HttpQueryInfo(hRequest, HTTP_QUERY_STATUS_CODE, (LPVOID)szCode, &dwSize, NULL);

	if (szCode[0] == '2' || szCode[0] == '3')
		printf("[OK]\n");
	else
		printf("[failed]\n");

	FILEINFO *pfi = NULL;
	BYTE szBuf[4096];
	DWORD dwPos, dwLen = 0;
	DWORD dwThread;
	char szPre[MAX_PATH] = { 0 };
	char* token;
	while (1)
	{
		memset((void*)szBuf, 0, 4096);

		dwPos = 0;
		while (1)
		{
			dwLen = 0;
			InternetReadFile(hRequest, szBuf + dwPos, 1, &dwLen);

			if (!dwLen || szBuf[dwPos] == '\n')
				break;

			dwPos++;
		}
		szBuf[dwPos] = '\0';

		if (!dwPos)
			break;

		pfi = (FILEINFO*)calloc(1, sizeof(FILEINFO));

		parse_fileinfo_from_line(pfi, (char*)szBuf);

		strcpy(pfi->dest_path, szDestDir);

		strcpy(szPre, szDestFile);
		token = szPre + strlen(szPre);
		while (*token != '\\' && *token != '/' && token != szPre)
			token--;

		*(token) = '\0';
		strcpy(pfi->path_name, szPre);

		g_thr[g_count++] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)UpdateFile, (void*)pfi, 0, &dwThread);
		if (g_count == MAXIMUM_WAIT_OBJECTS)
		{
			WaitForMultipleObjects(g_count, g_thr, 1, INFINITE);
			g_count = 0;
			memset((void*)g_thr, 0, MAXIMUM_WAIT_OBJECTS * sizeof(HANDLE));
		}
	}

	InternetCloseHandle(hRequest);
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hSession);

	return TRUE;
}

VOID UpdateBatch()
{
	char szIni[MAX_PATH] = { 0 };
	char szExt[4096] = { 0 };
	char szDir[MAX_PATH] = { 0 };
	char szList[4096] = { 0 };
	char szFile[MAX_PATH] = { 0 };

	sprintf(szIni, "%s\\xUpdate.ini", szRunPath);

	GetPrivateProfileString("update", "dir", "", szExt, 4096, szIni);

	memset((void*)g_thr, 0, MAXIMUM_WAIT_OBJECTS * sizeof(HANDLE));
	g_count = 0;

	int len;
	const char* token = szExt;
	while (*token != '\0')
	{
		len = 0;
		while (*token != ',' && *token != ' ' && *token != '\0')
		{
			token++;
			len++;
		}
		strncpy(szDir, token - len, len);
		szDir[len] = '\0';

		GetPrivateProfileString("update", szDir, "", szList, 4096, szIni);

		const char* list = szList;
		while (*list != '\0')
		{
			len = 0;
			while (*list != ',' && *list != ' ' && *list != '\0')
			{
				list++;
				len++;
			}

			strncpy(szFile, list - len, len);
			szFile[len] = '\0';

			if (strlen(szFile) > 0)
			{
				UpdateList(szDir, szFile);
			}

			if (*list == ',' || *list != ' ')
				list++;
		}

		if (*token == ',' || *token != ' ')
			token++;

		memset((void*)szList, 0, 4096 * sizeof(char));
	}

	WaitForMultipleObjects(g_count, g_thr, 1, INFINITE);
}
