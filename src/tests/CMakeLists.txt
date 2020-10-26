# SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
# SPDX-License-Identifier: LicenseRef-Apache-2.0
# SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
# SPDX-License-Identifier: GPL-3.0-or-later


if (GTEST_FOUND AND PNG_FOUND AND Python3_FOUND)
    message("-- Found PNG library: " ${PNG_LIBRARIES})
    message("-- Found Python executable: " ${Python3_EXECUTABLE})

    set(TEST_DEST "share/test/KSeExpr")

    # generate test cases for example developer expressions
    set (GENERATE_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/genImageTestFile.py")
    set (EXAMPLE_DIR ${CMAKE_INSTALL_PREFIX}/share/KSeExpr/demos/imageSynth/examples)
    if (EXISTS ${EXAMPLE_DIR})
        set (EXAMPLE_TESTS "testSeExprExamples.cpp")
        add_custom_command(
            SOURCE ${EXAMPLE_DIR}
            COMMAND ${GENERATE_SCRIPT}
            ARGS "${EXAMPLE_DIR}" "${EXAMPLE_TESTS}"
            OUTPUT ${EXAMPLE_TESTS}
            DEPENDS ${GENERATE_SCRIPT} ${EXAMPLE_DIR})
    endif()

    add_executable(testmain2
        "testmain.cpp" "imageTests.cpp"
        ${EXAMPLE_TESTS} ${PAINT3D_TESTS}
        "basic.cpp" "string.cpp")
    target_include_directories(testmain2 PRIVATE ${PNG_INCLUDE_DIR})
    target_include_directories(testmain2 PRIVATE ${GTEST_INCLUDE_DIR})
    target_link_libraries(testmain2 KSeExpr GTest::GTest ${PNG_LIBRARIES})
    install(TARGETS testmain2 DESTINATION ${TEST_DEST})
    add_test(NAME basic COMMAND testmain2)

    add_executable(dirtSimple "dirtSimple.cpp")
    target_include_directories(dirtSimple PRIVATE ${PNG_INCLUDE_DIR})
    target_link_libraries(dirtSimple KSeExpr ${PNG_LIBRARIES})
    install(TARGETS dirtSimple DESTINATION ${TEST_DEST})
    add_test(NAME dirtSimple COMMAND dirtSimple)

    if (ENABLE_LLVM_BACKEND)
        add_executable(BlockTests "BlockTests.cpp")
        target_include_directories(BlockTests PRIVATE ${PNG_INCLUDE_DIR})
        target_link_libraries(BlockTests KSeExpr ${PNG_LIBRARIES})
        install(TARGETS BlockTests DESTINATION ${TEST_DEST})
        add_test(NAME BlockTests COMMAND BlockTests)
    endif()

    add_executable(VarBlockExample VarBlockExample.cpp)
    target_link_libraries(VarBlockExample KSeExpr)
    install(TARGETS VarBlockExample DESTINATION ${TEST_DEST})

    if (ENABLE_SLOW_TESTS)
        add_test(NAME VarBlockExample COMMAND VarBlockExample)
    endif()
endif()
