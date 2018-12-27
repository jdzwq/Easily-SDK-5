// xLoad.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <conio.h>

#include "xUpdate.h"

char szRunPath[MAX_PATH] = { 0 };

int main(int argc, char* argv[])
{
	char* token;

	GetModuleFileName(NULL, szRunPath, MAX_PATH);

	token = szRunPath;
	while (*token != '\0')
		token++;

	while (*token != '\\' && *token != '/' && token != szRunPath)
		token--;

	if (*token != '\\' || *token != '/')
		*token = '\0';

	DWORD IDThread;

	if (argc > 1)
		token = argv[1];
	else
		token = NULL;

	printf("Begin Update................................[OK]\n");

	UpdateBatch();

	printf("End Update.................................[OK]\n");

	if (argc > 1)
	{
		char szFile[100] = { 0 };

		strcpy(szFile, argv[1]);

		if ((DWORD)ShellExecute(NULL, "open", szFile, NULL, szRunPath, SW_SHOW) > 32)
		{
			printf("Run %s.................................[OK]\n", szFile);
#ifdef _DEBUG
			getch();
#else
			Sleep(1000);
#endif
		}
		else
		{
			printf("Run %s.................................[failed]\n", szFile);
			getch();
		}
	}
	else
	{
#ifdef _DEBUG
		getch();
#else
		Sleep(1000);
#endif
	}

	return 0;
}

