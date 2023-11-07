cmake_minimum_required(VERSION 3.26)
project(AD3_project C)

set(CMAKE_C_STANDARD 17)

add_executable(AD3_project
        src/main.c
        src/tree/tree.h
        src/tree/tree.c
        src/extract.c
        src/extract.h
        src/compress.c
        src/compress.h
        src/sort.c
        src/sort.h
        src/tree/prefix_util.c
        src/tree/prefix_util.h
        src/tree/linked_list.c
        src/tree/linked_list.h
)
