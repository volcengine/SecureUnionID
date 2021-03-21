CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

SET(CMAKE_C_STANDARD 11)

IF (WIN32)
    MESSAGE(STATUS "It is windows OS.")
    message(FATAL_ERROR "Only support Linux currently")
ELSEIF (APPLE)
    MESSAGE(STATUS "It is Apple OS.")
    message(FATAL_ERROR "Only support Linux currently")
ELSEIF (UNIX)
    MESSAGE(STATUS "It is Linux OS.")
ENDIF ()

SET(SRC_DIR ../../src)

# python header file
if (PYTHON2)
    SET(PYTHON_C_FLAGS `python-config --cflags`)
else()
    SET(PYTHON_C_FLAGS `python3-config --cflags`)
endif()

#set(CMAKE_C_FLAGS_RELEASE $(PYTHON_C_FLAGS))
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} $(PYTHON_C_FLAGS)")

INCLUDE_DIRECTORIES(${SRC_DIR}/crypto)
INCLUDE_DIRECTORIES(${SRC_DIR}/psi)


FILE(GLOB crypto_SRC ${SRC_DIR}/crypto/*.c )
FILE(GLOB psi_SRC ${SRC_DIR}/psi/*.c )

if (PYTHON2)
    find_package(PythonLibs 2 REQUIRED)
    include_directories(${PYTHON_INCLUDE_DIRS})
    SET(wrap_SRC ${PROJECT_SOURCE_DIR}/SecureUnionID_wrap_py2.c)
else()
    find_package(PythonLibs 3 REQUIRED)
    include_directories(${PYTHON_INCLUDE_DIRS})
    SET(wrap_SRC ${PROJECT_SOURCE_DIR}/SecureUnionID_wrap_py3.c)
endif()


# output path
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/)

ADD_LIBRARY(SecureUnionID SHARED  ${crypto_SRC} ${psi_SRC} ${wrap_SRC} )
set_target_properties(SecureUnionID PROPERTIES PREFIX "_")

