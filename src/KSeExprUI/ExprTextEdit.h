// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * @file ExprTextEditor.h
 * @brief This provides an expression editor for SeExpr syntax with auto ui features
 * @author  aselle
 */

#pragma once

#include <QCompleter>
#include <QTextEdit>
#include <QToolTip>
#include <cstddef>
#include <vector>

#include "ExprCompletionModel.h"
#include "ExprHighlighter.h"
#include "ExprPopupDoc.h"

class ExprTextEdit : public QTextEdit
{
    Q_OBJECT

    QToolTip *functionTip {nullptr};
    std::map<std::string, std::string> functionTooltips;
    ExprHighlighter *highlighter {nullptr};
    QStyle *lastStyleForHighlighter {nullptr};
    QAction *_popupEnabledAction {nullptr};

public:
    QCompleter *completer {nullptr};
    ExprCompletionModel *completionModel {nullptr};
    ExprTextEdit(QWidget *parent = nullptr);
    void updateStyle();

protected:
    void showTip(const QString &string);
    static void hideTip();
    void keyPressEvent(QKeyEvent *e) override;
    void focusInEvent(QFocusEvent *e) override;
    void focusOutEvent(QFocusEvent *e) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *e) override;
    void wheelEvent(QWheelEvent *e) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

private Q_SLOTS:
    void insertCompletion(const QString &completion);
    void removeWord();
Q_SIGNALS:
    void applyShortcut();
    void nextError();
};
