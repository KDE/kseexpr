// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
#include "SeContext.h"

SeContext::SeContext(const SeContext* parent) : _parent(parent) {}

void SeContext::setParameter(const std::string& parameterName, const std::string& value) {
    _parameters[parameterName] = value;
}

SeContext* SeContext::createChildContext() const { return new SeContext(this); }

SeContext& SeContext::global() {
    static SeContext context(0);
    return context;
}
