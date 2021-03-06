/******* BEGIN LICENCE BLOCK *******

    libtera 
    Copyright (C) 2012 Yomei

    This program is free software; you can redistribute it and/or modify it under the terms of the BSD License.

******* END LICENCE BLOCK *******/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _WIN32_WINNT 0x0501
#define USE_SYS_TYPES_FD_SET

/* Win32 specified header */
#if defined(_WIN32) || defined(__CYGWIN__) 
#if (defined(_MSC_VER) && (_MSC_VER<1400)) || defined(__CYGWIN__)
#include <windows.h>
#include <winsock2.h>
#else
#include <winsock2.h>
#include <windows.h>
#endif
#endif

#if defined(_WIN32) || defined(__CYGWIN__) 
#include <Ws2tcpip.h>
#endif

#ifndef _WINSOCK2_H
#define _WINSOCK2_H
#endif

#ifndef _ANONYMOUS_UNION
#define _ANONYMOUS_UNION
#endif

#ifndef _ANONYMOUS_STRUCT
#define _ANONYMOUS_STRUCT
#endif

#ifndef DUMMYSTRUCTNAME
#define DUMMYSTRUCTNAME
#endif

#ifndef DUMMYUNIONNAME
#define DUMMYUNIONNAME
#endif

#ifndef IF_TYPE_ETHERNET_CSMACD
#define IF_TYPE_ETHERNET_CSMACD 6
#endif

#ifndef IF_TYPE_SOFTWARE_LOOPBACK
#define IF_TYPE_SOFTWARE_LOOPBACK 24
#endif

#ifndef IF_TYPE_IEEE80211
#define IF_TYPE_IEEE80211 71
#endif

#if defined(_WIN32) || defined(__CYGWIN__) 
#include <Iphlpapi.h>
#endif


#if defined(_WIN32) && !defined(__GNUC__)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#if defined(_WIN32) && !defined(__GNUC__)
#pragma comment( lib, "iphlpapi.lib" )
#pragma comment( lib, "wsock32.lib" )
#endif

#include "networkif.h"
#include "networkifv6_internal.h"

#define IPV4_DEFAULT_PREFIX 8
#define IPV6_DEFAULT_PREFIX 48


static unsigned char masklist[][4]={
	{0x0 },//0
	{0x80},//1
	{0xc0},//2
	{0xe0},//3
	{0xf0},//4
	{0xf8},//5
	{0xfc},//6
	{0xfe},//7
	{0xff},//8
	{0xff,0x80},//9
	{0xff,0xc0},//10
	{0xff,0xe0},//11
	{0xff,0xf0},//12
	{0xff,0xf8},//13
	{0xff,0xfc},//14
	{0xff,0xfe},//15
	{0xff,0xff},//16
	{0xff,0xff,0x80},//17
	{0xff,0xff,0xc0},//18
	{0xff,0xff,0xe0},//19
	{0xff,0xff,0xf0},//20
	{0xff,0xff,0xf8},//21
	{0xff,0xff,0xfc},//22
	{0xff,0xff,0xfe},//23
	{0xff,0xff,0xff},//24
	{0xff,0xff,0xff,0x80},//25
	{0xff,0xff,0xff,0xc0},//26
	{0xff,0xff,0xff,0xe0},//27
	{0xff,0xff,0xff,0xf0},//28
	{0xff,0xff,0xff,0xf8},//29
	{0xff,0xff,0xff,0xfc},//30
	{0xff,0xff,0xff,0xfe},//31
	{0xff,0xff,0xff,0xff},//32
};


