#pragma once
#include <cstdio>
#include <windows.h>
#include <Psapi.h>
#pragma comment(linker, "/defaultlib:Psapi.lib")


ULONG64 GetNTBase();									// ��ȡntos��ַ
ULONG64 GetSystemProcess();								// ��ȡSystem����EPROCESS
BOOL CreateClipboard(DWORD dwSize);						// ͨ�����а�ʵ�ֵ�Ƭ����,dwSize + 0x14 + 0x10���ڴ��
VOID ShowError(char *str, DWORD dwErrorCode);