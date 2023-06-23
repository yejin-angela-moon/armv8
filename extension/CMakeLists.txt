cmake_minimum_required(VERSION 3.23)
project(extension C)

set(CMAKE_C_STANDARD 11)

add_executable(extension
        iq_fit/src/iq_fit.c iq_fit/src/board.c iq_fit/src/board.h iq_fit/src/definitions.h iq_fit/src/piece.c iq_fit/src/piece.h iq_fit/src/iq_fit.h iq_fit/src/utilities.c iq_fit/src/utilities.h)
