# SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
# SPDX-License-Identifier: LicenseRef-Apache-2.0

add_executable(asciiGraph2 "asciiGraph.cpp")
target_link_libraries(asciiGraph2 KSeExpr)
install(TARGETS asciiGraph2 DESTINATION ${CMAKE_INSTALL_BINDIR})

add_executable(asciiCalc2 "asciiCalculator.cpp")
target_link_libraries(asciiCalc2 KSeExpr)
install(TARGETS asciiCalc2 DESTINATION ${CMAKE_INSTALL_BINDIR})

add_subdirectory(imageSynth)
add_subdirectory(imageEditor)

#if (ENABLE_LLVM_BACKEND)
#     add_executable(llvmtest "llvmtest.cpp")
#     target_link_libraries(llvmtest ${SEEXPR_LIBRARIES})
#     install(TARGETS llvmtest DESTINATION ${CMAKE_INSTALL_BINDIR})
#endif()
