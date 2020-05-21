#include <stdio.h>
#include <windows.h>
#include "file_operate.h"
#include "mapping_operate.h"
#include "process_operate.h"

#define MAX_LENGTH 50
#define Mutex_NAME "MemoryMutex"

void CreateMainMenu();

void SelectOperate();

void GetStringByInput(char *buffer);

void inputProcessInfo(int *pid, int *addr);

void waitMutex(HANDLE memoryMutex);

int main() {
    SelectOperate();
    return 0;
}

void CreateMainMenu() {
    printf("==============================\n");
    printf("0: 当前线程内容\n");
    printf("1: 创建或者打开共享内存\n");
    printf("2: 读取文件数据\n");
    printf("3: 读取共享数据\n");
    printf("4: 写入共享数据\n");
    printf("5: 创建进程实例\n");
    printf("6: 文件数据写入共享内存\n");
    printf("7: 共享内存数据输出文件\n");
    printf("8: 读取线程内容\n");
    printf("9: 写入线程内容\n");
    printf("10:文件内容写入线程\n");
    printf("11:线程内容写入文件\n");
    printf("12:关闭线程\n");
    printf("13:关闭程序\n");
    printf("==============================\n");
}

void SelectOperate() {
    int select = 0;
    // 定义一个线程初始数据，让其它线程可以得到
    char message[MAX_LENGTH] = "Process Data";
    // 共享内存数据
    HANDLE fileHandler;
    HANDLE fileMapping;
    LPVOID memoryPoint;
    // 互斥锁句柄
    HANDLE memoryMutex = SimpleCreateMutex(Mutex_NAME);
    // 线程数据
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    // 接收线程数据
    char processData[MAX_LENGTH];
    // 状态码
    DWORD statusCode;
    // 需要读取和写入数据的线程号和数据起始地址
    int pid;
    int addr;
    // 字符串接收变量
    char *str;
    // 键盘输入buffer
    char buffer[MAX_LENGTH];
    while (1) {
        CreateMainMenu();
        scanf("%d", &select);
        switch (select) {
            case 0:
                printf("当前进程的进程号为：%d, 起始数据为：%s, 数据地址为: 0x%p\n", GetCurrentProcessId(), message, &message);
                break;
            case 1:
                fileHandler = SimpleCreateFileHandler(FILE_PATH);
                if (fileHandler == INVALID_HANDLE_VALUE) {
                    printf("根据文件创建共享内存失败\n");
                    break;
                }
                fileMapping = SimpleCreateFileMapping(fileHandler, "TestSharedName");
                if (fileHandler == NULL) {
                    printf("构建文件映射失败\n");
                    break;
                }
                memoryPoint = SimpleMapViewOfFile(fileMapping);
                if (memoryPoint == NULL) {
                    printf("获取内存指针失败\n");
                    break;
                }
                printf("开辟或者打开共享内存成功\n");
                break;
            case 2:
                printf("文件内容为:\n%s\n", SimpleReadFile(FILE_PATH));
                break;
            case 3:
                if (memoryPoint == NULL) {
                    printf("当前共享内存未开辟成功或者未打开\n");
                    break;
                }
                waitMutex(memoryMutex);
                printf("共享内存数据:\n%s\n", (char *) memoryPoint);
                ReleaseMutex(memoryMutex);
                break;
            case 4:
                if (memoryPoint == NULL) {
                    printf("当前共享内存未开辟成功或者未打开\n");
                    break;
                }
                waitMutex(memoryMutex);
                printf("请输入一个五十字符内的数据\n");
                GetStringByInput(buffer);
                strcpy((char *) memoryPoint, buffer);
                ReleaseMutex(memoryMutex);
                printf("操作成功\n");
                break;
            case 5:
                printf("请输入一个五十字符内的.exe线程名称，设定的exe有process.exe\n");
                GetStringByInput(buffer);
                if (SimpleCreateProcess(buffer, &si, &pi) == 0) {
                    printf("创建失败，错误码:%d\n", GetLastError());
                    break;
                }
                printf("创建并打开线程成功\n");
                break;
            case 6:
                if (memoryPoint == NULL) {
                    printf("当前共享内存未开辟成功或者未打开\n");
                    break;
                }
                waitMutex(memoryMutex);
                strcpy((char *) memoryPoint, SimpleReadFile(FILE_PATH));
                printf("操作成功\n");
                ReleaseMutex(memoryMutex);
                break;
            case 7:
                waitMutex(memoryMutex);
                SimpleWriteFile(FILE_PATH, (char *) memoryPoint);
                ReleaseMutex(memoryMutex);
                break;
            case 8:
                inputProcessInfo(&pid, &addr);
                if (!SimpleReadDataOfProcess(pid, (LPCVOID)addr, (LPCVOID)processData, MAX_LENGTH)) {
                    printf("读取进程数据失败\n");
                    break;
                }
                printf("线程数据: %s\n", processData);
                break;
            case 9:
                inputProcessInfo(&pid, &addr);
                printf("输入一个五十个字符内的数据\n");
                GetStringByInput(buffer);
                if (!SimpleWriteDataOfProcess(pid, (LPCVOID)addr, (LPCVOID)buffer, strlen(buffer))) {
                    printf("写入进程数据失败\n");
                    break;
                }
                printf("写入成功\n");
                break;
            case 10:
                inputProcessInfo(&pid, &addr);
                str = SimpleReadFile(FILE_PATH);
                if (!SimpleWriteDataOfProcess(pid, (LPCVOID)addr, (LPCVOID)str, strlen(str))) {
                    printf("写入进程数据失败\n");
                    break;
                }
                printf("写入成功\n");
                break;
            case 11:
                inputProcessInfo(&pid, &addr);
                if (!SimpleReadDataOfProcess(pid, (LPCVOID)addr, (LPCVOID)processData, MAX_LENGTH)) {
                    printf("读取进程数据失败\n");
                    break;
                }
                SimpleWriteFile(FILE_PATH, processData);
                printf("写入成功\n");
                break;
            case 12:
                statusCode = TerminateProcess(pi.hProcess, -1);
                if (statusCode == 0) {
                    printf("关闭进程失败\n");
                    break;
                }
                printf("关闭进程成功\n");
                break;
            case 13:
                SimpleReleaseSharedMemory(fileHandler, fileMapping, memoryPoint);
                ReleaseMutex(memoryMutex);
                CloseHandle(memoryMutex);
                printf("关闭共享内存成功\n");
                printf("释放内存资源成功\n");
                return;
            default:
                printf("输入错误");
                break;
        }
    }
}

void inputProcessInfo(int *pid, int *addr) {
    printf("请输入进程号\n");
    scanf("%d", pid);
    printf("请输入进程数据地址\n");
    scanf("%x", addr);
}

void GetStringByInput(char *buffer) {
    scanf("%s", buffer);
}

void waitMutex(HANDLE memoryMutex) {
    printf("等待获取互斥锁\n");
    WaitForSingleObject(memoryMutex, INFINITE);
    printf("成功获取互斥锁\n");
}