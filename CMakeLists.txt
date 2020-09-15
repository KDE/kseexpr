# Copyright Disney Enterprises, Inc.  All rights reserved.
# Copyright (C) 2020 L. E. Segovia <amy@amyspark.me>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License
# and the following modification to it: Section 6 Trademarks.
# deleted and replaced with:
#
# 6. Trademarks. This License does not grant permission to use the
# trade names, trademarks, service marks, or product names of the
# Licensor and its affiliates, except as required for reproducing
# the content of the NOTICE file.
#
# You may obtain a copy of the License at
# http://www.apache.org/licenses/LICENSE-2.0

cmake_minimum_required(VERSION 3.8.0)

# option() honors normal variables. Needed for LLVM 10.
if (POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif()
# Minimum for SeExpr is 11. LLVM (if used) may need 14.
set(CMAKE_CXX_STANDARD 17)

## project name & version
project(SeExpr2)

# "API" version
set(${PROJECT_NAME}_VERSION "3.4.4.0")
set(${PROJECT_NAME}_MAJOR_VERSION 3)
set(${PROJECT_NAME}_MINOR_VERSION 0)

enable_testing()

## Silence installation messages
set(CMAKE_INSTALL_MESSAGE LAZY)

# macros
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(GenerateExportHeader)
LIST (APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/src/build")
include(macros)
include(CheckCXXSourceCompiles)

# Default installation prefix
if (NOT DEFINED FLAVOR)
    set(FLAVOR "optimize" CACHE STRING "Build flavor")
endif()

## Choose build options
# Disney specific method of choosing variant
if (${FLAVOR} STREQUAL "optimize")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "type of build" FORCE)
endif()

if (${FLAVOR} STREQUAL "debug")
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "type of build" FORCE)
endif()

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    execute_process(
        COMMAND sh -c "echo `uname -s`-`uname -r | cut -d- -f1`-`uname -m`"
        OUTPUT_VARIABLE VARIANT_DIRECTORY OUTPUT_STRIP_TRAILING_WHITESPACE)
    set(CMAKE_INSTALL_PREFIX
        "${CMAKE_SOURCE_DIR}/${VARIANT_DIRECTORY}-${FLAVOR}"
        CACHE PATH "Installation prefix" FORCE)
endif()

message(STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")
message(STATUS "CMAKE_INSTALL_LIBDIR = ${CMAKE_INSTALL_LIBDIR}")

## Install locations
set(DOC_DIR "share/doc/${PROJECT_NAME}")
set(CMAKE_DIR "share/cmake/${PROJECT_NAME}")
set(INCLUDE_DIR "include/${PROJECT_NAME}")

# Configuration options
# TODO: find boost and python the proper ways
set(BOOST_DIR /usr CACHE PATH "where boost is located")

find_package(PythonInterp) # provides Python version check
find_package(PythonLibs)

if(PYTHON_VERSION_MAJOR LESS 3)
    set(BOOST_PYTHON_LIBNAME boost_python CACHE STRING "what library name for boost python")
else()
    # try to find boost_python3 in a way compatible with most distributions
    find_library(BOOST_PYTHON3_LIB NAMES "boost_python3" "boost_python3${PYTHON_VERSION_MINOR}")
    SET (BOOST_PYTHON_LIBNAME ${BOOST_PYTHON3_LIB} CACHE STRING "what library name for boost python")
endif()

find_package(GTest)
option(ENABLE_LLVM_BACKEND "Whether to build with LLVM backend" TRUE)
option(ENABLE_QT5 "Whether to use Qt5" TRUE)
option(ENABLE_SSE4 "Whether to use SSE4" TRUE)
option(ENABLE_PLUGIN_SYSTEM "Whether to build the plugin system" FALSE)
option(USE_PYTHON "Whether to compile python libraries" TRUE)
option(BUILD_UTILS "Whether to build the utilities" TRUE)
option(BUILD_DEMOS "Whether to build the demos" TRUE)
option(BUILD_DOC "Whether to build the documentation" TRUE)
option(BUILD_TESTS "Whether to build the tests" TRUE)
option(ENABLE_SLOW_TESTS "Whether to enable slow tests" FALSE)

if (IS_DIRECTORY "${CMAKE_SOURCE_DIR}/po")
    find_package(ECM 5.64.0 NO_MODULE)
    if (ECM_FOUND)
        set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_MODULE_PATH})
        option(BUILD_TRANSLATIONS "Whether to include translations" TRUE)
    endif ()
endif ()

