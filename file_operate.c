#include <stdio.h>
#include <string.h>
#include "file_operate.h"

char *SimpleReadFile(char *path) {
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        printf("Open file error");
        return NULL;
    }
    // 将文件指针指向尾部
    fseek(file, 0, SEEK_END);
    // 得到文件长度
    long size = ftell(file);
    // 重新将文件指针指向头部 因为还需要读取文件
    rewind(file);
    // 定义一个文件大小加一的数组
    char result[size];
    // 获取从文件中读取的大小
    size_t len = fread(result, 1, size, file);
    // 比较读取的大小是不是和之前得到的文件大小一致
    if (len != size) {
        printf("Read error");
        return NULL;
    }
    // 关闭文件
    fclose(file);
    return RecoverString(result);
}

void SimpleWriteFile(char* path, char* content) {
    FILE *file = fopen(path, "a+");
    if (file == NULL) {
        printf("Open file error");
        return;
    }
    // 先写入一个换行
    fprintf(file, "\n");
    // 将内容输出到文件中
    fprintf(file, content);
    // 关闭文件
    fclose(file);
}

char *RecoverString(char *string) {
    if (string == NULL) { return NULL; }
    // 获取字符长度
    int len = strlen(string);
    // 定义长度加一的新字符数组
    char newString[len + 1];
    // 将旧数组拷贝，这样可以修复得到一个带有结尾符的正确字符串
    strcpy(newString, string);
    return newString;
}

