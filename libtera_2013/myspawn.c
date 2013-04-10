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

#ifdef _MSC_VER
#if _MSC_VER >= 1400
#pragma warning( disable : 4996 )
#pragma warning( disable : 4819 )
#endif
#endif
/* $Id: spawn.c,v 1.18 2008/12/15 10:13:28 tooyama Exp $ */

/*
 * Copyright (C) 2006 Sony Corporation.
 * All rights reserved.
 */




#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "myspawn.h"

#ifdef _WIN32
#include <windows.h>
#include <process.h>
#endif

#ifdef unix
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

#define VA_SIZE 16





#ifdef _WIN32
#define PATH_KUGIRI ';'
#else
#define PATH_KUGIRI ':'
#endif

#ifdef _WIN32
#define FILE_KUGIRI '\\'
#else
#define FILE_KUGIRI '/'
#endif




static char *get_path(void)
{
	char *p;
	char *ret;
	int len;
	int sz;
	
	p=getenv("PATH");
	if(p==NULL)return NULL;
	len=(int)strlen(p);
	sz=len+1;
	ret=malloc(sz);
	if(ret==NULL)return NULL;
	strncpy(ret,p,sz);
	return ret;
}

static char *next_path(char* p)
{

	if(p==NULL)return NULL;
	while(*p!=0 && *p!=PATH_KUGIRI)p++;
	if(*p==0)return NULL;
	p++;
	if(*p==0)return NULL;
	return p;
}

static void release_path(char *p)
{
	if(p==NULL)return;
	free(p);
}

static char* get_top_path(char* p,char* name)
{
	int plen,nlen;
	char *src;
	char *ret;
	int sz;

	if(p==NULL || name==NULL)return NULL;
	if(*p==0)return NULL;
	nlen=(int)strlen(name);

	src=p;
	plen=0;
	while(*src!=0 && *src!=PATH_KUGIRI)
	{
		src++;
		plen++;
	}
	//printf("plen=%d\n",plen);

	sz=plen+nlen+4;
	ret=malloc(sz);
	if(ret==NULL)return NULL;

	memcpy(ret,p,plen);
	ret[plen]=FILE_KUGIRI;
	strncpy(ret+plen+1,name,sz-1-plen);
	return ret;
}


static void release_top_path(char* p)
{
	if(p)free(p);
}






intptr_t myspawnl( int mode, const char *cmdname, const char *arg0, ...)
{
	int i;
	const char* argv[VA_SIZE];
	void *vp;
	va_list marker;

	argv[0]=arg0;

	if(arg0)
	{
		va_start( marker, arg0 );
		for(i=0;i<VA_SIZE-2;i++)
		{
			vp = (void*)va_arg( marker, void*);
			argv[1+i]=vp;
			if(vp==NULL)break;
		}
		argv[1+i]=NULL;
	}
	return myspawnv(mode,cmdname,argv);
}

intptr_t myspawnv( int mode, const char *cmdname, const char *const *argv )
{
#ifdef _WIN32
	return _spawnv(mode,cmdname,argv);
#endif
#ifdef unix	
	int pid;
	if(mode==MYP_OVERLAY)
	{
		execv(cmdname,(void*)argv);
		return -1;
	}
	else pid = vfork();

	if (pid) {
		if(mode==MYP_WAIT)
		{
			mywait(pid);
		}
//		else waitpid(0, NULL, WNOHANG);
//		fprintf( stderr, "pass spawnv\n" ) ;
		return pid;
	}
	else if (pid == 0)
	{
		execv(cmdname,(void*)argv);
		_exit(1);
	}
#endif
	return -1;
}





intptr_t myspawnle( int mode, const char *cmdname, const char *arg0, ...)
{
	int i;
	const char* argv[VA_SIZE];
	const char *const *envp;
	void *vp;
	va_list marker;

	argv[0]=arg0;

	if(arg0)
	{
		va_start( marker, arg0 );
		for(i=0;i<VA_SIZE-2;i++)
		{
			vp = (void*)va_arg( marker, void*);
			argv[1+i]=vp;
			if(vp==NULL)break;
		}
		argv[1+i]=NULL;
	}
	envp = (const char *const *)va_arg( marker, void*);
	return myspawnve(mode,cmdname,argv,envp);

}

