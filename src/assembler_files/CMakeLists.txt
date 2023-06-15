cmake_minimum_required(VERSION 3.23)
project(assembler_files C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(assembler_files
        B.c
        B.h
        definitions.h
        DP.c
        DP.h
        ioutils.c
        ioutils.h
        SDT.c
        SDT.h
        utilities.c
        utilities.h)
