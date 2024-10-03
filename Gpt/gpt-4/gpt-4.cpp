// gpt-4.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include <tlhelp32.h>

// 定义一些常量
#define MAX_PATH_LEN 260

// 定义一些类型
typedef HMODULE(WINAPI *LoadLibraryA_t)(LPCSTR);
typedef FARPROC(WINAPI *GetProcAddress_t)(HMODULE, LPCSTR);
typedef BOOL(WINAPI *DllMain_t)(HMODULE, DWORD, LPVOID);

// 函数声明
BOOL inject_dll(const char *target_process_name, const char *dll_path);
DWORD find_process_id(const char *process_name);
BOOL is_valid_PE(const uint8_t *data);
BOOL manual_map_dll(HANDLE h_process, const char *dll_path, uint8_t *mapped_dll, size_t mapped_dll_size);
uint8_t *load_file_to_memory(const char *file_path, size_t *file_size);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <target_process_name> <dll_path>\n", argv[0]);
        return 1;
    }

    if (!inject_dll(argv[1], argv[2])) {
        printf("Failed to inject DLL.\n");
        return 1;
    }

    printf("DLL injected successfully.\n");
    return 0;
}

BOOL inject_dll(const char *target_process_name, const char *dll_path) {
    // 获取目标进程ID
    DWORD process_id = find_process_id(target_process_name);
    if (process_id == 0) {
        printf("Error: Could not find process '%s'.\n", target_process_name);
        return FALSE;
    }

    // 打开目标进程
    HANDLE h_process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);
    if (h_process == NULL) {
        printf("Error: Could not open process (Error code: %lu).\n", GetLastError());
        return FALSE;
    }

    // 将DLL加载到目标进程的内存中
    size_t mapped_dll_size;
    uint8_t *mapped_dll = load_file_to_memory(dll_path, &mapped_dll_size);
    if (mapped_dll == NULL || !is_valid_PE(mapped_dll)) {
        printf("Error: Invalid DLL file.\n");
        if (mapped_dll != NULL) {
            VirtualFree(mapped_dll, 0, MEM_RELEASE);
        }
        CloseHandle(h_process);
        return FALSE;
    }

    if (!manual_map_dll(h_process, dll_path, mapped_dll, mapped_dll_size)) {
        printf("Error: Failed to map DLL into target process.\n");
        VirtualFree(mapped_dll, 0, MEM_RELEASE);
        CloseHandle(h_process);
        return FALSE;
    }

    VirtualFree(mapped_dll, 0, MEM_RELEASE);
    CloseHandle(h_process);
    return TRUE;
}

// 在这里实现其他辅助函数，例如 find_process_id、is_valid_PE、manual_map_dll 和 load_file_to_memory
