#include <stdio.h>
#include <windows.h>

// 映射内存大小，可调
#define MAPPING_MEMORY_SIZE  1024 * 8
// 创建文件句柄时，复合读写，产生可读可写类型
#define SIMPLE_POWER GENERIC_READ | GENERIC_WRITE
// 可读可写共享模式
#define SIMPLE_SHARE_MODE FILE_SHARE_READ | FILE_SHARE_WRITE
// 文件路径
#define FILE_PATH "a.txt"

// 创建文件映射
HANDLE SimpleCreateFileMapping(HANDLE hFile, LPCTSTR sharedName);

// 打开文件映射
HANDLE SimpleOpenFileMapping(LPCTSTR sharedName);

// 关联文件映射
LPVOID SimpleMapViewOfFile(HANDLE hMapping);

// 创建文件句柄，创建的句柄可以当作文件映射载体
HANDLE SimpleCreateFileHandler(LPCTSTR path);

// 释放共享内存
void SimpleReleaseSharedMemory(HANDLE file, HANDLE mappingFile, LPVOID memoryPointer);