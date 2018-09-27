#include "threadcom.h"


ThreadCom *g_threadCom = NULL;


int ConvertX(const unsigned char *src, int iLen, unsigned char *dest)
{
	int iLast = 0;
	int iNow = 0;
	int i = 0;
	for (; i < iLen; ++i)
	{
		char ch = toupper(src[i]);
		if (ch > '9')
		{
			iNow = ch-'A'+10;
		}
		else
		{
			iNow = ch - '0';
		}
		if (i%2==1/*&&i/2>0*/)
		{
			sprintf((char*)(dest+i/2), "%c", iLast*16+iNow);
		}
		iLast = iNow;
	}
	return i/2;
}

int ConvertMem(const unsigned char *src, int isrclen, unsigned char *dest)
{
	int i = 0;
	for (; i < isrclen; ++i)
	{
		unsigned int ichlow = src[i]&0x0f;
		unsigned int ichhigh = (src[i]&0xf0)>>4;
		sprintf((char*)(dest+i*2), "%x", ichhigh);
		sprintf((char*)(dest+i*2+1), "%x", ichlow);
	}
	return i;
}
