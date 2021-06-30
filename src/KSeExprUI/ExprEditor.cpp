// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * @file ExprEditor.cpp
 * @brief This provides an expression editor for SeExpr syntax with auto ui features
 * @author  aselle
 */

#include <QVBoxLayout>

#include <KSeExpr/ExprBuiltins.h>
#include <KSeExpr/ExprFunc.h>
#include <KSeExpr/ExprNode.h>
#include <KSeExpr/Expression.h>


#include "ExprColorCurve.h"
#include "ExprCompletionModel.h"
#include "ExprControl.h"
#include "ExprControlCollection.h"
#include "ExprCurve.h"
#include "ExprEditor.h"
#include "ExprHighlighter.h"
#include "ExprPopupDoc.h"


ExprLineEdit::ExprLineEdit(int id, QWidget *parent)
    : QLineEdit(parent)
    , _id(id)
{
    connect(this, SIGNAL(textChanged(const QString &)), SLOT(textChangedCB(const QString &)));
}

void ExprLineEdit::textChangedCB(const QString &text)
{
    _signaling = true;
    emit textChanged(_id, text);
    _signaling = false;
}

void ExprEditor::controlChanged(int id)
{
    QString newText = exprTe->toPlainText();
    controls->updateText(id, newText);
    _updatingText = true;
    exprTe->selectAll();
    exprTe->insertPlainText(newText);
    // exprTe->setPlainText(newText);
    _updatingText = false;

    // schedule preview update
    previewTimer->setSingleShot(true);
    previewTimer->start(0);
}

ExprEditor::~ExprEditor()
{
    delete controlRebuildTimer;
    delete previewTimer;
}

ExprEditor::ExprEditor(QWidget *parent)
    : QWidget(parent)
    , errorHeight(0)
{
    // timers
    controlRebuildTimer = new QTimer();
    previewTimer = new QTimer();

    // title and minimum size
    setWindowTitle(tr("Expression Editor"));
    setMinimumHeight(100);

    // make layout
    auto *exprAndErrors = new QVBoxLayout;
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

    exprAndErrors->addWidget(exprTe, 4);

    // create error widget
    errorWidget = new QListWidget();
    errorWidget->setObjectName(QString::fromUtf8("errorWidget"));
    errorWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    errorWidget->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    errorWidget->setMinimumHeight(30);
    connect(errorWidget, SIGNAL(itemSelectionChanged()), SLOT(selectError()));
    clearErrors();
    exprAndErrors->addWidget(errorWidget, 1);

    // wire up signals
    connect(exprTe, SIGNAL(applyShortcut()), SLOT(sendApply()));
    connect(exprTe, SIGNAL(nextError()), SLOT(nextError()));
    connect(exprTe, SIGNAL(textChanged()), SLOT(exprChanged()));
    connect(controlRebuildTimer, SIGNAL(timeout()), SLOT(sendPreview()));
    connect(previewTimer, SIGNAL(timeout()), SLOT(sendPreview()));
}

ExprControlCollection *ExprEditor::controlCollectionWidget() const
{
    return this->controls;
}

// expression controls, we need for signal connections
void ExprEditor::setControlCollectionWidget(ExprControlCollection *widget)
{
    if (this->controls) {
        disconnect(controlRebuildTimer, SIGNAL(timeout())), disconnect(controls, SIGNAL(controlChanged(int)));
        disconnect(controlRebuildTimer, SIGNAL(timeout()));
    }

    this->controls = widget;

    if (this->controls) {
        connect(controlRebuildTimer, SIGNAL(timeout()), SLOT(rebuildControls()));
        connect(controls, SIGNAL(controlChanged(int)), SLOT(controlChanged(int)));
        connect(controls, SIGNAL(insertString(const QString &)), SLOT(insertStr(const QString &)));
    }
}

void ExprEditor::selectError()
{
    int selected = errorWidget->currentRow();
    QListWidgetItem *item = errorWidget->item(selected);
    int start = item->data(Qt::UserRole).toInt();
    int end = item->data(Qt::UserRole + 1).toInt();
    QTextCursor cursor = exprTe->textCursor();
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, start);
    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, end - start + 1);
    exprTe->setTextCursor(cursor);
}

void ExprEditor::sendApply()
{
    emit apply();
}

void ExprEditor::sendPreview()
{
    emit preview();
}

void ExprEditor::exprChanged()
{
    if (_updatingText)
        return;

    // schedule control rebuild
    controlRebuildTimer->setSingleShot(true);
    controlRebuildTimer->start(0);
}

void ExprEditor::rebuildControls()
{
    bool wasShown = !exprTe->completer->popup()->isHidden();
    bool newVariables = controls->rebuildControls(exprTe->toPlainText(), exprTe->completionModel->local_variables);
    if (newVariables)
        exprTe->completer->setModel(exprTe->completionModel);
    if (wasShown)
        exprTe->completer->popup()->show();
}

QString ExprEditor::getExpr()
{
    return exprTe->toPlainText();
}

void ExprEditor::setExpr(const QString &expression, const bool doApply)
{
    // exprTe->clear();
    exprTe->selectAll();
    exprTe->insertPlainText(expression);
    clearErrors();
    exprTe->moveCursor(QTextCursor::Start);
    if (doApply)
        emit apply();
}

void ExprEditor::insertStr(const QString &str)
{
    exprTe->moveCursor(QTextCursor::StartOfLine);
    exprTe->insertPlainText(str);
}

void ExprEditor::appendStr(const QString &str)
{
    exprTe->append(str);
}

void ExprEditor::addError(const int startPos, const int endPos, const QString &error)
{
    QString message = tr("(%1, %2): %3").arg(startPos).arg(endPos).arg(error);
    auto *item = new QListWidgetItem(message, errorWidget);
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
    // ensure cursor stays visible if it was hidden by the error widget -- amyspark
    exprTe->ensureCursorVisible();
}

void ExprEditor::nextError()
{
    int newRow = errorWidget->currentRow() + 1;
    if (newRow >= errorWidget->count())
        newRow = 0;
    errorWidget->setCurrentRow(newRow);
}

void ExprEditor::clearErrors()
{
    QList<QTextEdit::ExtraSelection> extraSelections;
    exprTe->setExtraSelections(extraSelections);
    errorWidget->clear();
    errorWidget->setHidden(true);
    errorHeight = 0;
}

void ExprEditor::clearExtraCompleters()
{
    exprTe->completionModel->clearFunctions();
    exprTe->completionModel->clearVariables();
}

void ExprEditor::registerExtraFunction(const QString &name, const QString &docString)
{
    exprTe->completionModel->addFunction(name, docString);
}

void ExprEditor::registerExtraVariable(const QString &name, const QString &docString)
{
    exprTe->completionModel->addVariable(name, docString);
}

void ExprEditor::replaceExtras(const ExprCompletionModel &completer)
{
    exprTe->completionModel->syncExtras(completer);
}

void ExprEditor::updateCompleter()
{
    exprTe->completer->setModel(exprTe->completionModel);
}

void ExprEditor::updateStyle()
{
    exprTe->updateStyle();
}
