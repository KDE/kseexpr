# SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
# SPDX-License-Identifier: LicenseRef-Apache-2.0
# SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
# SPDX-License-Identifier: GPL-3.0-or-later

# Source files for llvm supported library and interpreter library
set(KSeExpr_HEADERS
        Context.h
        Curve.h
        ErrorCode.h
        Evaluator.h
        ExprBuiltins.h
        ExprEnv.h
        Expression.h
        ExprFunc.h
        ExprFuncStandard.h
        ExprFuncX.h
        ExprLLVM.h
        ExprLLVMAll.h
        ExprMultiExpr.h
        ExprNode.h
        ExprParser.h
        ExprPatterns.h
        ExprType.h
        ExprWalker.h
        Interpreter.h
        Noise.h
        NoiseTables.h
        PerformanceTimer.h
        StringUtils.h
        Timer.h
        TypePrinter.h
        Utils.h
        VarBlock.h
        Vec.h
    )

set(KSeExpr_SOURCES
        Context.cpp
        Curve.cpp
        ExprBuiltins.cpp
        ExprEnv.cpp
        Expression.cpp
        ExprFunc.cpp
        ExprFuncStandard.cpp
        ExprFuncX.cpp
        ExprLLVMCodeGeneration.cpp
        ExprMultiExpr.cpp
        ExprNode.cpp
        ExprWalker.cpp
        Interpreter.cpp
        Noise.cpp
        Utils.cpp
    )

set_source_files_properties("ExprBuiltins.cpp" PROPERTIES COMPILE_DEFINITIONS "__STDC_LIMIT_MACROS")


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

BuildParserScanner(KSeExpr ExprParserLex ExprParser SeExpr parser_cpp)

if (NOT WIN32)
    add_library(KSeExpr SHARED ${KSeExpr_SOURCES} ${parser_cpp})
    if (NOT APPLE)
        set_source_files_properties(interpreter.cpp PROPERTIES COMPILE_OPTIONS "-rdynamic")
    endif()
    target_link_libraries(KSeExpr "dl")
else()
    add_library(KSeExpr STATIC ${KSeExpr_SOURCES} ${parser_cpp})
endif()

target_include_directories(KSeExpr
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    INTERFACE $<INSTALL_INTERFACE:include>
)

set_property(TARGET KSeExpr PROPERTY VERSION ${KSeExpr_VERSION})
set_property(TARGET KSeExpr PROPERTY SOVERSION ${KSeExpr_VERSION_MAJOR})
set_property(TARGET KSeExpr PROPERTY
             INTERFACE_KSeExpr_MAJOR_VERSION ${KSeExpr_VERSION_MAJOR})
set_property(TARGET KSeExpr APPEND PROPERTY
             COMPATIBLE_INTERFACE_STRING ${KSeExpr_VERSION_MAJOR})

generate_export_header(KSeExpr)

configure_file("ExprConfig.h.in" "ExprConfig.h")

## Install binary and includes
install(TARGETS KSeExpr
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
    EXPORT ${PROJECT_NAME}Targets
)

install(FILES
    ${KSeExpr_HEADERS}
    ${KSeExpr_LLVM_HEADERS}
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/kseexpr_export.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/ExprConfig.h>
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/KSeExpr"
)

if (ENABLE_LLVM_BACKEND)
    add_definitions(${LLVM_DEFINITIONS})

    target_include_directories(${SEEXPR_LIBRARIES} PUBLIC ${LLVM_INCLUDE_DIRS})

    if(LLVM_VERSION VERSION_GREATER_EQUAL 10)
        # LLVM >= 10 moved to C++ 14.
        target_compile_features(${SEEXPR_LIBRARIES} PUBLIC cxx_std_14)
    else()
        target_compile_features(${SEEXPR_LIBRARIES} PUBLIC cxx_std_11)
    endif()

    target_link_libraries(KSeExpr ${LLVM_LIB})
endif()
