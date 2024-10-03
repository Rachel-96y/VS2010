// CheckWriteFile.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>

#define BUFFER_SIZE 1024

void CALLBACK FileIOCompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped);

int main() {
    char directory[MAX_PATH];
    printf("请输入要监视的目录路径：");
    scanf("%s", directory);

    // 创建监视器
    HANDLE hDir = CreateFileA(
        directory,
        FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
        NULL
    );

    if (hDir == INVALID_HANDLE_VALUE) {
        printf("无法打开目录\n");
        return 1;
    }

    // 创建并初始化缓冲区
    DWORD dwBytesReturned;
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);

    // 开始监视文件变化
    OVERLAPPED overlapped = { 0 };
    overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (ReadDirectoryChangesW(hDir, buffer, BUFFER_SIZE, TRUE, FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE, NULL, &overlapped, FileIOCompletionRoutine)) {
        printf("开始监视目录：%s\n", directory);
        while (TRUE) {
            // 等待事件
            SleepEx(INFINITE, TRUE);
        }
    }

    // 关闭句柄
    CloseHandle(hDir);
    CloseHandle(overlapped.hEvent);

    return 0;
}

// 回调函数
void CALLBACK FileIOCompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped) {
    if (dwErrorCode == 0) {
        // 获取变化信息
        PFILE_NOTIFY_INFORMATION fileInfo = (PFILE_NOTIFY_INFORMATION)lpOverlapped->Pointer;
        do {
            // 打印文件路径
            WCHAR fileName[MAX_PATH];
            memset(fileName, 0, MAX_PATH);
            wcsncpy_s(fileName, MAX_PATH, fileInfo->FileName, fileInfo->FileNameLength / sizeof(WCHAR));
            printf("写入文件：%ls\n", fileName);
            // 获取下一个变化信息
            fileInfo = (PFILE_NOTIFY_INFORMATION)((LPBYTE)fileInfo + fileInfo->NextEntryOffset);
        } while (fileInfo->NextEntryOffset);

        // 继续监视文件变化
        if (!ReadDirectoryChangesW(
                lpOverlapped->hEvent,
                lpOverlapped->Pointer,
                BUFFER_SIZE,
                TRUE,
                FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE,
                NULL,
                lpOverlapped,
                FileIOCompletionRoutine)) {
            printf("无法继续监视目录\n");
        }
    } else {
        printf("监视目录出错\n");
    }
}