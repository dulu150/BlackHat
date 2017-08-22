#include <windows.h>  
#include <iostream>

int  main ()  
{  
	DWORD  pid =  8944;  
	LPVOID pAddress = 0;  
	LPCSTR dllpath = "D:\\100-tmp\\mydll\\Debug\\mydll.dll";
	int dwSize = lstrlenA(dllpath) + 1;  
	DWORD  write = 0;  
	BOOL bRet  = FALSE;  
	HANDLE hThread  = 0;  
	DWORD dwthread = 0;  

	PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE) GetProcAddress(GetModuleHandle(TEXT("kernel32")),"LoadLibraryA");  

	HANDLE  hPid = OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);  

	if ( NULL == hPid )  
	{  
		CloseHandle(hPid);  
	}  

	pAddress = VirtualAllocEx( hPid,0,0x1000,MEM_COMMIT,PAGE_EXECUTE_READWRITE);  

	if ( NULL == pAddress)  
	{  
		VirtualFreeEx(hPid,pAddress,0x1000,MEM_RELEASE);  
		CloseHandle(hPid);  
	}  

	bRet  = WriteProcessMemory(hPid,pAddress,dllpath,dwSize,&write);  

	if ( FALSE == bRet )  
	{  
		VirtualFreeEx(hPid,pAddress,0x1000,MEM_RELEASE);  
		CloseHandle(hPid);  
	}  

	hThread = CreateRemoteThread(hPid,NULL,0,pfnThreadRtn,pAddress,0,&dwthread);  
	DWORD dwErrorID = GetLastError();
	std::cout << dwErrorID << std::endl;

	if ( NULL == hThread)  
	{  
		VirtualFreeEx(hPid,pAddress,0x1000,MEM_RELEASE);  
		CloseHandle(hPid);  
	}  

	

	return 0;     
} 
