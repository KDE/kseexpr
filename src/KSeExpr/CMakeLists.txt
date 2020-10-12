# Copyright Disney Enterprises, Inc.  All rights reserved.
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

# Source files for llvm supported library and interpreter library
file(GLOB io_cpp "*.cpp")
file(GLOB to_remove "ExprLLVMCodeGeneration.cpp")
list(REMOVE_ITEM io_cpp ${to_remove})

set_source_files_properties("ExprBuiltins.cpp" PROPERTIES COMPILE_DEFINITIONS "__STDC_LIMIT_MACROS")

# Uncomment below to print debug messages / performance stats
#add_definitions(-DSEEXPR_DEBUG)
#add_definitions(-DSEEXPR_PERFORMANCE)

# Allow flex/bison to find the current directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

## find our parser generators
find_package(BISON)
find_package(FLEX)
find_program(SED_EXE sed)

if (NOT BISON_FOUND OR
    NOT FLEX_FOUND OR
    (SED_EXE STREQUAL "SED_EXE-NOTFOUND"))
    # don't have flex/bison/sed, use pregenerated versions
    message(STATUS "Using pregenerated parser files")
    file(COPY ${CMAKE_SOURCE_DIR}/windows7/SeExpr/generated/
        DESTINATION .
        USE_SOURCE_PERMISSIONS)
    file(COPY ${CMAKE_SOURCE_DIR}/windows7/SeExpr/UI/generated/
        DESTINATION UI
        USE_SOURCE_PERMISSIONS) 
    set(parser_cpp ExprParser.cpp ExprParserLex.cpp)
    
else()
    ## build the parser from the flex/yacc sources
    add_custom_command(
        SOURCE "ExprParserLex.l"
        COMMAND "flex"
        ARGS "-oExprParserLexIn.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/ExprParserLex.l"
        OUTPUT ExprParserLexIn.cpp
        DEPENDS ExprParserLex.l)
    
    add_custom_command(
        SOURCE "ExprParserLexIn.cpp"
        COMMAND "sed"
        ARGS -e "'s/SeExprwrap(n)/SeExprwrap()/g'" -e "'s/yy/KSeExpr/g'" -e "'s/YY/SeExprYY/g'"  ExprParserLexIn.cpp | tee ExprParserLex.cpp ${CMAKE_CURRENT_SOURCE_DIR}/generated/ExprParserLex.cpp > /dev/null
        OUTPUT ExprParserLex.cpp
        DEPENDS ExprParserLexIn.cpp)
    
    add_custom_command(
        SOURCE "ExprParser.y"
        COMMAND "bison"
        ARGS "--defines" "--verbose" "--fixed-output-files" "-p" "KSeExpr" "${CMAKE_CURRENT_SOURCE_DIR}/ExprParser.y"
        OUTPUT y.tab.c y.tab.h
        DEPENDS ExprParser.y)
    
    add_custom_command(
        SOURCE "y.tab.h"
        COMMAND "sed"
        ARGS -e "'s/yy/KSeExpr/g'" -e "'s/YY/SeExprYY/g'" y.tab.h | tee  ExprParser.tab.h ${CMAKE_CURRENT_SOURCE_DIR}/generated/ExprParser.tab.h > /dev/null
        OUTPUT ExprParser.tab.h
        DEPENDS y.tab.h)
    
    add_custom_command(
        SOURCE "y.tab.c"
        COMMAND "sed"
        ARGS -e "'s/yy/KSeExpr/g'" -e "'s/YY/SeExprYY/g'" y.tab.c | tee ExprParser.cpp  "${CMAKE_CURRENT_SOURCE_DIR}/generated/ExprParser.cpp" > /dev/null
        OUTPUT ExprParser.cpp
        DEPENDS y.tab.c ExprParser.tab.h)

    ## set build files
    set(parser_cpp ExprParser.cpp ExprParserLex.cpp)
endif()

## Test charconv and dynamic dispatch support
check_cxx_source_compiles("
  #include <charconv>
  #include <string>

  int main()
  {
    std::string test{ \"4.5\" };
    double d;
    auto [p, ec] = std::from_chars(test.data(), test.data() + test.size(), d);
    return ec != std::errc();
  }
" KSeExpr_HAVE_CHARCONV_WITH_DOUBLES)

check_cxx_source_compiles("
    [[gnu::target(\"default\")]] int main() { return 4; }
    [[gnu::target(\"sse4.1\")]] int main() { return 5; }
" KSeExpr_HAVE_DYNAMIC_DISPATCH)

if(ENABLE_PLUGIN_SYSTEM)
    set(KSeExpr_ENABLE_PLUGIN_SYSTEM 1)
endif()

## Make the SeExpr library with and without LLVM support
file(GLOB llvm_cpp "*.cpp")
if (NOT WIN32)
    add_library(KSeExpr SHARED ${io_cpp} ${core_cpp} ${parser_cpp} ${llvm_cpp})
    if (NOT APPLE)
        set_source_files_properties(interpreter.cpp PROPERTIES COMPILE_OPTIONS "-rdynamic")
    endif()
    target_link_libraries(KSeExpr "dl" "pthread")
else()
    add_library(KSeExpr STATIC ${io_cpp} ${core_cpp} ${parser_cpp} ${llvm_cpp})
endif()

target_include_directories(KSeExpr INTERFACE
    $<INSTALL_INTERFACE:include>
)

set_property(TARGET KSeExpr PROPERTY VERSION ${KSeExpr_VERSION})
set_property(TARGET KSeExpr PROPERTY SOVERSION ${KSeExpr_MAJOR_VERSION})
set_property(TARGET KSeExpr PROPERTY
             INTERFACE_KSeExpr_MAJOR_VERSION ${KSeExpr_MAJOR_VERSION})
set_property(TARGET KSeExpr APPEND PROPERTY
             COMPATIBLE_INTERFACE_STRING ${KSeExpr_MAJOR_VERSION})

generate_export_header(KSeExpr)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/kseexpr_export.h"
        COMPONENT devel DESTINATION ${INCLUDE_DIR})

## Install binary and includes
install(TARGETS KSeExpr DESTINATION ${CMAKE_INSTALL_LIBDIR}
        EXPORT ${PROJECT_NAME}Targets)

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/"
        COMPONENT devel DESTINATION ${INCLUDE_DIR}
        FILES_MATCHING PATTERN "*.h"
        PATTERN "UI/*" EXCLUDE
        PATTERN "generated/*" EXCLUDE
)

configure_file("ExprConfig.h.in" "ExprConfig.h")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/ExprConfig.h"
        COMPONENT devel DESTINATION ${INCLUDE_DIR})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

if (ENABLE_LLVM_BACKEND)
    if(LLVM_VERSION VERSION_GREATER_EQUAL 10)
        # LLVM >= 10 moved to C++ 14.  
        target_compile_features(${SEEXPR_LIBRARIES} PUBLIC cxx_std_14)
    else()
        target_compile_features(${SEEXPR_LIBRARIES} PUBLIC cxx_std_11)
    endif()
    if (NOT WIN32)
        target_link_libraries(KSeExpr ${LLVM_LIB} "dl" "pthread")
    else ()
        target_link_libraries(KSeExpr ${LLVM_LIB})
    endif ()
endif()
