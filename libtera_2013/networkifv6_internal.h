/******* BEGIN LICENCE BLOCK *******

    libtera 
    Copyright (C) 2012 Yomei

    This program is free software; you can redistribute it and/or modify it under the terms of the BSD License.

******* END LICENCE BLOCK *******/

#ifndef df_NETWORKIF_INT_H
#define df_NETWORKIF_INT_H

#include "networkif.h"

#ifdef __cplusplus
extern "C"
{
#endif


//typedef struct st_myipaddress_info MYIPADDRESSINFO;
//typedef struct st_mynetworkif MYNETWORKIF;

int networkifv6_internal_enum(MYNETWORKIF* ret,int(*f)(MYNETWORKIF*,MYIPADDRESSINFO*));

int networkifv6_internal_enum_windows(MYNETWORKIF* ret,int(*f)(MYNETWORKIF*,MYIPADDRESSINFO*));
int networkifv6_internal_enum_linux(MYNETWORKIF* ret,int(*f)(MYNETWORKIF*,MYIPADDRESSINFO*));
int networkifv6_internal_enum_default(MYNETWORKIF* ret,int(*f)(MYNETWORKIF*,MYIPADDRESSINFO*));

int networkifv6_internal_print(MYNETWORKIF * p,MYIPADDRESSINFO* pipaddress);

#ifdef __cplusplus
}
#endif


#endif /* df_NETWORKIF_H */