static unsigned char masklist6[][16]={
	{0x0 },//0
	{0x80},//1
	{0xc0},//2
	{0xe0},//3
	{0xf0},//4
	{0xf8},//5
	{0xfc},//6
	{0xfe},//7
	{0xff},//8
	{0xff,0x80},//9
	{0xff,0xc0},//10
	{0xff,0xe0},//11
	{0xff,0xf0},//12
	{0xff,0xf8},//13
	{0xff,0xfc},//14
	{0xff,0xfe},//15
	{0xff,0xff},//16
	{0xff,0xff,0x80},//17
	{0xff,0xff,0xc0},//18
	{0xff,0xff,0xe0},//19
	{0xff,0xff,0xf0},//20
	{0xff,0xff,0xf8},//21
	{0xff,0xff,0xfc},//22
	{0xff,0xff,0xfe},//23
	{0xff,0xff,0xff},//24
	{0xff,0xff,0xff,0x80},//25
	{0xff,0xff,0xff,0xc0},//26
	{0xff,0xff,0xff,0xe0},//27
	{0xff,0xff,0xff,0xf0},//28
	{0xff,0xff,0xff,0xf8},//29
	{0xff,0xff,0xff,0xfc},//30
	{0xff,0xff,0xff,0xfe},//31
	{0xff,0xff,0xff,0xff},//32
	{0xff,0xff,0xff,0xff,0x80},//33
	{0xff,0xff,0xff,0xff,0xc0},//34
	{0xff,0xff,0xff,0xff,0xe0},//35
	{0xff,0xff,0xff,0xff,0xf0},//36
	{0xff,0xff,0xff,0xff,0xf8},//37
	{0xff,0xff,0xff,0xff,0xfc},//38
	{0xff,0xff,0xff,0xff,0xfe},//39
	{0xff,0xff,0xff,0xff,0xff},//40
	{0xff,0xff,0xff,0xff,0xff,0x80},//41
	{0xff,0xff,0xff,0xff,0xff,0xc0},//43
	{0xff,0xff,0xff,0xff,0xff,0xe0},//43
	{0xff,0xff,0xff,0xff,0xff,0xf0},//44
	{0xff,0xff,0xff,0xff,0xff,0xf8},//45
	{0xff,0xff,0xff,0xff,0xff,0xfc},//46
	{0xff,0xff,0xff,0xff,0xff,0xfe},//47
	{0xff,0xff,0xff,0xff,0xff,0xff},//48
	{0xff,0xff,0xff,0xff,0xff,0xff,0x80},//49
	{0xff,0xff,0xff,0xff,0xff,0xff,0xc0},//50
	{0xff,0xff,0xff,0xff,0xff,0xff,0xe0},//51
	{0xff,0xff,0xff,0xff,0xff,0xff,0xf0},//52
	{0xff,0xff,0xff,0xff,0xff,0xff,0xf8},//53
	{0xff,0xff,0xff,0xff,0xff,0xff,0xfc},//54
	{0xff,0xff,0xff,0xff,0xff,0xff,0xfe},//55
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff},//56
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x80},//57
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xc0},//58
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xe0},//59
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf0},//60
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf8},//61
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfc},//62
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe},//63
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},//64
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x80},//1
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xc0},//2
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xe0},//3
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf0},//4
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf8},//5
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfc},//6
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe},//7
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},//8
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x80},//9
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xc0},//10
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xe0},//11
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf0},//12
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf8},//13
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfc},//14
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe},//15
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},//16
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x80},//17
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xc0},//18
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xe0},//19
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf0},//20
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf8},//21
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfc},//22
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe},//23
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},//24
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x80},//25
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xc0},//26
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xe0},//27
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf0},//28
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf8},//29
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfc},//30
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe},//31
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},//32
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x80},//33
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xc0},//34
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xe0},//35
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf0},//36
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf8},//37
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfc},//38
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe},//39
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},//40
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x80},//41
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xc0},//43
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xe0},//43
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf0},//44
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf8},//45
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfc},//46
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe},//47
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},//48
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x80},//49
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xc0},//50
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xe0},//51
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf0},//52
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf8},//53
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfc},//54
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe},//55
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},//56
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x80},//57
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xc0},//58
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xe0},//59
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf0},//60
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf8},//61
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfc},//62
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe},//63
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},//64
};


