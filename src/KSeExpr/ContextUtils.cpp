// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0

#include "ContextUtils.h"
#include "Context.h"

namespace KSeExpr {

namespace /* anonymous */ {
const std::string disableThreading = "disableThreading";
}  // namespace anonymous

void ContextUtils::DisableThreading(Context& context) { context.setParameter(disableThreading, "true"); }

bool ContextUtils::IsThreading(const Context& context) {
    bool isThreading = true;

    std::string result;
    if (context.lookupParameter(disableThreading, result) && result == "true") {
        isThreading = false;
    }

    return isThreading;
}

}  // namespace KSeExpr
