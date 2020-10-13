/*
* Copyright Disney Enterprises, Inc.  All rights reserved.
* Copyright (C) 2020 L. E. Segovia <amy@amyspark.me>
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License
* and the following modification to it: Section 6 Trademarks.
* deleted and replaced with:
*
* 6. Trademarks. This License does not grant permission to use the
* trade names, trademarks, service marks, or product names of the
* Licensor and its affiliates, except as required for reproducing
* the content of the NOTICE file.
*
* You may obtain a copy of the License at
* http://www.apache.org/licenses/LICENSE-2.0
*
* @file ExprControlCollection.h
* @brief Manages/creates a bunch of ExprControls by using expression text
* @author  aselle
*/
#ifndef _ExprControlCollection_h
#define _ExprControlCollection_h

#include <vector>
#include <QVBoxLayout>
#include <QWidget>

#include "EditableExpression.h"
#include "ExprControl.h"

/// Widget that holds and manages controls for an Expression
///   This is typically used by an ExprEditor or a ExprShortEdit widget
///   This widget also is responsible for finding all user local variables (for use in autocomplete)
class ExprControlCollection : public QWidget {
    Q_OBJECT;

    int _linkedId;
    int count;
    bool showAddButton;

    // holds a representation factored into the controls
    EditableExpression *editableExpression;

  public:
    ExprControlCollection(QWidget *parent = 0, bool showAddButton = true);
    ~ExprControlCollection();

  private:
    // TODO: put back
    std::vector<ExprControl *> _controls;
    QVBoxLayout *controlLayout;

  public:
    /// Request new text, given taking into account control id's new values
    void updateText(const int id, QString &text);
    /// Rebuild the controls given the new expressionText. Return any local variables found
    bool rebuildControls(const QString &expressionText, std::vector<QString> &variables);

    /// Number of controls
    int numControls() { return _controls.size(); }

    void showEditor(int idx);

  private
Q_SLOTS:
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
  public
Q_SLOTS:
    /// Notification from outside that a linked color widget was changed
    /// and should be forwarded to any linked controls
    void linkColorInput(QColor color);
};

#endif
