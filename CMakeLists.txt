cmake_minimum_required(VERSION 3.23)
project(armv8_48 C)

set(CMAKE_C_STANDARD 11)

include_directories(src/assembler_files)
include_directories(src/emulate_files)

add_executable(armv8_48
        src/assembler_files/B.c
        src/assembler_files/B.h
        src/assembler_files/definitions.h
        src/assembler_files/DP.c
        src/assembler_files/DP.h
        src/assembler_files/ioutils.c
        src/assembler_files/ioutils.h
        src/assembler_files/SDT.c
        src/assembler_files/SDT.h
        src/assembler_files/symbol_table.c
        src/assembler_files/utilities.c
        src/assembler_files/utilities.h
        src/emulate_files/B.c
        src/emulate_files/B.h
        src/emulate_files/definitions.h
        src/emulate_files/DPImm.c
        src/emulate_files/DPImm.h
        src/emulate_files/DPReg.c
        src/emulate_files/DPReg.h
        src/emulate_files/ioutils.c
        src/emulate_files/ioutils.h
        src/emulate_files/SDT.c
        src/emulate_files/SDT.h
        src/emulate_files/utilities.c
        src/emulate_files/utilities.h
        src/assemble.c
        src/emulate.c)
