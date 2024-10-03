#include "new.h"

BOOL new1()
{
	printf("123");
	return 0;
}

// VEH异常处理函数
LONG CALLBACK VectoreHandler(PEXCEPTION_POINTERS ExceptionInfo)
{
	if ((DWORD)ExceptionInfo -> ExceptionRecord -> ExceptionAddress == (DWORD)new1)
	{
		MessageBoxA(0, "1", "1", 0);
		ExceptionInfo -> ContextRecord -> Eip += 0x6;
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	return EXCEPTION_CONTINUE_SEARCH;
}

int main(int arcg, char* argv[])
{
	LPVOID pVectoreHandle = AddVectoredExceptionHandler(1, VectoreHandler);
	if (!pVectoreHandle) MessageBoxA(0, "失败1", "失败1", 0);
	MessageBoxA(0, "0", "0", 0);
	HANDLE hThread = GetCurrentThread();
	CONTEXT context;
	context.ContextFlags = CONTEXT_ALL;
	GetThreadContext(hThread, &context);
	context.Dr0 = (DWORD)new1;
	context.Dr7 = 0x1;
	BOOL bRet = SetThreadContext(hThread, &context);
	new1();
	return 0;
}