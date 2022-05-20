#pragma once
#include <cstdio>
#include <windows.h>
#include <Psapi.h>
#pragma comment(linker, "/defaultlib:Psapi.lib")


ULONG64 GetNTBase();									// 获取ntos基址
ULONG64 GetSystemProcess();								// 获取System进程EPROCESS
BOOL CreateClipboard(DWORD dwSize);						// 通过剪切板实现垫片操作,dwSize + 0x14 + 0x10的内存块
VOID ShowError(char *str, DWORD dwErrorCode);