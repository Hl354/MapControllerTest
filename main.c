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
    printf("0: ��ǰ�߳�����\n");
    printf("1: �������ߴ򿪹����ڴ�\n");
    printf("2: ��ȡ�ļ�����\n");
    printf("3: ��ȡ��������\n");
    printf("4: д�빲������\n");
    printf("5: ��������ʵ��\n");
    printf("6: �ļ�����д�빲���ڴ�\n");
    printf("7: �����ڴ���������ļ�\n");
    printf("8: ��ȡ�߳�����\n");
    printf("9: д���߳�����\n");
    printf("10:�ļ�����д���߳�\n");
    printf("11:�߳�����д���ļ�\n");
    printf("12:�ر��߳�\n");
    printf("13:�رճ���\n");
    printf("==============================\n");
}

void SelectOperate() {
    int select = 0;
    // ����һ���̳߳�ʼ���ݣ��������߳̿��Եõ�
    char message[MAX_LENGTH] = "Process Data";
    // �����ڴ�����
    HANDLE fileHandler;
    HANDLE fileMapping;
    LPVOID memoryPoint;
    // ���������
    HANDLE memoryMutex = SimpleCreateMutex(Mutex_NAME);
    // �߳�����
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    // �����߳�����
    char processData[MAX_LENGTH];
    // ״̬��
    DWORD statusCode;
    // ��Ҫ��ȡ��д�����ݵ��̺߳ź�������ʼ��ַ
    int pid;
    int addr;
    // �ַ������ձ���
    char *str;
    // ��������buffer
    char buffer[MAX_LENGTH];
    while (1) {
        CreateMainMenu();
        scanf("%d", &select);
        switch (select) {
            case 0:
                printf("��ǰ���̵Ľ��̺�Ϊ��%d, ��ʼ����Ϊ��%s, ���ݵ�ַΪ: 0x%p\n", GetCurrentProcessId(), message, &message);
                break;
            case 1:
                fileHandler = SimpleCreateFileHandler(FILE_PATH);
                if (fileHandler == INVALID_HANDLE_VALUE) {
                    printf("�����ļ����������ڴ�ʧ��\n");
                    break;
                }
                fileMapping = SimpleCreateFileMapping(fileHandler, "TestSharedName");
                if (fileHandler == NULL) {
                    printf("�����ļ�ӳ��ʧ��\n");
                    break;
                }
                memoryPoint = SimpleMapViewOfFile(fileMapping);
                if (memoryPoint == NULL) {
                    printf("��ȡ�ڴ�ָ��ʧ��\n");
                    break;
                }
                printf("���ٻ��ߴ򿪹����ڴ�ɹ�\n");
                break;
            case 2:
                printf("�ļ�����Ϊ:\n%s\n", SimpleReadFile(FILE_PATH));
                break;
            case 3:
                if (memoryPoint == NULL) {
                    printf("��ǰ�����ڴ�δ���ٳɹ�����δ��\n");
                    break;
                }
                waitMutex(memoryMutex);
                printf("�����ڴ�����:\n%s\n", (char *) memoryPoint);
                ReleaseMutex(memoryMutex);
                break;
            case 4:
                if (memoryPoint == NULL) {
                    printf("��ǰ�����ڴ�δ���ٳɹ�����δ��\n");
                    break;
                }
                waitMutex(memoryMutex);
                printf("������һ����ʮ�ַ��ڵ�����\n");
                GetStringByInput(buffer);
                strcpy((char *) memoryPoint, buffer);
                ReleaseMutex(memoryMutex);
                printf("�����ɹ�\n");
                break;
            case 5:
                printf("������һ����ʮ�ַ��ڵ�.exe�߳����ƣ��趨��exe��process.exe\n");
                GetStringByInput(buffer);
                if (SimpleCreateProcess(buffer, &si, &pi) == 0) {
                    printf("����ʧ�ܣ�������:%d\n", GetLastError());
                    break;
                }
                printf("���������̳߳ɹ�\n");
                break;
            case 6:
                if (memoryPoint == NULL) {
                    printf("��ǰ�����ڴ�δ���ٳɹ�����δ��\n");
                    break;
                }
                waitMutex(memoryMutex);
                strcpy((char *) memoryPoint, SimpleReadFile(FILE_PATH));
                printf("�����ɹ�\n");
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
                    printf("��ȡ��������ʧ��\n");
                    break;
                }
                printf("�߳�����: %s\n", processData);
                break;
            case 9:
                inputProcessInfo(&pid, &addr);
                printf("����һ����ʮ���ַ��ڵ�����\n");
                GetStringByInput(buffer);
                if (!SimpleWriteDataOfProcess(pid, (LPCVOID)addr, (LPCVOID)buffer, strlen(buffer))) {
                    printf("д���������ʧ��\n");
                    break;
                }
                printf("д��ɹ�\n");
                break;
            case 10:
                inputProcessInfo(&pid, &addr);
                str = SimpleReadFile(FILE_PATH);
                if (!SimpleWriteDataOfProcess(pid, (LPCVOID)addr, (LPCVOID)str, strlen(str))) {
                    printf("д���������ʧ��\n");
                    break;
                }
                printf("д��ɹ�\n");
                break;
            case 11:
                inputProcessInfo(&pid, &addr);
                if (!SimpleReadDataOfProcess(pid, (LPCVOID)addr, (LPCVOID)processData, MAX_LENGTH)) {
                    printf("��ȡ��������ʧ��\n");
                    break;
                }
                SimpleWriteFile(FILE_PATH, processData);
                printf("д��ɹ�\n");
                break;
            case 12:
                statusCode = TerminateProcess(pi.hProcess, -1);
                if (statusCode == 0) {
                    printf("�رս���ʧ��\n");
                    break;
                }
                printf("�رս��̳ɹ�\n");
                break;
            case 13:
                SimpleReleaseSharedMemory(fileHandler, fileMapping, memoryPoint);
                ReleaseMutex(memoryMutex);
                CloseHandle(memoryMutex);
                printf("�رչ����ڴ�ɹ�\n");
                printf("�ͷ��ڴ���Դ�ɹ�\n");
                return;
            default:
                printf("�������");
                break;
        }
    }
}

void inputProcessInfo(int *pid, int *addr) {
    printf("��������̺�\n");
    scanf("%d", pid);
    printf("������������ݵ�ַ\n");
    scanf("%x", addr);
}

void GetStringByInput(char *buffer) {
    scanf("%s", buffer);
}

void waitMutex(HANDLE memoryMutex) {
    printf("�ȴ���ȡ������\n");
    WaitForSingleObject(memoryMutex, INFINITE);
    printf("�ɹ���ȡ������\n");
}