# SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
# SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
# SPDX-License-Identifier: LicenseRef-Apache-2.0

cmake_minimum_required(VERSION 3.8.0)

# option() honors normal variables. Needed for LLVM 10.
if (POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif()
# Minimum for SeExpr is 11. LLVM (if used) may need 14.
# LLVM with Clang < 8 crashes on compile: https://github.com/andreasfertig/cppinsights/issues/56#issuecomment-408674466
set(CMAKE_CXX_STANDARD 14)

## project name & version
project(KSeExpr)

# "API" version
set(${PROJECT_NAME}_VERSION "3.4.4.0")

enable_testing()

## Silence installation messages
set(CMAKE_INSTALL_MESSAGE LAZY)

# macros
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(GenerateExportHeader)
LIST (APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(macros)
include(CheckCXXSourceCompiles)
include(FeatureSummary)

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
option(ENABLE_LLVM_BACKEND "Whether to build with LLVM backend" FALSE)
option(ENABLE_QT5 "Whether to use Qt5" TRUE)
option(ENABLE_PLUGIN_SYSTEM "Whether to build the plugin system" FALSE)
option(USE_PYTHON "Whether to compile python libraries" FALSE)
option(BUILD_UTILS "Whether to build the utilities" FALSE)
option(BUILD_DEMOS "Whether to build the demos" FALSE)
option(BUILD_DOC "Whether to build the documentation" FALSE)
option(BUILD_TESTS "Whether to build the tests" FALSE)
option(ENABLE_SLOW_TESTS "Whether to enable slow tests" FALSE)
option(USE_PREGENERATED_FILES "Whether to use the bundled pregenerated parser files" FALSE)

find_package(ECM 5.64.0 NO_MODULE)
if (ECM_FOUND)
    set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_MODULE_PATH})
    if (FLAVOR STREQUAL "debug")
        message(STATUS "ECM sanitizer support is available.")
        include(ECMEnableSanitizers)
    endif()

    if (IS_DIRECTORY "${CMAKE_SOURCE_DIR}/po")
        option(BUILD_TRANSLATIONS "Whether to include translations" TRUE)
    endif ()
endif ()

set(LLVM_LIB "")
if (ENABLE_LLVM_BACKEND)
    set(LLVM_DIR /usr/share/llvm/cmake CACHE PATH "Where to search for LLVM i.e. ")

    find_package(LLVM)
    if (NOT LLVM_FOUND OR LLVM_VERSION VERSION_LESS 3.8.0)
        set(ENABLE_LLVM_BACKEND off)
        message(STATUS "Not building with LLVM, version must be >= 3.8.0")
    else()
        set(SEEXPR_ENABLE_LLVM 1)

        message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
        set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${LLVM_DIR}")
        include(LLVM-Config)
        include(LLVMConfig)
        include(HandleLLVMOptions)

        message(STATUS "LLVM_DEFINITIONS =" ${LLVM_DEFINITIONS})
        message(STATUS "LLVM_INCLUDE_DIRS =" ${LLVM_INCLUDE_DIRS})
        message(STATUS "LLVM_LIBRARY_DIR =" ${LLVM_LIBRARY_DIR})
        message(STATUS "LLVM_PACKAGE_VERSION =" ${LLVM_PACKAGE_VERSION})

        # construct library names

        message(STATUS "Detected processor: ${CMAKE_SYSTEM_PROCESSOR}")
        if(CMAKE_SYSTEM_PROCESSOR MATCHES "amd64.*|x86_64.*|AMD64.*")
            set(KSeExpr_PLATFORM_CODEGEN_LIBS 
                X86CodeGen
                X86AsmParser)
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "i686.*|i386.*|x86.*")
            set(KSeExpr_PLATFORM_CODEGEN_LIBS 
                X86CodeGen
                X86AsmParser)
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64.*|AARCH64.*|arm64.*|ARM64.*)")
            set(KSeExpr_PLATFORM_CODEGEN_LIBS 
                AArch64CodeGen
                AArch64AsmParser)
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm.*|ARM.*)")
            set(KSeExpr_PLATFORM_CODEGEN_LIBS 
                ARMCodeGen
                ARMAsmParser)
        else()
            message(ERROR "Unknown processor, unable to set LLVM codegen targets")
        endif()

        if (LLVM IN_LIST LLVM_AVAILABLE_LIBS)
            message(STATUS "Shared libLLVM is available, using it.")
            set(LLVM_LIB LLVM)
        else()
            message(STATUS "Shared libLLVM is not available, falling back to static linkage.")
            llvm_map_components_to_libnames(LLVM_LIB
                Interpreter
                MCJIT
                ObjCARCOpts
                Passes
                ${KSeExpr_PLATFORM_CODEGEN_LIBS}
            )
        endif()

        message(STATUS "LLVM_LIB = ${LLVM_LIB}")
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
    find_package(Qt5 5.9.0 COMPONENTS Core Gui Widgets REQUIRED)
    message(STATUS "Qt5 Enabled")
endif()

## Make modules able to see seexpr library
# Setup environment variable to link seexpr
set(SEEXPR_LIBRARIES KSeExpr)
set(SEEXPR_LLVM_LIBRARIES KSeExprLLVM)
set(SEEXPR_EDITOR_LIBRARIES KSeExprUI)

# make it so seexpr can be found
include_directories(BEFORE ${CMAKE_SOURCE_DIR}/src)

## Traverse subdirectories
add_subdirectory(src/KSeExpr)
add_subdirectory(src/KSeExprUI)
if (USE_PYTHON)
    add_subdirectory(src/KSeExprPy)
endif()
if (BUILD_UTILS)
    add_subdirectory(src/utils)
endif()
if (BUILD_DEMOS)
    add_subdirectory(src/demos)
endif()
if (BUILD_DOC)
    add_subdirectory(src/doc)
endif()
if (BUILD_TESTS)
    add_subdirectory(src/tests)
endif()

# cmake packaging -- this is done after adding subdirectories so that
# all exported targets are found by export() below.
configure_file("cmake/kseexpr.pc.in" "kseexpr.pc" @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/kseexpr.pc"
    COMPONENT devel DESTINATION share/pkgconfig)

write_basic_package_version_file("kseexpr-config-version.cmake" VERSION ${${PROJECT_NAME}_VERSION} COMPATIBILITY ExactVersion)

configure_package_config_file(
    "cmake/kseexpr-config.cmake" "kseexpr-config.cmake"
    INSTALL_DESTINATION ${CMAKE_DIR}
    PATH_VARS CMAKE_INSTALL_PREFIX CMAKE_INSTALL_LIBDIR CMAKE_INSTALL_INCLUDEDIR)
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/kseexpr-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/kseexpr-config-version.cmake"
    COMPONENT devel DESTINATION ${CMAKE_DIR})
install(EXPORT ${PROJECT_NAME}Targets
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_DIR})

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