intptr_t myspawnve( int mode, const char *cmdname, const char *const *argv, const char *const *envp )
{
#ifdef _WIN32
	return _spawnve(mode,cmdname,argv,envp);
#endif
#ifdef unix
	int pid;
	if(mode==MYP_OVERLAY)
	{
		execve(cmdname,(void*)argv,(void*)envp);
		return -1;
	}
	else pid = vfork();
	if (pid) {
		if(mode==MYP_WAIT)
		{
			return mywait(pid);
		}
//		else waitpid(0, NULL, WNOHANG);
//		fprintf( stderr, "pass spawnve\n" ) ;
		return pid;
	}
	else if (pid == 0)
	{
		execve(cmdname,(void*)argv,(void*)envp);
		_exit(1);
	}
#endif
	return -1;
}








intptr_t myspawnlp( int mode, const char *cmdname, const char *arg0, ...)
{
	int i;
	const char* argv[VA_SIZE];
	void *vp;
	va_list marker;

	argv[0]=arg0;

	if(arg0)
	{
		va_start( marker, arg0 );
		for(i=0;i<VA_SIZE-2;i++)
		{
			vp = (void*)va_arg( marker, void*);
			argv[1+i]=vp;
			if(vp==NULL)break;
		}
		argv[1+i]=NULL;
	}
	return myspawnvp(mode,cmdname,argv);
}

intptr_t myspawnvp( int mode, const char *cmdname, const char *const *argv )
{
#ifdef _WIN32
	return _spawnvp(mode,cmdname,argv);
#endif


#ifdef unix
#if 1	
	int pid;
	char *p=NULL,*q=NULL;
	char* n=NULL;
	struct stat ssss;
	int ret=-1;
	p=get_path();
	if(p==NULL)return -1;

	q=p;
	while((n=get_top_path(q,(void*)cmdname)))
	{
		//printf("PATH=##%s##\n",n);
		if(stat(n,&ssss)==0)
		{
			//release_top_path(n);
			//release_path(p);
			goto next;
		}
		release_top_path(n);
		q=next_path(q);
	}
	release_path(p);

	return -1;
	
next:
	
	if(mode==MYP_OVERLAY)
	{
		execv(n,(void*)argv);
//		execvp(n,(void*)argv);
		ret=-1;
		goto end;
	}
	else pid = vfork();
	if (pid) {
		if(mode==MYP_WAIT)
		{
			ret= mywait(pid);
			goto end;
		}
		ret=pid;
		goto end;
	}
	else if (pid == 0)
	{
		execv(n,(void*)argv);
//		execvp(n,(void*)argv);
		_exit(1);
	}
end:
	if(n)release_top_path(n);
	if(p)release_path(p);

	return ret;
#else
	int pid;
	if(mode==MYP_OVERLAY)
	{
		execvp(cmdname,(void*)argv);
		return -1;
	}
	else pid = vfork();
	if (pid) {
		if(mode==MYP_WAIT)
		{
			return mywait(pid);
		}
//		else waitpid(0, NULL, WNOHANG);
//		fprintf( stderr, "pass spawnvp\n" ) ;
		return pid;
	}
	else if (pid == 0)
	{
		execvp(cmdname,(void*)argv);
		_exit(1);
	}
#endif
#endif
	return -1;
}



/* この関数はLINUXにはない */
intptr_t myspawnvpe( int mode, const char *cmdname, const char *const *argv, const char *const *envp )
{
#ifdef _WIN32
	return _spawnvpe(mode,cmdname,argv,envp);
#endif
#ifdef unix
#if 1	
	int pid;
	char *p=NULL,*q=NULL;
	char* n=NULL;
	struct stat ssss;
	int ret=-1;
	p=get_path();
	if(p==NULL)return -1;

	q=p;
	while((n=get_top_path(q,(void*)cmdname)))
	{
		//printf("PATH=##%s##\n",n);
		if(stat(n,&ssss)==0)
		{
			//release_top_path(n);
			//release_path(p);
			goto next;
		}
		release_top_path(n);
		q=next_path(q);
	}
	release_path(p);

	return -1;
	
next:
	
	if(mode==MYP_OVERLAY)
	{
		execve(n,(void*)argv,(void*)envp);
		ret=-1;
		goto end;
	}
	else pid = vfork();
	if (pid) {
		if(mode==MYP_WAIT)
		{
			ret= mywait(pid);
			goto end;
		}
		ret=pid;
		goto end;
	}
	else if (pid == 0)
	{
		execve(n,(void*)argv,(void*)envp);
		_exit(1);
	}
end:
	if(n)release_top_path(n);
	if(p)release_path(p);

	return ret;
#else
	int pid;
	if(mode==MYP_OVERLAY)
	{
		execvpe(cmdname,(void*)argv,envp);
		return -1;
	}
	else pid = vfork();
	if (pid) {
		if(mode==MYP_WAIT)
		{
			return mywait(pid);
		}
		return pid;
	}
	else if (pid == 0)
	{
		execvpe(cmdname,(void*)argv,envp);
		_exit(1);
	}
#endif
#endif /* unix */
	return -1;
}




