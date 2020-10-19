// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <KSeExpr/Expression.h>
#include <KSeExpr/Vec.h>

#include <cstdlib>
#include <cstdio>
#include <cstring>

using namespace KSeExpr;
int main() {
    Expression e("1+2");

    if (!e.isValid()) {
        throw std::runtime_error(std::to_string(e.parseError()));
    }
    e.debugPrintParseTree();
    e.debugPrintLLVM();
    const double* val = e.evalFP();
    std::cout << "val is " << val[0] << std::endl;

    return 0;
}