set(LLVM_LIB "")
if (ENABLE_LLVM_BACKEND)
    set(LLVM_DIR /usr/share/llvm/cmake CACHE PATH "Where to search for LLVM i.e. ")

    find_package(LLVM CONFIG NAMES LLVM CONFIGS LLVM-Config.cmake LLVMConfig.cmake)
    if (LLVM_FOUND)
        message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
        find_program(LLVM_CONFIG_EXECUTABLE NAMES ${LLVM_TOOLS_BINARY_DIR}/llvm-config)

        # Uncomment to use clang++
        #set(CMAKE_CXX_COMPILER clang++)
        #set(CMAKE_C_COMPILER clang)

        set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${LLVM_DIR}")
        include(LLVM-Config)
        include(LLVMConfig)
        include(HandleLLVMOptions)

        message(STATUS "LLVM_DEFINITIONS =" ${LLVM_DEFINITIONS})
        add_definitions(${LLVM_DEFINITIONS})

        if (NOT LLVM_CONFIG_EXECUTABLE STREQUAL "LLVM_CONFIG_EXECUTABLE-NOTFOUND")
            execute_process(
                COMMAND ${LLVM_CONFIG_EXECUTABLE} --includedir
                OUTPUT_VARIABLE LLVM_INCLUDE_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
        else ()
            set(LLVM_INCLUDE_DIR ${LLVM_INCLUDE_DIRS})
        endif ()
        message(STATUS "LLVM_INCLUDE_DIR =" ${LLVM_INCLUDE_DIR})
        include_directories(${LLVM_INCLUDE_DIR})

        if (NOT LLVM_CONFIG_EXECUTABLE STREQUAL "LLVM_CONFIG_EXECUTABLE-NOTFOUND")
            execute_process(
                COMMAND ${LLVM_CONFIG_EXECUTABLE} --libdir
                OUTPUT_VARIABLE LLVM_LIBRARY_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
        else ()
            set(LLVM_LIBRARY_DIR ${LLVM_LIBRARY_DIRS})
        endif ()
        message(STATUS "LLVM_LIBRARY_DIR =" ${LLVM_LIBRARY_DIR})
        link_directories(${LLVM_LIBRARY_DIR})

        # construct library name
        if (NOT LLVM_CONFIG_EXECUTABLE STREQUAL "LLVM_CONFIG_EXECUTABLE-NOTFOUND")
            execute_process(
                COMMAND ${LLVM_CONFIG_EXECUTABLE} --version
                OUTPUT_VARIABLE LLVM_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
        else ()
            set(LLVM_VERSION ${LLVM_PACKAGE_VERSION})
        endif ()
        message(STATUS "LLVM_VERSION = ${LLVM_VERSION}")

        message(STATUS "Detected processor: ${CMAKE_SYSTEM_PROCESSOR}")
        if(CMAKE_SYSTEM_PROCESSOR MATCHES "amd64.*|x86_64.*|AMD64.*")
            set(SeExpr2_PLATFORM_CODEGEN_LIBS 
                X86CodeGen
                X86AsmParser)
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "i686.*|i386.*|x86.*")
            set(SeExpr2_PLATFORM_CODEGEN_LIBS 
                X86CodeGen
                X86AsmParser)
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64.*|AARCH64.*|arm64.*|ARM64.*)")
            set(SeExpr2_PLATFORM_CODEGEN_LIBS 
                AArch64CodeGen
                AArch64AsmParser)
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm.*|ARM.*)")
            set(SeExpr2_PLATFORM_CODEGEN_LIBS 
                ARMCodeGen
                ARMAsmParser)
        else()
            message(ERROR "Unknown processor, unable to set LLVM codegen targets")
        endif()

        llvm_map_components_to_libnames(LLVM_LIB
            Interpreter
            MCJIT
            ObjCARCOpts
            Passes
            ${SeExpr2_PLATFORM_CODEGEN_LIBS}
        )

        message(STATUS "LLVM_LIB = ${LLVM_LIB}")

        if (LLVM_VERSION VERSION_LESS 3.8.0)
            set(ENABLE_LLVM_BACKEND off)
            message(STATUS "Not building with LLVM, version must be >= 3.8.0")
        else()
            set(SEEXPR_ENABLE_LLVM 1)
        endif()
    endif()
endif()

