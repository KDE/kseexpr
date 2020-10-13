// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
#ifndef _SeTests_h_
#define _SeTests_h_

#include <iostream>

#define SE_TEST_ASSERT(x) \
    if (!(x)) std::cerr << "Test " << #x << " failed at " << __FILE__ << ":" << __LINE__ << std::endl;

#define SE_TEST_ASSERT_EQUAL(actual, expected)                                                                        \
    if ((actual) != (expected))                                                                                       \
        std::cerr << "Test failed at " << __FILE__ << ":" << __LINE__ << " value expected '" << (expected) << " got " \
                  << "'" << (actual) << "'" << std::endl;

#define SE_TEST_ASSERT_VECTOR_EQUAL(actual, expected)                                                             \
    if ((actual)[0] != (expected)[0])                                                                             \
        std::cerr << "Test failed at " << __FILE__ << ":" << __LINE__ << " first value expected '" << (expected)  \
                  << " got "                                                                                      \
                  << "'" << (actual) << "'" << std::endl;                                                         \
    if ((actual)[1] != (expected)[1])                                                                             \
        std::cerr << "Test failed at " << __FILE__ << ":" << __LINE__ << " second value expected '" << (expected) \
                  << " got "                                                                                      \
                  << "'" << (actual) << "'" << std::endl;                                                         \
    if ((actual)[2] != (expected)[2])                                                                             \
        std::cerr << "Test failed at " << __FILE__ << ":" << __LINE__ << " third value expected '" << (expected)  \
                  << " got "                                                                                      \
                  << "'" << (actual) << "'" << std::endl;

#endif
