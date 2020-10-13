// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
#pragma once

#include <map>
#include <string>

namespace KSeExpr {

class Context {
  public:
    /// Lookup a Context parameter by name.
    bool lookupParameter(const std::string& parameterName, std::string& value) const {
        ParameterMap::const_iterator it = _parameters.find(parameterName);
        if (it != _parameters.end()) {
            value = it->second;
            return true;
        } else if (_parent)
            return _parent->lookupParameter(parameterName, value);
        else
            return false;
    }
    /// Set a parameter. NOTE: this must be done when no threads are accessing lookupParameter for safety
    void setParameter(const std::string& parameterName, const std::string& value);
    /// Create a context that is a child of this context
    Context* createChildContext() const;

    // Parent access uses pointers as it is acceptable to set/get a NULL parent
    void setParent(const Context* context) { _parent = context; }
    const Context* getParent() const { return _parent; }

    bool hasContext(const Context* context) const {
        if (this == context) return true;
        if (_parent) return _parent->hasContext(context);
        return false;
    }

    /// The global default context of the seexpr
    static Context& global();

  private:
    /// Private constructor and un-implemented default/copy/assignment
    /// (it is required that we derive from the global context via createChildContext)
    Context(const Context&);
    Context& operator=(const Context&);

    Context(const Context* parent);
    /// The parent scope
    const Context* _parent;

    // TODO: Use std::map until C++11 is ubiq.
    typedef std::map<std::string, std::string> ParameterMap;
    /// Attribute/value pairs
    ParameterMap _parameters;
};
}
