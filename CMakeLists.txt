cmake_minimum_required(VERSION 3.26)
project(AD3_project C)

set(CMAKE_C_STANDARD 17)

add_executable(AD3_project
        src/main.c
        src/tree.h
        src/tree.c
        src/extract.c
        src/extract.h
        src/compress.c
        src/compress.h
        src/sort.c
        src/sort.h
        src/prefix_util.c
        src/prefix_util.h
        src/linked_list.c
        src/linked_list.h
)
