#include <stdio.h>
#include <windows.h>
#include "mapping_operate.h"

HANDLE SimpleCreateFileMapping(HANDLE hFile, LPCTSTR sharedName) {
    HANDLE result = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, MAPPING_MEMORY_SIZE, sharedName);
    // 错误类型等于ERROR_ALREADY_EXISTS，说明文件映射已经存在直接打开
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        result = SimpleOpenFileMapping(sharedName);
    }
    return result;
}

HANDLE SimpleOpenFileMapping(LPCTSTR sharedName) {
    HANDLE result = OpenFileMapping(FILE_MAP_ALL_ACCESS, NULL, sharedName);
    return result;
}

LPVOID SimpleMapViewOfFile(HANDLE hMapping) {
    LPVOID result = MapViewOfFile(hMapping, FILE_MAP_ALL_ACCESS, 0, 0, MAPPING_MEMORY_SIZE);
    return result;
}

HANDLE SimpleCreateFileHandler(LPCTSTR path) {
    HANDLE result = CreateFile("a.txt", SIMPLE_POWER, SIMPLE_SHARE_MODE, NULL, OPEN_ALWAYS, FILE_FLAG_NO_BUFFERING, NULL);
    return result;
}

void SimpleReleaseSharedMemory(HANDLE file, HANDLE mappingFile, LPVOID memoryPointer) {
    // 释放文件映射
    UnmapViewOfFile(memoryPointer);
    memoryPointer = NULL;
    // 关闭内存映射文件对象句柄
    CloseHandle(mappingFile);
    // 关闭文件句柄
    CloseHandle(file);
}
