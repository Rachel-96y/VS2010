#pragma once

#ifndef _DLL_INIT_
#define _DLL_INIT_

extern "C" void __declspec(dllexport) SetHook(); // ��װHOOK
extern "C" void __declspec(dllexport) UnHook(); // ж��HOOK
LRESULT CALLBACK MessageHookProc(int nCode, WPARAM wParam, LPARAM lParam); //��Ϣ������
VOID Init(void);

#endif