# SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
# SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
# SPDX-License-Identifier: LicenseRef-Apache-2.0

# Source files for llvm supported library and interpreter library
file(GLOB io_cpp "*.cpp")
file(GLOB to_remove "ExprLLVMCodeGeneration.cpp")
list(REMOVE_ITEM io_cpp ${to_remove})

set_source_files_properties("ExprBuiltins.cpp" PROPERTIES COMPILE_DEFINITIONS "__STDC_LIMIT_MACROS")

# Uncomment below to print debug messages / performance stats
#add_definitions(-DSEEXPR_DEBUG)
#add_definitions(-DSEEXPR_PERFORMANCE)


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

BuildParserScanner(KSeExpr ExprParserLex ExprParser SeExpr parser_cpp)

file(GLOB llvm_cpp "*.cpp")
if (NOT WIN32)
    add_library(KSeExpr SHARED ${io_cpp} ${core_cpp} ${parser_cpp} ${llvm_cpp})
    if (NOT APPLE)
        set_source_files_properties(interpreter.cpp PROPERTIES COMPILE_OPTIONS "-rdynamic")
    endif()
    target_link_libraries(KSeExpr "dl")
else()
    add_library(KSeExpr STATIC ${io_cpp} ${core_cpp} ${parser_cpp} ${llvm_cpp})
endif()

target_include_directories(KSeExpr 
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    INTERFACE $<INSTALL_INTERFACE:include>
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
        PATTERN "generated/*" EXCLUDE
)

configure_file("ExprConfig.h.in" "ExprConfig.h")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/ExprConfig.h"
        COMPONENT devel DESTINATION ${INCLUDE_DIR})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

if (ENABLE_LLVM_BACKEND)
    add_definitions(${LLVM_DEFINITIONS})

    target_include_directories(${SEEXPR_LIBRARIES} PUBLIC ${LLVM_INCLUDE_DIRS})
    target_link_directories(${SEEXPR_LIBRARIES} PUBLIC ${LLVM_LIBRARY_DIR})

    if(LLVM_VERSION VERSION_GREATER_EQUAL 10)
        # LLVM >= 10 moved to C++ 14.  
        target_compile_features(${SEEXPR_LIBRARIES} PUBLIC cxx_std_14)
    else()
        target_compile_features(${SEEXPR_LIBRARIES} PUBLIC cxx_std_11)
    endif()

    target_link_libraries(KSeExpr ${LLVM_LIB})
endif()
