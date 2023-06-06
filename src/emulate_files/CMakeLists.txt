cmake_minimum_required(VERSION 3.23)
project(emulate_files C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(emulate_files
        B.c
        B.h
        definitions.h
        DPImm.c
        DPImm.h
        DPReg.c
        DPReg.h
        ioutils.c
        ioutils.h
        SDT.c
        SDT.h
        utilities.c
        utilities.h)