// for Visual Studio 6 , Visual Studio 2002 ,Visual Studio 2003
#ifndef JAVA
#if defined(_MSC_VER) && (_MSC_VER<1700)
static int snprintf(char * buf,size_t n,const char *format,...)
{
	int ret;
	va_list ap;
    va_start(ap, format);
    ret = vsprintf(buf,format, ap);
    va_end(ap);
	return ret;
}
#endif
#endif



#ifdef _WIN32
static int get_prefix(PIP_ADAPTER_UNICAST_ADDRESS pUU,PIP_ADAPTER_PREFIX ppre)
{
	int i;
	int family,flag;	
	PIP_ADAPTER_PREFIX pPP;
	char tmp[16]; 
	char *mask,*ip;
	
	if(pUU==NULL)return 0;

	family=pUU->Address.lpSockaddr->sa_family;
	if(family!=AF_INET && family!=AF_INET6)return 0;

	if(family==AF_INET6)goto inet6;

	{
		LPSOCKADDR psa=NULL;
		struct sockaddr_in* p;

		if(ppre==NULL)return IPV4_DEFAULT_PREFIX;

		psa=pUU->Address.lpSockaddr;
		p=(struct sockaddr_in*)psa;
		ip=(char*)(&(p->sin_addr.S_un.S_addr));


		for(pPP=ppre;pPP;pPP=pPP->Next)
		if(pPP->Address.lpSockaddr->sa_family==AF_INET){
			struct sockaddr_in* p4;
			char* pp;
			p4=(struct sockaddr_in*)(pPP->Address.lpSockaddr);
			pp=(char*)(&(p4->sin_addr.S_un.S_addr));
		
			//printf("ipv4 prefix\n");
			//for(i=0;i<4;i++){
			//	printf("%d",pp[i]&255);
			//	if(i!=3)printf(".");
			//}
			//printf("\n");

			flag=1;
			mask=(char*)(masklist[pPP->PrefixLength]);
			for(i=0;i<4;i++)tmp[i]=ip[i]&mask[i];
			for(i=0;i<4;i++){
				//printf("tmp[%d]=%d  pp[%d]=%d\n",i,tmp[i]&255,i,pp[i]&255);
				if(tmp[i]!=pp[i])flag=0;
			}
			if(flag==1){
				return pPP->PrefixLength;
			}
		}
		//default prefix ipv4
		return IPV4_DEFAULT_PREFIX;
	}
inet6:
	{
		LPSOCKADDR psa=NULL;
		struct sockaddr_in6* p;

		if(ppre==NULL)return IPV6_DEFAULT_PREFIX;

		psa=pUU->Address.lpSockaddr;
		p=(struct sockaddr_in6*)psa;
		ip=(char*)(p->sin6_addr.s6_addr);
	
		for(pPP=ppre;pPP;pPP=pPP->Next)
		if(pPP->Address.lpSockaddr->sa_family==AF_INET6){
			struct sockaddr_in6* p6;
			char* pp;
			p6=(struct sockaddr_in6*)(pPP->Address.lpSockaddr);
			pp=(char*)&(p6->sin6_addr.s6_addr[0]);
		
			//printf("ipv6 prefix\n");
			//for(i=0;i<16;i++){
			//	printf("%02x",pp[i]&255);
			//	if(i!=15)printf(":");
			//}
			//printf("\n");

			flag=1;
			mask=(char*)(masklist6[pPP->PrefixLength]);
			for(i=0;i<16;i++)tmp[i]=ip[i]&mask[i];
			for(i=0;i<16;i++){
				//printf("tmp[%d]=%02x  pp[%d]=%02x\n",i,tmp[i]&255,i,pp[i]&255);
				if(tmp[i]!=pp[i])flag=0;
			}
			if(flag==1){
				return pPP->PrefixLength;
			}
		}
		//default prefix ipv6
		return IPV6_DEFAULT_PREFIX;
	}

	return 0;
}
#endif

