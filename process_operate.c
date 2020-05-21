#include <stdio.h>
#include <windows.h>
#include "process_operate.h"

void SimpleZeroMemory(STARTUPINFO* si, PROCESS_INFORMATION* pi) {
    // 用0为决定新进程的主窗体如何显示的STARTUPINFO结构体填充内存
    ZeroMemory(si, sizeof(*si));
    si->cb = sizeof(*si);
    // 用0为新进程的识别信息的PROCESS_INFORMATION结构体填充内存
    ZeroMemory(pi, sizeof(*pi));
}

int SimpleCreateProcess(LPTSTR exeName, STARTUPINFO* si, PROCESS_INFORMATION* pi) {
    SimpleZeroMemory(si, pi);
    int createResult = CreateProcess(NULL, exeName, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, si, pi);
    return createResult;
}

BOOL SimpleReadDataOfProcess(DWORD processId, LPCVOID addr, PVOID data, int readSize) {
    // 打开一个存在进程，获取进程句柄
    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (process == NULL) {
        return FALSE;
    }
    // 接受更改后的页面属性
    DWORD protectAttribute = 0;
    // 改变在特定进程中内存区域的保护属性
    BOOL vpeRes = VirtualProtectEx(process, addr, readSize, PAGE_READWRITE, &protectAttribute);
    if (vpeRes == FALSE) {
        CloseHandle(process);
        return FALSE;
    }
    // 定义接受读取数据长度变量
    int realitySize = 0;
    // 读取数据
    BOOL rpmRes = ReadProcessMemory(process, addr, data, readSize, &realitySize);
    if (rpmRes == FALSE || realitySize == 0) {
        CloseHandle(process);
        return FALSE;
    }
    // 还原保护属性
    VirtualProtectEx(process, addr, readSize, protectAttribute, &protectAttribute);
    // 关闭进程句柄
    CloseHandle(process);
    return TRUE;
}

BOOL SimpleWriteDataOfProcess(DWORD processId, LPCVOID addr, LPVOID data, int writeSize) {
    // 打开一个存在进程，获取进程句柄
    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (process == NULL) {
        return FALSE;
    }
    // 接受更改后的页面属性
    DWORD protectAttribute = 0;
    // 改变在特定进程中内存区域的保护属性
    BOOL vpeRes = VirtualProtectEx(process, addr, writeSize, PAGE_READWRITE, &protectAttribute);
    if (vpeRes == FALSE) {
        CloseHandle(process);
        return FALSE;
    }
    // 写入数据实际长度
    int realitySize = 0;
    // 写入数据
    BOOL wpmRes = WriteProcessMemory(process, addr, data, writeSize + 1, &realitySize);
    if (wpmRes == FALSE || realitySize == 0) {
        CloseHandle(process);
        return FALSE;
    }
    // 还原保护属性
    VirtualProtectEx(process, addr, writeSize, protectAttribute, &protectAttribute);
    // 关闭进程句柄
    CloseHandle(process);
    return TRUE;
}

HANDLE SimpleCreateMutex(LPCTSTR mutexName) {
    HANDLE mutex = CreateMutex(NULL, FALSE, mutexName);
    if (GetLastError() == ERROR_ACCESS_DENIED) {
        return SimpleOpenMutex(mutexName);
    }
    return mutex;
}

HANDLE SimpleOpenMutex(LPCTSTR mutexName) {
    HANDLE mutex = OpenMutex(MUTEX_ALL_ACCESS, TRUE, mutexName);
    return mutex;
}
