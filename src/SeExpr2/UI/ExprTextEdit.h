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
* @file ExprTextEditor.h
* @brief This provides an expression editor for SeExpr syntax with auto ui features
* @author  aselle
*/

#include <vector>
#include <QCompleter>
#include <QTextEdit>
#include <QToolTip>

#include "ExprCompletionModel.h"
#include "ExprHighlighter.h"
#include "ExprPopupDoc.h"

class ExprTextEdit : public QTextEdit {
    Q_OBJECT

    QToolTip* functionTip;
    std::map<std::string, std::string> functionTooltips;
    ExprHighlighter* highlighter;
    QStyle* lastStyleForHighlighter;
    ExprPopupDoc* _tip;
    QAction* _popupEnabledAction;

  public:
    QCompleter* completer;
    ExprCompletionModel* completionModel;

  public:
    ExprTextEdit(QWidget* parent = 0);
    ~ExprTextEdit();
    void updateStyle();

  protected:
    void showTip(const QString& string);
    void hideTip();

    virtual void keyPressEvent(QKeyEvent* e);
    void focusInEvent(QFocusEvent* e);
    void focusOutEvent(QFocusEvent* e);
    void mousePressEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
    void paintEvent(QPaintEvent* e);
    void wheelEvent(QWheelEvent* e);
    void contextMenuEvent(QContextMenuEvent* event);

  private Q_SLOTS:
    void insertCompletion(const QString& completion);
  Q_SIGNALS:
    void applyShortcut();
    void nextError();
};
