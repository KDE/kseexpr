// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
#pragma once

namespace KSeExpr {

class Context;

class ContextUtils {
  public:
    static void DisableThreading(Context& context);
    static bool IsThreading(const Context& context);
};

}  // namespace KSeExpr
