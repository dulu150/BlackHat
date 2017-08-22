// MyDll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <stdio.h>  

extern "C" BOOL APIENTRY DllMain( HANDLE hModule,   
	DWORD  ul_reason_for_call,   
	LPVOID lpReserved  
	)  
{  
	MessageBoxA(NULL,"hello world","dll",MB_OK);      
	return TRUE;  
} 
