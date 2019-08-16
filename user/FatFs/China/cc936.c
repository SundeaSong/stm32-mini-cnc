/*------------------------------------------------------------------------*/
/* Unicode - OEM code bidirectional converter  (C)ChaN, 2009              */
/*                                                                        */
/* CP936 (Simplified Chinese GBK)                                         */
/*------------------------------------------------------------------------*/
//Sharpufo(HuangRongBin)做了部分更改以适用于OOPY MUSIC PLAYER。
#include "ff.h"
extern FIL f_u2gb;
extern unsigned char u2gb_on;

WCHAR ff_convert (	/* Converted code, 0 means conversion error */
	WCHAR	src	/* Character code to be converted */
)
{
	WCHAR t[2];
	WCHAR c;
	unsigned long i, li, hi;
	int n;
	unsigned int cout;

if( ! u2gb_on )
{
	if(f_open (&f_u2gb,"SYSTEM/U2GB.TBL", FA_READ)!=FR_OK)
			u2gb_on=0;
	else 
			u2gb_on=1;
}


	if (src < 0x80){/* ASCII */c = src;}
	else 
		if(u2gb_on==0)c='?';
		else
		{
			/* Unicode to OEMCP */
			hi=f_u2gb.fsize;
			hi =hi / 4 - 1;
			li = 0;
			for (n = 16; n; n--)
				{
				i = li + (hi - li) / 2;
				f_lseek(&f_u2gb,i*4);
				f_read(&f_u2gb,&t,4,&cout);
				if (src == t[0]) break;
				if (src > t[0])
					li = i;
				else
					hi = i;
				}
			if(n!=0)
				c=t[1];
			else c='?';
 			}
	return c;
}
