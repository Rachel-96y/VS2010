// CheckWriteFile.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>

#define BUFFER_SIZE 1024

void CALLBACK FileIOCompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped);

int main() {
    char directory[MAX_PATH];
    printf("������Ҫ���ӵ�Ŀ¼·����");
    scanf("%s", directory);

    // ����������
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
        printf("�޷���Ŀ¼\n");
        return 1;
    }

    // ��������ʼ��������
    DWORD dwBytesReturned;
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);

    // ��ʼ�����ļ��仯
    OVERLAPPED overlapped = { 0 };
    overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (ReadDirectoryChangesW(hDir, buffer, BUFFER_SIZE, TRUE, FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE, NULL, &overlapped, FileIOCompletionRoutine)) {
        printf("��ʼ����Ŀ¼��%s\n", directory);
        while (TRUE) {
            // �ȴ��¼�
            SleepEx(INFINITE, TRUE);
        }
    }

    // �رվ��
    CloseHandle(hDir);
    CloseHandle(overlapped.hEvent);

    return 0;
}

// �ص�����
void CALLBACK FileIOCompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped) {
    if (dwErrorCode == 0) {
        // ��ȡ�仯��Ϣ
        PFILE_NOTIFY_INFORMATION fileInfo = (PFILE_NOTIFY_INFORMATION)lpOverlapped->Pointer;
        do {
            // ��ӡ�ļ�·��
            WCHAR fileName[MAX_PATH];
            memset(fileName, 0, MAX_PATH);
            wcsncpy_s(fileName, MAX_PATH, fileInfo->FileName, fileInfo->FileNameLength / sizeof(WCHAR));
            printf("д���ļ���%ls\n", fileName);
            // ��ȡ��һ���仯��Ϣ
            fileInfo = (PFILE_NOTIFY_INFORMATION)((LPBYTE)fileInfo + fileInfo->NextEntryOffset);
        } while (fileInfo->NextEntryOffset);

        // ���������ļ��仯
        if (!ReadDirectoryChangesW(
                lpOverlapped->hEvent,
                lpOverlapped->Pointer,
                BUFFER_SIZE,
                TRUE,
                FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE,
                NULL,
                lpOverlapped,
                FileIOCompletionRoutine)) {
            printf("�޷���������Ŀ¼\n");
        }
    } else {
        printf("����Ŀ¼����\n");
    }
}