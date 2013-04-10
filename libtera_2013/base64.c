/******* BEGIN LICENCE BLOCK *******

    Sony Digital Photo Frame Application
    Copyright (C) 2008 Sony Corporation.

    This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License,version 2, as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program; if not, see <http://www.gnu.org/licenses>.

    Linking Sony Digital Photo Frame Application statically or dynamically with other modules is making a combined work based on Sony Digital Photo Frame Application. Thus, the terms and conditions of the GNU General Public License cover the whole combination.

    In addition, as a special exception, the copyright holders of Sony Digital Photo Frame Application give you permission to combine Sony Digital Photo Frame Application with free software programs or libraries that are released under the GNU LGPL and with code included in the standard release of Sony OS Abstraction Library (or modified versions of such code, with unchanged license). You may copy and distribute such a system following the terms of the GNU GPL for Sony Digital Photo Frame Application and the licenses of the other code concerned{, provided that you include the source code of that other code when and as the GNU GPL requires distribution of source code}.

    Note that people who make modified versions of Sony Digital Photo Frame Application are not obligated to grant this special exception for their modified versions; it is their choice whether to do so. The GNU General Public License gives permission to release a modified version without this exception; this exception also makes it possible to release a modified version which carries forward this exception.

******* END LICENCE BLOCK *******/

/* $Id: base64.c,v 1.3 2008/04/14 02:11:48 akatsuka Exp $ */



#ifdef _MSC_VER
#if _MSC_VER >= 1400
#pragma warning( disable : 4996 )
#pragma warning( disable : 4819 )
#endif
#endif

#include <stdlib.h>


static char *b64chars = 
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
//"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


/*
** encrypt by base64
** sizeof(dest) must be 4/3 times larger than len
**
** src  (i) target string
** len  (i) length of src
** dest (o) encrypted string
**
** return strlen(dest)
*/
int b64enc(const unsigned char *src, int len, char *dest)
{
	char *buf = dest;

	if(src==NULL || dest==NULL) return 0;

	/* Encode 3 bytes at a time. */
	while (len >= 3) {
		buf[0] = b64chars[ (src[0] >> 2) & 0x3f ];
		buf[1] = b64chars[((src[0] << 4) & 0x30) | ((src[1] >> 4) & 0xf)];
		buf[2] = b64chars[((src[1] << 2) & 0x3c) | ((src[2] >> 6) & 0x3)];
		buf[3] = b64chars[  src[2]       & 0x3f ];
		src += 3;
		buf += 4;
		len -= 3;
	}

	/* last 2 or 1 byte */
	switch(len) {
	case 2:
		buf[0] = b64chars[ (src[0] >> 2) & 0x3f ];
		buf[1] = b64chars[((src[0] << 4) & 0x30) | ((src[1] >> 4) & 0xf)];
		buf[2] = b64chars[ (src[1] << 2) & 0x3c ];
		buf[3] = '=';
		buf += 4;
		break;
	case 1:
		buf[0] = b64chars[ (src[0] >> 2) & 0x3f ];
		buf[1] = b64chars[ (src[0] << 4) & 0x30 ];
		buf[2] = '=';
		buf[3] = '=';
		buf += 4;
		break;
	}

	*buf = '\0';
	return (buf - dest);
}


/*
** decrypt by base64
** sizeof(dest) must be 3/4 times larger than len
**
** src  (i) target string
** len  (i) length of src
** dest (o) decrypted string
**
** return strlen(dest)
*/
int b64dec(const unsigned char *src, int len, char *dest)
{
	unsigned char c;
	int cur = 0, val = 0, i = 0;

	if(src==NULL || dest==NULL) return 0;

	while (len) {
		c = *src;
		len--;
		src++;

		if (c >= 'A' && c <= 'Z') val = c - 'A';
		else if (c >= 'a' && c <= 'z') val = c - 'a' + 26;
		else if (c >= '0' && c <= '9') val = c - '0' + 52;
		else if (c == '-') val = 62;
		else if (c == '_') val = 63;
		else if (c == '=') break;
		else continue;

		switch (cur++) {
			case 0:
				dest[i]  = (val << 2) & 0xfc;
				break;
			case 1:
				dest[i] |= (val >> 4) & 0x03;
				i++;
				dest[i]  = (val << 4) & 0xf0;
				break;
			case 2:
				dest[i] |= (val >> 2) & 0x0f;
				i++;
				dest[i]  = (val << 6) & 0xc0;
				break;
			case 3:
				dest[i] |= val & 0x3f;
				i++;
				cur = 0;
				break;
		}
	}
	dest[i] = 0;
	return (i);
}



#if 0

int main()
{
	int ret=0;
	char aaa[64],bbb[64];

	strcpy(aaa,"user01:password");

	ret=b64enc(aaa,strlen(aaa),bbb);
	printf("%d >%s<\n",ret,bbb);

	ret=b64dec(bbb,strlen(bbb),aaa);
	printf("%d >%s<\n",ret,aaa);
	return 0;
}

#endif

