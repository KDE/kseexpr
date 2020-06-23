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
* @file ExprEditor.cpp
* @brief This provides an expression editor for SeExpr syntax with auto ui features
* @author  aselle
*/

#include <QVBoxLayout>

#include "../Expression.h"
#include "../ExprNode.h"
#include "../ExprFunc.h"
#include "../ExprBuiltins.h"

#include "ExprEditor.h"
#include "ExprHighlighter.h"
#include "ExprCompletionModel.h"
#include "ExprControlCollection.h"
#include "ExprCurve.h"
#include "ExprColorCurve.h"
#include "ExprControl.h"
#include "ExprPopupDoc.h"

ExprLineEdit::ExprLineEdit(int id, QWidget* parent) : QLineEdit(parent), _id(id), _signaling(0) {
    connect(this, SIGNAL(textChanged(const QString&)), SLOT(textChangedCB(const QString&)));
}

void ExprLineEdit::textChangedCB(const QString& text) {
    _signaling = 1;
    emit textChanged(_id, text);
    _signaling = 0;
}

void ExprEditor::controlChanged(int id) {
    QString newText = exprTe->toPlainText();
    controls->updateText(id, newText);
    _updatingText = 1;
    exprTe->selectAll();
    exprTe->insertPlainText(newText);
    // exprTe->setPlainText(newText);
    _updatingText = 0;

    // schedule preview update
    previewTimer->setSingleShot(true);
    previewTimer->start(0);
}

ExprEditor::~ExprEditor() {
    delete controlRebuildTimer;
    delete previewTimer;
}

ExprEditor::ExprEditor(QWidget* parent, ExprControlCollection* controls)
    : QWidget(parent), _updatingText(0), errorHeight(0) {
    // timers
    controlRebuildTimer = new QTimer();
    previewTimer = new QTimer();

    // title and minimum size
    setWindowTitle(tr("Expression Editor"));
    setMinimumHeight(100);

    // expression controls, we need for signal connections
    this->controls = controls;

    // make layout
    QVBoxLayout* exprAndErrors = new QVBoxLayout;
    exprAndErrors->setMargin(0);
    setLayout(exprAndErrors);

    // create text editor widget
    exprTe = new ExprTextEdit(this);
    exprTe->setObjectName(QString::fromUtf8("exprTe"));
    exprTe->setMinimumHeight(50);

    // calibrate the font size
    // This should be done inside the target application. --amyspark
    // int fontsize = 12
    // QFont font("Liberation Sans", fontsize);
    // QFont font = exprTe->font();
    // font.setPointSize(fontsize);
    // while (QFontMetrics(font).boundingRect("yabcdef").width() < 38 && fontsize < 20) {
    //     fontsize++;
    //     font.setPointSize(fontsize);
    // } ;
    // while (QFontMetrics(font).boundingRect("abcdef").width() > 44 && fontsize > 3) {
    //     fontsize--;
    //     font.setPointSize(fontsize);
    // };
    // exprTe->setFont(font);

    exprAndErrors->addWidget(exprTe);

    // create error widget
    errorWidget = new QListWidget();
    errorWidget->setObjectName(QString::fromUtf8("errorWidget"));
    errorWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    errorWidget->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    errorWidget->setMinimumHeight(30);
    connect(errorWidget, SIGNAL(itemSelectionChanged()), SLOT(selectError()));
    clearErrors();
    exprAndErrors->addWidget(errorWidget);

    // wire up signals
    connect(exprTe, SIGNAL(applyShortcut()), SLOT(sendApply()));
    connect(exprTe, SIGNAL(nextError()), SLOT(nextError()));
    connect(exprTe, SIGNAL(textChanged()), SLOT(exprChanged()));
    connect(controls, SIGNAL(controlChanged(int)), SLOT(controlChanged(int)));
    connect(controls, SIGNAL(insertString(const std::string&)), SLOT(insertStr(const std::string&)));
    connect(controlRebuildTimer, SIGNAL(timeout()), SLOT(rebuildControls()));
    connect(controlRebuildTimer, SIGNAL(timeout()), SLOT(sendPreview()));
    connect(previewTimer, SIGNAL(timeout()), SLOT(sendPreview()));
}

