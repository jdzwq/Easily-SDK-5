#ifdef _UNICODE
#define _tstrcmp	wcscmp
#define _tstrncmp	wcsncmp
#define _tsprintf	_swprintf
#define _tstrcat	wcscat
#define _tstrncat	wcsncat
#define _tstrcpy	wcscpy
#define _tstrstr	wcsstr
#define _tstrncpy	wcsncpy
#define _tstrlen	wcslen
#define _tstrtol	_wtol
#define _tstrtof	_wtof
#define _tstrnull(s)		(s == NULL || s[0] == L'\0')
#define _tprintf    wprintf
#define _tsscanf	swscanf
#else
#define _tstrcmp	strcmp
#define _tstrncmp	strncmp
#define _tstricmp	strcmp
#define _tstrnicmp	strncmp
#define _tsprintf	sprintf
#define _tstrcat	strcat
#define _tstrncat	strncat
#define _tstrcpy	strcpy
#define _tstrstr	strstr
#define _tstrncpy	strncpy
#define _tstrlen	strlen
#define _tstrtol	atol
#define _tstrtof	atof
#define _tstrnull(s)		(s == NULL || s[0] == '\0')
#define _tprintf    printf
#define _tsscanf	sscanf
#endif