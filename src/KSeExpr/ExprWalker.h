// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <type_traits>

#include "ExprNode.h"

namespace KSeExpr {
template<class T, bool constnode> struct conditional_const {
    using type = typename std::conditional<constnode, typename std::add_const<T>::type, T>::type;
};

template <bool constnode = false>
class Examiner {
  public:
      using T_NODE = typename conditional_const<ExprNode, constnode>::type;

      virtual bool examine(T_NODE *examinee) = 0;
      virtual void post(T_NODE *examinee) {}; // TODO: make this pure virt
      virtual void reset() = 0;
};

template <bool constnode = false>
class Walker {
  public:
    using T_EXAMINER = Examiner<constnode>;
    using T_NODE = typename T_EXAMINER::T_NODE;

    Walker(T_EXAMINER* examiner) : _examiner(examiner) {
        _examiner->reset();
    };

    /// Preorder walk
    void walk(T_NODE* examinee);

  protected:
    void internalWalk(T_NODE* examinee);
    void walkChildren(T_NODE* parent);

  private:
    T_EXAMINER* _examiner;
};

using ConstExaminer = Examiner<true>;
using ConstWalker = Walker<true>;
} // namespace KSeExpr
