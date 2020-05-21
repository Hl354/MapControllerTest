cmake_minimum_required(VERSION 3.16)
project(MapControllerTest C)

set(CMAKE_C_STANDARD 11)

add_executable(MapControllerTest main.c mapping_operate.h mapping_operate.c file_operate.h file_operate.c process_operate.h process_operate.c)