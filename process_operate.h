#include <windows.h>

#define ADDRESS (PVOID)0x400000

void SimpleZeroMemory(STARTUPINFO* si, PROCESS_INFORMATION* pi);

int SimpleCreateProcess(LPTSTR exeName, STARTUPINFO* si, PROCESS_INFORMATION* pi);

BOOL SimpleReadDataOfProcess(DWORD processId, LPCVOID addr, PVOID data, int readSize);

BOOL SimpleWriteDataOfProcess(DWORD processId, LPCVOID addr, LPVOID data, int writeSize);

HANDLE SimpleCreateMutex(LPCTSTR mutexName);

HANDLE SimpleOpenMutex(LPCTSTR mutexName);
