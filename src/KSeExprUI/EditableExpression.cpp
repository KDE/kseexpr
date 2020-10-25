// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "EditableExpression.h"
#include "Editable.h"
#include <sstream>

bool ExprSpecParse(std::vector<Editable *> &literals, std::vector<std::string> &variables, std::vector<std::pair<int, int>> &comments, const char *str);

EditableExpression::~EditableExpression()
{
    cleanup();
}

void EditableExpression::setExpr(const std::string &expr)
{
    // get rid of old data
    cleanup();

    // run parser
    _expr = expr;
    std::vector<std::pair<int, int>> comments;
    ExprSpecParse(_editables, _variables, comments, _expr.c_str());

    for (auto it = _editables.begin(); it != _editables.end();) {
        Editable &literal = **it;
        int endPos = literal.endPos;
        std::string comment;
        for (auto &ci : comments) {
            if (ci.first >= endPos) {
                // check to make sure there is no newlines between end of editable and comment
                size_t pos = _expr.find('\n', endPos);
                if (pos == std::string::npos || pos >= (size_t)ci.second) {
                    comment = _expr.substr(ci.first, ci.second - ci.first);
                    break;
                }
            }
        }
        bool keepEditable = literal.parseComment(comment);
        if (!keepEditable) { // TODO: this is potentially quadratic if we remove a bunch
            delete &literal;
            it = _editables.erase(it);
        } else {
            ++it;
        }
    }
}

void EditableExpression::cleanup()
{
    for (auto &_editable : _editables)
        delete _editable;
    _editables.clear();
    _variables.clear();
}

std::string EditableExpression::getEditedExpr() const
{
    int offset = 0;
    std::stringstream stream;
    for (auto *_editable : _editables) {
        Editable &literal = *_editable;
        stream << _expr.substr(offset, literal.startPos - offset);
        literal.appendString(stream);
        offset = literal.endPos;
    }
    stream << _expr.substr(offset, _expr.size() - offset);
    return stream.str();
}

void EditableExpression::updateString(const EditableExpression &other)
{
    // TODO: move semantics?
    _variables = other._variables;
    _expr = other._expr;
    _variables = other._variables;
    for (size_t i = 0, sz = _editables.size(); i < sz; i++) {
        Editable &literal = *_editables[i];
        Editable &otherLiteral = *other._editables[i];
        assert(literal.controlsMatch(otherLiteral));
        literal.updatePositions(otherLiteral);
    }
}

bool EditableExpression::controlsMatch(const EditableExpression &other) const
{
    if (_editables.size() != other._editables.size())
        return false;

    for (size_t i = 0, sz = _editables.size(); i < sz; i++) {
        const Editable &literal = *_editables[i];
        const Editable &otherLiteral = *other._editables[i];
        if (!literal.controlsMatch(otherLiteral))
            return false;
    }
    return true;
}
