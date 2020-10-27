// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <cstring>
#include <vector>

#include "ExprNode.h"
#include "ExprPatterns.h"
#include "ExprWalker.h"

namespace KSeExpr {

template <bool constnode>
void Walker<constnode>::walk(T_NODE* examinee) {
    _examiner->reset();
    internalWalk(examinee);
}

template <bool constnode>
void Walker<constnode>::internalWalk(T_NODE* examinee) {
    /// If examine returns false, do not recurse
    if (_examiner->examine(examinee)) walkChildren(examinee);
    _examiner->post(examinee);
}

template <bool constnode>
void Walker<constnode>::walkChildren(T_NODE* parent) {
    for (int i = 0; i < parent->numChildren(); i++) internalWalk(parent->child(i));
}

template class Walker<false>;
template class Walker<true>;
} // namespace KSeExpr
