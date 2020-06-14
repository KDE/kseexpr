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
* @file ExprShortEdit.h
* @brief This provides an expression editor for SeExpr syntax with auto ui features
* @author  aselle
*/

#ifndef ExprShortEditUI_h
#define ExprShortEditUI_h

#include <vector>
#include <string>

#include <QWidget>
#include <QTextEdit>
#include <QWheelEvent>

class ExprControlCollection;
class QToolButton;
class QVBoxLayout;
class QHBoxLayout;
class QTextEdit;
class ExprHighlighter;
class QCompleter;
class ExprCompletionModel;
class ExprShortTextEdit;
class QLabel;
class ExprPopupDoc;
class ExprDialog;

class ExprShortEdit : public QWidget {
    Q_OBJECT

  protected:
    QTimer* controlRebuildTimer;
    QToolButton* editDetail;
    ExprControlCollection* controls;
    ExprDialog* _dialog;
    QVBoxLayout* vboxlayout;
    QHBoxLayout* hboxlayout;
    QLabel* error;
    QString _context;
    QString _searchPath;
    bool _applyOnSelect;

  public:
    ExprShortEdit(QWidget* parent, bool expanded = true, bool applyOnSelect = true);
    virtual ~ExprShortEdit();

    // Gets the string that is in the edit widget
    std::string getExpressionString() const;
    QString getExpression() const;
    // Sets the string that is in the edit widget
    void setExpressionString(const QString& expression);
    // Removes all extra completion symbols
    void clearExtraCompleters();
    // Registers an extra function and associated do QString
    void registerExtraFunction(const QString& name, const QString& docString);
    // Register an extra variable (i.e. $P, or $u, something provided by resolveVar)
    void registerExtraVariable(const QString& name, const QString& docString);
    // Updates the completion widget, must call after registering any new functions/variables
    void updateCompleter();
    // Hides the expression part of the interface
    void setSimple(bool enabled);
    // Set a menu on the "details" button
    void setDetailsMenu(QMenu* menu);
    // Set a colon-delimited path variable for finding expressions
    void setSearchPath(const QString& context, const QString& path);
    // Set line wrap mode on the text edit
    void setLineWrapMode(QTextEdit::LineWrapMode mode);
    // Set the vertical scrollbar policy -- set to Qt::ScrollBarAlwaysOff to
    // disable it
    void setVerticalScrollBarPolicy(Qt::ScrollBarPolicy policy);
    // Set the horizontal scrollbar policy -- set to Qt::ScrollBarAlwaysOff to
    // disable it
    void setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy policy);
    //  Open the details window and open the Nth editor
    //  Pass -1 to not show the editor
    void showDetails(int idx);

    virtual QSize sizeHint() const { return QSize(400, 50); }
    virtual void hideErrors(bool hidden, const std::string& err);

    // Exposed via Python
    QToolButton* expandButton;
    ExprShortTextEdit* edit;

  protected:
    void checkErrors();

  protected
Q_SLOTS:
    virtual void detailPressed();
    virtual void expandPressed();
    virtual void textFinished();
    virtual void handleTextEdited();
    virtual void controlChanged(int id);
    virtual void rebuildControls();
    virtual void expressionApplied();
    virtual void dialogClosed();

Q_SIGNALS:
    void exprChanged();
};

class ExprShortTextEdit : public QTextEdit {
    Q_OBJECT;

    ExprHighlighter* highlighter;

    bool editing;
    QString savedText;
    ExprPopupDoc* _tip;
    QStyle* lastStyleForHighlighter;

  public:
    QCompleter* completer;
    ExprCompletionModel* completionModel;

    ExprShortTextEdit(QWidget* parent);

  protected:
    void showTip(const QString& string);
    void hideTip();

    void paintEvent(QPaintEvent* e);
    virtual void keyPressEvent(QKeyEvent* e);
    virtual void focusInEvent(QFocusEvent* e);
    virtual void focusOutEvent(QFocusEvent* e);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseDoubleClickEvent(QMouseEvent* event);
    virtual void wheelEvent(QWheelEvent* e) { e->ignore(); }

    void setColor(bool editing);
    void finishEdit();
Q_SIGNALS:
    void editingFinished();
  private
Q_SLOTS:
    void insertCompletion(const QString& completion);
};

#endif