## Setup platform specific helper defines build variants
if (WIN32)
    add_definitions(-DSEEXPR_WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else()
    add_definitions(-Wall -Wextra -Wno-unused-parameter)
    add_definitions(-pthread)

    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    if (APPLE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-date-time")
    endif()
endif()

# Set to release if nothing else defined
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
      "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif()

if (ENABLE_QT5)
    find_package(Qt5 COMPONENTS Core Gui OpenGL)
    if (Qt5_FOUND)
        message(STATUS "Qt5 Enabled")
        message(STATUS "Qt5Core_INCLUDE_DIRS = ${Qt5Core_INCLUDE_DIRS}")
        message(STATUS "Qt5Gui_INCLUDE_DIRS = ${Qt5Gui_INCLUDE_DIRS}")
        message(STATUS "Qt5OpenGL_INCLUDE_DIRS = ${Qt5OpenGL_INCLUDE_DIRS}")
        include_directories(${Qt5Core_INCLUDE_DIRS})
        include_directories(${Qt5Gui_INCLUDE_DIRS})
        include_directories(${Qt5OpenGL_INCLUDE_DIRS})
    endif()
else()
    find_package(Qt4 4.2.0 COMPONENTS QtCore QtGui QtOpenGL)
    if (Qt4_FOUND)
        message(STATUS "Qt4 Enabled")
        message(STATUS "QT_INCLUDE_DIR = ${QT_INCLUDE_DIR}")
        include_directories(${QT_INCLUDE_DIR}
                            ${QT_INCLUDE_DIR}/QtCore
                            ${QT_INCLUDE_DIR}/QtGui
                            ${QT_INCLUDE_DIR}/QtOpenGL)
    endif()
endif()

if (DEFINED ANIMLIB_DIR)
    message(STATUS "ANIMLIB_DIR = ${ANIMLIB_DIR}")
    add_definitions(-DSEEXPR_USE_ANIMLIB)
    include_directories(${ANIMLIB_DIR}/include)
    link_directories(${ANIMLIB_DIR}/${CMAKE_INSTALL_LIBDIR})
endif()

if (DEFINED DERR_DIR)
    message(STATUS "DERR_DIR = ${DMSG_DIR}")
    include_directories(${DERR_DIR}/include)
    link_directories(${DERR_DIR}/${CMAKE_INSTALL_LIBDIR})
endif()

if (DEFINED DMSG_DIR)
    message(STATUS "DMSG_DIR = ${DMSG_DIR}")
    include_directories(${DMSG_DIR}/include)
    link_directories(${DMSG_DIR}/${CMAKE_INSTALL_LIBDIR})
endif()

if (DEFINED QDGUI_DIR)
    message(STATUS "QDGUI_DIR = ${QDGUI_DIR}")
    add_definitions(-DSEEXPR_USE_QDGUI)
    include_directories(${QDGUI_DIR}/include)
    link_directories(${QDGUI_DIR}/${CMAKE_INSTALL_LIBDIR})
endif()

## Make modules able to see seexpr library
# Setup environment variable to link seexpr
set(SEEXPR_LIBRARIES SeExpr2)
set(SEEXPR_LLVM_LIBRARIES SeExpr2LLVM)
set(SEEXPR_EDITOR_LIBRARIES SeExpr2Editor)

# make it so seexpr can be found
include_directories(BEFORE ${CMAKE_SOURCE_DIR}/src)

## Traverse subdirectories
add_subdirectory(src/SeExpr2)
add_subdirectory(src/SeExpr2/UI)
add_subdirectory(src/py)
if (${BUILD_UTILS})
    add_subdirectory(src/utils)
endif ()
if (${BUILD_DEMOS})
    add_subdirectory(src/demos)
endif ()
if (${BUILD_DOC})
    add_subdirectory(src/doc)
endif ()
if (${BUILD_TESTS})
    add_subdirectory(src/tests)
endif ()

# cmake packaging -- this is done after adding subdirectories so that
# all exported targets are found by export() below.
configure_file("src/build/seexpr2.pc.in" "seexpr2.pc" @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/seexpr2.pc"
    COMPONENT devel DESTINATION share/pkgconfig)

write_basic_package_version_file("seexpr2-version.cmake" VERSION ${${PROJECT_NAME}_VERSION} COMPATIBILITY ExactVersion)

configure_package_config_file(
    "src/build/seexpr2-config.cmake" "seexpr2-config.cmake"
    INSTALL_DESTINATION ${CMAKE_DIR}
    PATH_VARS CMAKE_INSTALL_PREFIX CMAKE_INSTALL_LIBDIR CMAKE_INSTALL_INCLUDEDIR)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/seexpr2-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/seexpr2-version.cmake"
    COMPONENT devel DESTINATION ${CMAKE_DIR})
install(EXPORT ${PROJECT_NAME}Targets
    NAMESPACE SeExpr2::
    DESTINATION ${CMAKE_DIR})