void ExprEditor::selectError() {
    int selected = errorWidget->currentRow();
    QListWidgetItem* item = errorWidget->item(selected);
    int start = item->data(Qt::UserRole).toInt();
    int end = item->data(Qt::UserRole + 1).toInt();
    QTextCursor cursor = exprTe->textCursor();
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, start);
    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, end - start + 1);
    exprTe->setTextCursor(cursor);
}

void ExprEditor::sendApply() { emit apply(); }

void ExprEditor::sendPreview() { emit preview(); }

void ExprEditor::exprChanged() {
    if (_updatingText) return;

    // schedule control rebuild
    controlRebuildTimer->setSingleShot(true);
    controlRebuildTimer->start(0);
}

void ExprEditor::rebuildControls() {
    bool wasShown = !exprTe->completer->popup()->isHidden();
    bool newVariables = controls->rebuildControls(exprTe->toPlainText(), exprTe->completionModel->local_variables);
    if (newVariables) exprTe->completer->setModel(exprTe->completionModel);
    if (wasShown) exprTe->completer->popup()->show();
}

QString ExprEditor::getExpr() { return exprTe->toPlainText(); }

void ExprEditor::setExpr(const QString& expression, const bool doApply) {
    // exprTe->clear();
    exprTe->selectAll();
    exprTe->insertPlainText(expression);
    clearErrors();
    exprTe->moveCursor(QTextCursor::Start);
    if (doApply) emit apply();
}

void ExprEditor::insertStr(const std::string& str) { exprTe->insertPlainText(QString::fromStdString(str)); }

void ExprEditor::appendStr(const std::string& str) { exprTe->append(QString::fromStdString(str)); }

void ExprEditor::addError(const int startPos, const int endPos, const QString& error) {
    QString message = tr("(%1, %2): %3").arg(startPos).arg(endPos).arg(error);
    QListWidgetItem* item = new QListWidgetItem(message, errorWidget);
    item->setData(Qt::UserRole, startPos);
    item->setData(Qt::UserRole + 1, endPos);
    errorWidget->setHidden(false);

    // Underline error
    QTextCursor cursor = exprTe->textCursor();
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, startPos);
    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, endPos - startPos + 1);
    QList<QTextEdit::ExtraSelection> extraSelections = exprTe->extraSelections();
    QTextEdit::ExtraSelection selection;
    QColor lineColor = QColor(Qt::yellow).lighter(130);
    selection.format.setUnderlineColor(lineColor);
    selection.format.setUnderlineStyle(QTextCharFormat::UnderlineStyle::WaveUnderline);
    selection.cursor = cursor;
    extraSelections.append(selection);
    exprTe->setExtraSelections(extraSelections);

    // errorWidget has its height fixed -- amyspark
    // TODO: fix to not use count lines and compute heuristic of 25 pixels per line!
    // const char* c = error.c_str();
    // int lines = 1;
    // while (*c != '\0') {
    //     if (*c == '\n') lines++;
    //     c++;
    // }
    // errorHeight += 25 * lines;
    // // widget should not need to be bigger than this
    // errorWidget->setMaximumHeight(errorHeight);
}

void ExprEditor::nextError() {
    int newRow = errorWidget->currentRow() + 1;
    if (newRow >= errorWidget->count()) newRow = 0;
    errorWidget->setCurrentRow(newRow);
}

void ExprEditor::clearErrors() {
    QList<QTextEdit::ExtraSelection> extraSelections;
    exprTe->setExtraSelections(extraSelections);
    errorWidget->clear();
    errorWidget->setHidden(true);
    errorHeight = 0;
}

void ExprEditor::clearExtraCompleters() {
    exprTe->completionModel->clearFunctions();
    exprTe->completionModel->clearVariables();
}

void ExprEditor::registerExtraFunction(const QString& name, const QString& docString) {
    exprTe->completionModel->addFunction(name, docString);
}

void ExprEditor::registerExtraVariable(const QString& name, const QString& docString) {
    exprTe->completionModel->addVariable(name, docString);
}

void ExprEditor::replaceExtras(const ExprCompletionModel& completer) { exprTe->completionModel->syncExtras(completer); }

void ExprEditor::updateCompleter() { exprTe->completer->setModel(exprTe->completionModel); }

void ExprEditor::updateStyle() { exprTe->updateStyle(); }
