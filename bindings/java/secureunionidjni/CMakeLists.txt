CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
PROJECT(secureunionidjni C)

SET(CMAKE_C_STANDARD 11)

IF(NOT DEFINED ENV{JAVA_HOME})
    message(FATAL_ERROR "not defined environment variable:JAVA_HOME")  
ENDIF()

IF (WIN32)
    MESSAGE(STATUS "Now is windows")
    SET(JDK_SUB win32 )
    SET(SO_TYPE win64 )
ELSEIF (APPLE)
    MESSAGE(STATUS "Now is Apple systens.")
    SET(JDK_SUB darwin )
    SET(SO_TYPE osx )
ELSEIF (UNIX)
    MESSAGE(STATUS "Now is UNIX-like OS's.")
    SET(JDK_SUB linux )
    SET(SO_TYPE linux64 )
ENDIF ()

SET(SRC_DIR ../../../src)

# jdk header file
INCLUDE_DIRECTORIES($ENV{JAVA_HOME}/include)
INCLUDE_DIRECTORIES($ENV{JAVA_HOME}/include/${JDK_SUB})

INCLUDE_DIRECTORIES(${SRC_DIR}/crypto)
INCLUDE_DIRECTORIES(${SRC_DIR}/psi)


FILE(GLOB crypto_SRC ${SRC_DIR}/crypto/*.c )
FILE(GLOB psi_SRC ${SRC_DIR}/psi/*.c )

SET(jni_SRC ${PROJECT_SOURCE_DIR}/src/SecureUnionID_wrap.c)

# output path
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../secureunionid/src/main/resources)

ADD_LIBRARY(secureunionidjni-${SO_TYPE} SHARED  ${crypto_SRC} ${psi_SRC} ${jni_SRC} )
