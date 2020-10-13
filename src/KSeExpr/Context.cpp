// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
#include "Context.h"

using namespace KSeExpr;

Context::Context(const Context* parent) : _parent(parent) {}

void Context::setParameter(const std::string& parameterName, const std::string& value) {
    _parameters[parameterName] = value;
}

Context* Context::createChildContext() const { return new Context(this); }

Context& Context::global() {
    static Context context(0);
    return context;
}
