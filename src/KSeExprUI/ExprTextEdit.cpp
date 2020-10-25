// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * @file ExprTextEdit.cpp
 * @brief This provides an expression editor for SeExpr syntax with auto ui features
 * @author  aselle
 */

#include <QAction>
#include <QLabel>
#include <QMenu>
#include <QScrollBar>
#include <QTreeView>

#include "ExprTextEdit.h"

ExprTextEdit::ExprTextEdit(QWidget *parent)
    : QTextEdit(parent)
{
    highlighter = new ExprHighlighter(document());

    // Block all external RTF input - amyspark
    this->setAcceptRichText(false);

    // setup auto completion
    completer = new QCompleter();
    completionModel = new ExprCompletionModel(this);
    completer->setModel(completionModel);
    auto *treePopup = new QTreeView;
    completer->setPopup(treePopup);
    treePopup->setRootIsDecorated(false);
    treePopup->setMinimumWidth(300);
    treePopup->setMinimumHeight(50);
    treePopup->setItemsExpandable(true);
    treePopup->setWordWrap(true);

    completer->setWidget(this);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    QObject::connect(completer, SIGNAL(activated(const QString &)), this, SLOT(insertCompletion(const QString &)));

    _popupEnabledAction = new QAction(tr("Pop-up Help"), this);
    _popupEnabledAction->setCheckable(true);
    _popupEnabledAction->setChecked(true);

    this->horizontalScrollBar()->setObjectName("exprTextEdit_horizontalBar");
    this->verticalScrollBar()->setObjectName("exprTextEdit_verticalBar");
}

void ExprTextEdit::updateStyle()
{
    lastStyleForHighlighter = nullptr;
    highlighter->fixStyle(palette());
    highlighter->rehighlight();
    repaint();
}

void ExprTextEdit::focusInEvent(QFocusEvent *e)
{
    if (completer)
        completer->setWidget(this);
    QTextEdit::focusInEvent(e);
}

void ExprTextEdit::focusOutEvent(QFocusEvent *e)
{
    hideTip();
    QTextEdit::focusInEvent(e);
}

void ExprTextEdit::mousePressEvent(QMouseEvent *event)
{
    hideTip();
    QTextEdit::mousePressEvent(event);
}

void ExprTextEdit::mouseDoubleClickEvent(QMouseEvent *event)
{
    hideTip();
    QTextEdit::mouseDoubleClickEvent(event);
}

void ExprTextEdit::paintEvent(QPaintEvent *event)
{
    if (lastStyleForHighlighter != style()) {
        lastStyleForHighlighter = style();
        highlighter->fixStyle(palette());
        highlighter->rehighlight();
    }
    QTextEdit::paintEvent(event);
}

void ExprTextEdit::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier) {
        if (event->delta() > 0)
            zoomIn();
        else if (event->delta() < 0)
            zoomOut();
    }
    return QTextEdit::wheelEvent(event);
}

