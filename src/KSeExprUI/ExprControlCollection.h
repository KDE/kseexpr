// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * @file ExprControlCollection.h
 * @brief Manages/creates a bunch of ExprControls by using expression text
 * @author  aselle
 */

#pragma once

#include <QVBoxLayout>
#include <QWidget>
#include <vector>


#include "EditableExpression.h"
#include "ExprControl.h"

/// Widget that holds and manages controls for an Expression
///   This is typically used by an ExprEditor or a ExprShortEdit widget
///   This widget also is responsible for finding all user local variables (for use in autocomplete)
class ExprControlCollection : public QWidget
{
    Q_OBJECT

    int _linkedId{};
    int count{};
    bool showAddButton{};

    // holds a representation factored into the controls
    EditableExpression *editableExpression {nullptr};

public:
    ExprControlCollection(QWidget *parent = nullptr, bool showAddButton = true);
    ~ExprControlCollection() override;

private:
    // TODO: put back
    std::vector<ExprControl *> _controls;
    QVBoxLayout *controlLayout;

public:
    /// Request new text, given taking into account control id's new values
    void updateText(int id, QString &text);
    /// Rebuild the controls given the new expressionText. Return any local variables found
    bool rebuildControls(const QString &expressionText, std::vector<QString> &variables);

    /// Number of controls
    int numControls()
    {
        return _controls.size();
    }

    void showEditor(int idx);

private Q_SLOTS:
    /// When a user clicks "Add new variable" button
    void addControlDialog();
    /// Notification when by a control whenever it is edited
    void singleControlChanged(int id);
    /// Notification by a control that a new color link is desired
    void linkColorLink(int id);
    /// Notification by a control that a color is edited (when it is linked)
    void linkColorEdited(int id, QColor color);
Q_SIGNALS:
    /// Notification that a specific control was changed
    void controlChanged(int id);
    /// Gives information about when a link color was changed
    void linkColorOutput(QColor color);
    /// Emitted to request that a new widget string should be added to the expression
    /// i.e. after "Add new variable" was used
    void insertString(const QString &controlString);
public Q_SLOTS:
    /// Notification from outside that a linked color widget was changed
    /// and should be forwarded to any linked controls
    void linkColorInput(QColor color);
};
