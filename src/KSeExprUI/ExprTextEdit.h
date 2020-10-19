// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later
/*
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
    void removeWord();
  Q_SIGNALS:
    void applyShortcut();
    void nextError();
};