void ExprTextEdit::keyPressEvent(QKeyEvent *e)
{
    // Accept expression
    if (e->key() == Qt::Key_Return && e->modifiers() == Qt::ControlModifier) {
        emit applyShortcut();
        return;
    } else if (e->key() == Qt::Key_F4) {
        emit nextError();
        return;
    } else if (e->key() == Qt::Key_Backspace && e->modifiers() == Qt::ControlModifier) {
        removeWord();
        return;
    }

    // If the completer is active pass keys it needs down
    if (completer && completer->popup()->isVisible()) {
        switch (e->key()) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
        case Qt::Key_Escape:
        case Qt::Key_Tab:
        case Qt::Key_Backtab:
            e->ignore();
            return;
        default:
            break;
        }
    }

    // use the values here as long as we are not using the shortcut to bring up the editor
    bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_E); // CTRL+E
    if (!isShortcut)                                                                     // dont process the shortcut when we have a completer
        QTextEdit::keyPressEvent(e);

    const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
    if (!completer || (ctrlOrShift && e->text().isEmpty()))
        return;

    bool hasModifier = (e->modifiers() != Qt::NoModifier) && ~(e->modifiers() & Qt::KeypadModifier) && !ctrlOrShift;

    // grab the line we're on
    QTextCursor tc = textCursor();
    tc.movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);
    QString line = tc.selectedText();

    // matches the last prefix of a completable variable or function and extract as completionPrefix
    static QRegExp completion(QString::fromLatin1("^(?:.*[^A-Za-z0-9_$])?((?:\\$[A-Za-z0-9_]*)|[A-Za-z]+[A-Za-z0-9_]*)$"));
    int index = completion.indexIn(line);
    QString completionPrefix;
    if (index != -1 && !line.contains(QLatin1Char('#'))) {
        completionPrefix = completion.cap(1);
        // std::cout<<"we have completer prefix '"<<completionPrefix.toStdString()<<"'"<<std::endl;
    }

    // hide the completer if we have too few characters, we are at end of word
    if (!isShortcut && (hasModifier || e->text().isEmpty() || completionPrefix.length() < 1 || index == -1)) {
        completer->popup()->hide();
    } else if (_popupEnabledAction->isChecked()) {
        // copy the completion prefix in if we don't already have it in the completer
        if (completionPrefix != completer->completionPrefix()) {
            completer->setCompletionPrefix(completionPrefix);
            completer->popup()->setCurrentIndex(completer->completionModel()->index(0, 0));
        }

        // display the completer
        QRect cr = cursorRect();
        cr.setWidth(completer->popup()->sizeHintForColumn(0) + completer->popup()->sizeHintForColumn(1) + completer->popup()->verticalScrollBar()->sizeHint().width());
        cr.translate(0, 6);
        completer->complete(cr);
        hideTip();
        return;
    }

    // documentation completion
    static QRegExp inFunction(QString::fromLatin1("^(?:.*[^A-Za-z0-9_$])?([A-Za-z0-9_]+)\\([^()]*$"));
    int index2 = inFunction.indexIn(line);
    if (index2 != -1) {
        QString functionName = inFunction.cap(1);
        QStringList tips = completionModel->getDocString(functionName).split(QString::fromLatin1("\n"));
        QString tip = QString(tr("<b>%1</b>")).arg(tips[0]);
        for (int i = 1; i < tips.size(); i++) {
            tip += QString(tr("<br>%1")).arg(tips[i]);
        }
        if (_popupEnabledAction->isChecked())
            showTip(tip);
        // QToolTip::showText(mapToGlobal(cr.bottomLeft()),tip,this,cr);
    } else {
        hideTip();
    }
}

void ExprTextEdit::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = createStandardContextMenu();

    if (!menu->actions().empty()) {
        QAction *f = menu->actions().first();
        menu->insertAction(f, _popupEnabledAction);
        menu->insertSeparator(f);
    }

    menu->exec(event->globalPos());
    delete menu;
}

void ExprTextEdit::showTip(const QString &string)
{
    // skip empty strings
    if (string.isEmpty())
        return;
    // skip already shown stuff
    if (QToolTip::isVisible())
        return;

    QRect cr = cursorRect();
    cr.setX(0);
    cr.setWidth(cr.width() * 3);
    QToolTip::showText(mapToGlobal(cr.bottomLeft()) + QPoint(0, 6), string);
}

void ExprTextEdit::hideTip()
{
    QToolTip::hideText();
}

void ExprTextEdit::insertCompletion(const QString &completion)
{
    if (completer->widget() != this)
        return;
    QTextCursor tc = textCursor();
    int extra = completion.length() - completer->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    setTextCursor(tc);
}

void ExprTextEdit::removeWord()
{
    QTextCursor tc = textCursor();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.select(QTextCursor::WordUnderCursor);
    tc.removeSelectedText();
    setTextCursor(tc);
}
