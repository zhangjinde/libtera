/******* BEGIN LICENCE BLOCK *******

    libtera 
    Copyright (C) 2012 Yomei

    This program is free software; you can redistribute it and/or modify it under the terms of the BSD License.

******* END LICENCE BLOCK *******/

#ifndef df_MYSIGNAL_H_
#define df_MYSIGNAL_H_


#ifdef __cplusplus
extern "C"
{
#endif


typedef void (*mysighandler_t)(int);


void mysignal_init(void);
void mysignal_fini(void);

mysighandler_t mysignal(int signum, mysighandler_t sighandler);  
int myraise(int sig);

int mysignal_is_abort(void);


#ifdef __cplusplus
}
#endif


#endif /* _FDUP_H_ */


