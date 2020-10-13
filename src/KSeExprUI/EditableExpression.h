// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0

/*
* @file EditableExpression.h
* @author Andrew Selle
*/
#ifndef __EditableExpression__
#define __EditableExpression__

#include <vector>
#include <string>

class Editable;

/// Factors a SeExpr into an editable expression with controls (i.e. value boxes, curve boxes)
class EditableExpression {
    std::string _expr;  // original full expression
    typedef std::vector<Editable*> Editables;
    std::vector<Editable*> _editables;  // control that can edit the expression
    std::vector<std::string> _variables;

  public:
    EditableExpression();
    ~EditableExpression();

    /// Set's expressions and parses it into "control editable form"
    void setExpr(const std::string& expr);

    /// Return a reconstructed expression using all the editable's current values
    std::string getEditedExpr() const;

    /// Check if the other editable expression has editables that all match i.e. the controls are same
    bool controlsMatch(const EditableExpression& other) const;

    /// Update the string refered to into the controls (this is only valid if controlsmatch)
    void updateString(const EditableExpression& other);

    /// Access an editable parameter
    Editable* operator[](const int i) { return _editables[i]; }

    /// Return the count of editable parameters
    size_t size() const { return _editables.size(); }

    /// Get list of commentsø
    const std::vector<std::string>& getVariables() const { return _variables; }

  private:
    /// clean memeory
    void cleanup();
};

#endif