intptr_t myspawnlpe( int mode, const char *cmdname, const char *arg0, ...)
{
	int i;
	const char* argv[VA_SIZE];
	const char *const *envp;
	void *vp;
	va_list marker;

	argv[0]=arg0;

	if(arg0)
	{
		va_start( marker, arg0 );
		for(i=0;i<VA_SIZE-2;i++)
		{
			vp = (void*)va_arg( marker, void*);
			argv[1+i]=vp;
			if(vp==NULL)break;
		}
		argv[1+i]=NULL;
	}
	envp = (const char *const *)va_arg( marker, void*);
	return myspawnvpe(mode,cmdname,argv,envp);

}


int mywait(intptr_t pid)
{
	int ret=-1;
	if(pid<1)return ret;
	
#ifdef _WIN32
	_cwait(&ret,pid,0);
	return ret;
#endif
#ifdef unix
	waitpid(pid,&ret,0);
	if(WIFEXITED(ret))
	{
		return WEXITSTATUS(ret);
	}
	return -1;
#endif
	return ret;
}

int mynwait(intptr_t pid,int* status)
{
	int ret=-1;
	int result;
	if(pid<1)return ret;
	
	if(status)*status=0;

#ifdef _WIN32
	result=WaitForSingleObject((HANDLE)pid,0);
	if(result==WAIT_OBJECT_0)
	{
		int st;
		GetExitCodeProcess((HANDLE)pid, (LPDWORD)((void*)&st));
	    CloseHandle((HANDLE)pid);
		if(status)*status=st;
		return 1;
	}
	if(result==WAIT_TIMEOUT)
	{
		return 0;
	}
	return ret;
#endif

#ifdef unix
	result=waitpid(pid,&ret,WNOHANG);
//	if ( result < 0 )
//		fprintf( stderr, "[mynwait] waitpid=%d\n", result ) ;
	if(result>0)
	{
		if(WIFEXITED(ret))
		{
			 if(status)*status=WEXITSTATUS(ret);
		}
		result=1;
	}
	return result;
#endif
	return ret;
}

//シグナルを送るときに使う
int handle_2_pid(intptr_t hProcess)
{ 
#ifdef _WIN32
	DWORD   status;
	DWORD pbi[6];
	int (__stdcall *NtQueryInformationProcess)(PVOID,DWORD,PVOID,ULONG,PULONG);
	HANDLE h=NULL;

	pbi[4]=0;
 
	h=LoadLibrary("NTDLL.DLL");
	if(h==NULL)goto end;

	NtQueryInformationProcess=(int (__stdcall *)(PVOID,DWORD,PVOID,ULONG,PULONG))GetProcAddress(h,"NtQueryInformationProcess");
	if(NtQueryInformationProcess==NULL)goto end;

	status = (*NtQueryInformationProcess)((void*)hProcess, 0,(void*)(&pbi),sizeof(pbi), NULL);
	if (status != 0)goto end;

end:
	 if(h)FreeLibrary(h);

	 return pbi[4];
#else
	return hProcess;
#endif

} 


#if 0
#ifdef _CONSOLE


main()
{
	int i,ret,st,pid,gpid;

	
	pid=spawnlp(P_NOWAIT,"ls","ls",NULL);
	gpid=handle_2_pid(pid);
	printf("pid=%d\n",pid);
	printf("gpid=%d\n",gpid);

	for(i=0;i<15;i++)
	{
		ret=mynwait(pid,&st);
		printf("ret=%d  st=%d\n",ret,st);
#ifdef _WIN32
		Sleep(1000);
#endif
#ifdef unix
		sleep(1);
#endif
	}
	printf("end\n");
}

#endif
#endif



#if 0

int main(int argc,char *argv[])
{
	int ret;
	char *envp[]={
		"PATH=c:/cygwin/bin",
		NULL,
	};
	ret=(int)myspawnlpe(MYP_NOWAIT,"printenv","printenv",NULL,envp);
	printf("ret=%d\n",ret);
}

#endif


