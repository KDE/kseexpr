// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <stdio.h>
#include <gtest/gtest.h>
#include <KSeExpr/ExprFunc.h>

using ::testing::InitGoogleTest;

int main(int argc, char **argv) {
    int result = 0;

    {
        InitGoogleTest(&argc, argv);
        result = RUN_ALL_TESTS();
        KSeExpr::ExprFunc::cleanup();
    }
    return result;
}
