#pragma once

#ifndef _DLL_INIT_
#define _DLL_INIT_

extern "C" void __declspec(dllexport) SetHook(); // 安装HOOK
extern "C" void __declspec(dllexport) UnHook(); // 卸载HOOK
LRESULT CALLBACK MessageHookProc(int nCode, WPARAM wParam, LPARAM lParam); //消息处理函数
VOID Init(void);

#endif