int networkifv6_internal_enum_windows(MYNETWORKIF* pmif,int(*f)(MYNETWORKIF*,MYIPADDRESSINFO*))
{

#if 1
#ifdef _WIN32
	ULONG len = 0;
	DWORD ret = 0;
	PIP_ADAPTER_ADDRESSES adpts=NULL,pAA=NULL;
	PIP_ADAPTER_UNICAST_ADDRESS paua=NULL,pUU=NULL;
	char paddr_type=0;
	int prefix;

	ret=GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, 0, 0, &len);
	if(ret != ERROR_BUFFER_OVERFLOW)return 0;
	
	adpts = (PIP_ADAPTER_ADDRESSES)malloc(len);
	if(adpts == NULL) return 0;

	ret = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, 0, adpts, &len);
	if(ret != ERROR_SUCCESS){
		free(adpts);
		return 0;
	}

    for (pAA = adpts; pAA; pAA = pAA->Next) {
		paua=pAA->FirstUnicastAddress;
		if(pAA->IfType!=IF_TYPE_ETHERNET_CSMACD &&
		pAA->IfType!=IF_TYPE_SOFTWARE_LOOPBACK &&
		pAA->IfType!=IF_TYPE_IEEE80211)continue;

		paddr_type=0;
		if(pAA->IfType==IF_TYPE_ETHERNET_CSMACD)paddr_type=1;
		if(pAA->IfType==IF_TYPE_IEEE80211)paddr_type=1;


		for(pUU=paua;pUU;pUU=pUU->Next){
			LPSOCKADDR psa=NULL;
			psa=pUU->Address.lpSockaddr;

			if(psa->sa_family==AF_INET)
			{
				struct sockaddr_in* p;
				char *pp;
				MYIPADDRESSINFO ip;

				p=(struct sockaddr_in*)psa;
				pp=(char*)(&(p->sin_addr.S_un.S_addr));

				prefix=get_prefix(pUU,pAA->FirstPrefix);

				memset(&ip,0,sizeof(MYIPADDRESSINFO));
				ip.type=MYIPADDRESS_TYPE_IPV4;

				memcpy(ip.ipAddress.address,pp,4);
				ip.ipAddress.type=MYIPADDRESS_TYPE_IPV4;
				ip.ipAddress.len=4;
				ip.netMask.type=MYIPADDRESS_TYPE_IPV4;
				ip.netMask.len=4;
				memcpy(ip.netMask.address,masklist[prefix],4);

				if(paddr_type==1){
					ip.phyAddress.type=MYIPADDRESS_TYPE_ETHERNET;
					ip.phyAddress.len=6;
					memcpy(ip.phyAddress.address,pAA->PhysicalAddress,6);
				}
				(*f)(pmif,&ip);

			}
			if(psa->sa_family==AF_INET6)
			{
				struct sockaddr_in6* p;
				char *pp;
				MYIPADDRESSINFO ip;

				p=(struct sockaddr_in6*)psa;
				p->sin6_addr;
				pp=(char*)&(p->sin6_addr.s6_addr[0]);

				prefix=get_prefix(pUU,pAA->FirstPrefix);

				memset(&ip,0,sizeof(MYIPADDRESSINFO));
				ip.type=MYIPADDRESS_TYPE_IPV6;

				memcpy(ip.ipAddress.address,pp,16);
				ip.ipAddress.type=MYIPADDRESS_TYPE_IPV6;
				ip.ipAddress.len=16;
				ip.netMask.type=MYIPADDRESS_TYPE_IPV6;
				ip.netMask.len=16;
				memcpy(ip.netMask.address,masklist6[prefix],16);

				if(paddr_type==1){
					ip.phyAddress.type=MYIPADDRESS_TYPE_ETHERNET;
					ip.phyAddress.len=6;
					memcpy(ip.phyAddress.address,pAA->PhysicalAddress,6);
				}
				//invoke
				(*f)(pmif,&ip);

			}
		}
	}
	free(adpts);
	return 1;
#endif /* _WIN32 */
#endif
	return 0;
}





