cmake_minimum_required(VERSION 2.8)

PROJECT(demo)

set(CMAKE_BUILD_TYPE "Release")

SET(SRC_DIR ../../src)

INCLUDE_DIRECTORIES(${SRC_DIR}/crypto)
INCLUDE_DIRECTORIES(${SRC_DIR}/psi)


FILE(GLOB crypto_SRC ${SRC_DIR}/crypto/*.c )
FILE(GLOB psi_SRC ${SRC_DIR}/psi/*.c )


ADD_EXECUTABLE(demo ${crypto_SRC} ${psi_SRC} demo.c)
cmake_minimum_required(VERSION 2.8)
