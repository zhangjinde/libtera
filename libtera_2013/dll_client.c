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


#include <stdio.h>
#ifdef WIN32			/* For Windows */
#include <windows.h>
#endif /* WIN32 */
#if unix
#include <dlfcn.h>
#endif /* WIN32 */

#include "dll_client.h"


//#define ERR_PRINT

#ifdef ERR_PRINT
 #define EPRINT(a) fprintf(stderr,a)
 #define EPRINT2(a,b) fprintf(stderr,a,b)
#else
 #define EPRINT(a) do; while(0)
 #define EPRINT2(a,b) do; while(0)
#endif


/*----------------------------------------------------------*/
/*						Load DLL							*/
/*----------------------------------------------------------*/
void *dll_load(char *dllname)
{
	void *pFunclib=NULL;

	if(dllname==NULL) {
		EPRINT("### dllname is NULL\n");
		return NULL;
	}
#ifdef _WIN32
	pFunclib = LoadLibrary(dllname);
#endif

#ifdef unix
	pFunclib = dlopen(dllname, RTLD_LAZY);
#endif
	if(!pFunclib) {
		EPRINT2("### cannot dll_load >%s<\n",dllname);
	}
	return pFunclib;
}

/*----------------------------------------------------------*/
/*						Access DLL							*/
/*----------------------------------------------------------*/
void *dll_access(void *pDllmod, char *funcname)
{
	void *pDll_func=NULL;

	if(pDllmod==NULL) {
		EPRINT("### dll_access handle is NULL\n");
		return NULL;
	}
	if(funcname==NULL) {
		EPRINT("### dll_access funcname is NULL\n");
		return NULL;
	}
#ifdef _WIN32
	pDll_func = GetProcAddress((HMODULE)pDllmod, funcname); 
#endif

#ifdef unix
	pDll_func = dlsym(pDllmod, funcname);
#endif
	if(!pDll_func) {
		EPRINT2("### cannot dll_access >%s<\n",funcname);
	}
	return pDll_func;
}

/*----------------------------------------------------------*/
/*						Close DLL							*/
/*----------------------------------------------------------*/
void dll_close(void *pDllmod)
{
	if(pDllmod==NULL)return;
#ifdef _WIN32
	FreeLibrary(pDllmod);
#endif

#ifdef unix
	dlclose(pDllmod);
#endif
	return;
}


