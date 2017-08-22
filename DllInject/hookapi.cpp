// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"

//关闭HOOK函数
void OffHook();

//HOOK函数
void OnHook();

char NewCode[5] = {};

char OldCode[5] = {};

//劫持MessageBox，替换的自定义函数
int WINAPI MyMessageBoxW(
    _In_opt_ HWND hWnd,
    _In_opt_ LPCWSTR lpText,
    _In_opt_ LPCWSTR lpCaption,
    _In_ UINT uType)
{

    DWORD dwResault = 0;
    lpText = L"你的按钮已经被Hook";
    OffHook();
    dwResault = MessageBox(hWnd, lpText, lpCaption, uType);
    OnHook();
    return dwResault;
}


void OnHook()
{
    DWORD dwOld = 0;
    //修改一块虚拟内存的属性，设置为可写可执行
    VirtualProtect(MessageBoxW, 1, PAGE_EXECUTE_READWRITE, &dwOld);
    memcpy(MessageBoxW, NewCode, 5);
    VirtualProtect(MessageBoxW, 1, dwOld, &dwOld);
}


void OffHook()
{
    DWORD dwOld = 0;
    VirtualProtect(MessageBoxW, 1, PAGE_EXECUTE_READWRITE, &dwOld);
    memcpy(MessageBoxW, OldCode, 5);
    VirtualProtect(MessageBoxW, 1, dwOld, &dwOld);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        //准备基本工作
        NewCode[0] = 0xE9; //实际上0xe9就相当于jmp指令  
        //地址偏移 = 目标地址-JMP所在地址-5
        DWORD dwOffset = (DWORD)MyMessageBoxW - (DWORD)MessageBoxW - 5;

        //*(DWORD*)(NewCode + 1) = dwOffset;
        memcpy(NewCode + 1, &dwOffset, 4);
        memcpy(OldCode, MessageBoxW, 5);
        //
        OnHook();
    }
    break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        //OffHook();
        break;
    }
    return TRUE;
}
