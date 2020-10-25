// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

/*
 * @file EditableExpression.h
 * @author Andrew Selle
 */
#pragma once

#include <memory>
#include <string>
#include <vector>

class Editable;

/// Factors a SeExpr into an editable expression with controls (i.e. value boxes, curve boxes)
class EditableExpression
{
    std::string _expr; // original full expression
    using Editables = std::vector<Editable *>; // TODO: use shared_ptr -amyspark
    Editables _editables; // control that can edit the expression
    std::vector<std::string> _variables;

public:
    EditableExpression() = default;
    virtual ~EditableExpression();

    EditableExpression& operator=(const EditableExpression&) = default;
    EditableExpression &operator=(EditableExpression &&) = default;
    EditableExpression(const EditableExpression &) = default;
    EditableExpression(EditableExpression &&) = default;

    /// Set's expressions and parses it into "control editable form"
    void setExpr(const std::string &expr);

    /// Return a reconstructed expression using all the editable's current values
    std::string getEditedExpr() const;

    /// Check if the other editable expression has editables that all match i.e. the controls are same
    bool controlsMatch(const EditableExpression &other) const;

    /// Update the string refered to into the controls (this is only valid if controlsmatch)
    void updateString(const EditableExpression &other);

    /// Access an editable parameter
    Editable *operator[](const int i)
    {
        return _editables[i];
    }

    /// Return the count of editable parameters
    size_t size() const
    {
        return _editables.size();
    }

    /// Get list of commentsø
    const std::vector<std::string> &getVariables() const
    {
        return _variables;
    }

private:
    /// clean memeory
    void cleanup();
};
