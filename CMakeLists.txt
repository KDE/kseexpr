# SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
# SPDX-License-Identifier: LicenseRef-Apache-2.0
# SPDX-FileCopyrightText: 2020-2021 L. E. Segovia <amy@amyspark.me>
# SPDX-License-Identifier: GPL-3.0-or-later

cmake_minimum_required(VERSION 3.8.0)

# option() honors normal variables. Needed for LLVM 10.
if (POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif()
# Minimum for SeExpr is 11. LLVM (if used) may need 14.
# LLVM with Clang < 8 crashes on compile: https://github.com/andreasfertig/cppinsights/issues/56#issuecomment-408674466
set(CMAKE_CXX_STANDARD 14)

## project name & version
project(KSeExpr
    VERSION "6.0.0.0"
    DESCRIPTION "An embeddable expression evaluation engine for Krita"
)

message(STATUS "Welcome to ${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_VERSION} !")

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

# Configuration options
option(ENABLE_LLVM_BACKEND "Whether to build with LLVM backend" FALSE)
add_feature_info(LLVMBackend ENABLE_LLVM_BACKEND "Build the LLVM backend of KSeExpr")
option(ENABLE_QT6 "Whether to use Qt6" TRUE)
add_feature_info(KSeExprUI ENABLE_QT6 "Build the KSeExprUI widget library")
option(BUILD_UTILS "Whether to build the utilities" FALSE)
add_feature_info(Utils BUILD_UTILS "Build the utilities")
option(BUILD_DEMOS "Whether to build the demos" FALSE)
add_feature_info(Demos BUILD_DEMOS "Build the demos")
option(BUILD_DOC "Whether to build the documentation" FALSE)
add_feature_info(Docs BUILD_DOC "Build the documentation")
option(BUILD_TESTS "Whether to build the tests" FALSE)
add_feature_info(Tests BUILD_TESTS "Build the tests")
option(ENABLE_SLOW_TESTS "Whether to enable slow tests" FALSE)
add_feature_info(SlowTests ENABLE_SLOW_TESTS "Enables slow tests")
option(USE_PREGENERATED_FILES "Whether to use the bundled pregenerated parser files" FALSE)
add_feature_info(PregeneratedFiles USE_PREGENERATED_FILES "Build the language parser using the bundled, pregenerated files")
option(ENABLE_PERFORMANCE_STATS "Whether to print performance stats across KSeExpr" FALSE)
add_feature_info(PerformanceStats ENABLE_PERFORMANCE_STATS "Print performance stats across KSeExpr")

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(USE_PREGENERATED_FILES TRUE)
endif()

# Package lookup

find_package(Doxygen)
set_package_properties(Doxygen PROPERTIES
                       TYPE OPTIONAL
                       PURPOSE "Needed to build the documentation")
find_package(ECM 6.2.0 NO_MODULE)
set_package_properties(ECM PROPERTIES
                       TYPE OPTIONAL
                       PURPOSE "Enables translations (if bundled) and automatic sanitizer support")
find_package(KF6I18n 6.2.0)
set_package_properties(KF5I18n PROPERTIES
                       TYPE OPTIONAL
                       PURPOSE "Enables detection of fallback languages in KDE apps")
find_package(GTest)
set_package_properties(GTest PROPERTIES
                       TYPE OPTIONAL
                       PURPOSE "Needed for building the tests")
find_package(PNG)
set_package_properties(PNG PROPERTIES
                       TYPE OPTIONAL
                       PURPOSE "Needed to support image file output in demos and tests")
find_package(Python3)
set_package_properties(Python3 PROPERTIES
                       TYPE OPTIONAL
                       PURPOSE "Needed for running some tests")
find_package(BISON)
set_package_properties(BISON PROPERTIES
                       TYPE OPTIONAL
                       PURPOSE "Needed to build KSeExpr's language parser from source")
find_package(FLEX)
set_package_properties(FLEX PROPERTIES
                       TYPE OPTIONAL
                       PURPOSE "Needed to build KSeExpr's language parser from source")
find_program(SED_EXE sed)
set_package_properties(SED_EXE PROPERTIES
                       TYPE OPTIONAL
                       PURPOSE "Needed to build KSeExpr's language parser from source")

if (ECM_FOUND)
    set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_MODULE_PATH})
    if (FLAVOR STREQUAL "debug")
        message(STATUS "ECM sanitizer support is available.")
        include(ECMEnableSanitizers)
    endif()

    if (IS_DIRECTORY "${CMAKE_SOURCE_DIR}/poqm")
        option(BUILD_TRANSLATIONS "Whether to include translations" TRUE)
        add_feature_info(i18n BUILD_TRANSLATIONS "Build and embed translations")
    endif ()
endif ()

if (ENABLE_LLVM_BACKEND)
    find_package(LLVM)
    set_package_properties(LLVM PROPERTIES
                        TYPE OPTIONAL
                        PURPOSE "Needed for building the LLVM-based script backend")
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
    add_compile_definitions(_USE_MATH_DEFINES)
else()
    add_definitions(-Wall -Wextra -Wno-unused-parameter)

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

if (ENABLE_QT6)
    find_package(Qt6 6.6.0 COMPONENTS Core Gui Widgets REQUIRED)
    set_package_properties(Qt5 PROPERTIES
                            TYPE REQUIRED
                            PURPOSE "Required if you're building KSeExprUI")
    message(STATUS "Qt6 Enabled")
endif()

## Make modules able to see seexpr library
# Setup environment variable to link seexpr
set(SEEXPR_LIBRARIES KSeExpr)
set(SEEXPR_LLVM_LIBRARIES KSeExprLLVM)
set(SEEXPR_EDITOR_LIBRARIES KSeExprUI)

# make it so seexpr can be found
include_directories(BEFORE ${CMAKE_SOURCE_DIR}/src)

if (ENABLE_PERFORMANCE_STATS)
    add_definitions(-DSEEXPR_PERFORMANCE)
endif()

## Traverse subdirectories
add_subdirectory(src/KSeExpr)
if (ENABLE_QT6)
    add_subdirectory(src/KSeExprUI)
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

write_basic_package_version_file("kseexpr-config-version.cmake" VERSION ${${PROJECT_NAME}_VERSION} COMPATIBILITY SameMajorVersion)

configure_package_config_file(
    "cmake/kseexpr-config.cmake" "kseexpr-config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    PATH_VARS CMAKE_INSTALL_PREFIX CMAKE_INSTALL_LIBDIR CMAKE_INSTALL_INCLUDEDIR)
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/kseexpr-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/kseexpr-config-version.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
install(EXPORT ${PROJECT_NAME}Targets
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
