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
* @file ExprDialog.cpp
* @brief A basic editor/browser/previewer for expression editing
* @author  jlacewel
*/

#include <QApplication>
#include <QDir>
#include <QLabel>
#include <QTextBrowser>
#include <fstream>
#include <iostream>

#include "ExprBrowser.h"
#include "ExprDialog.h"
#include "ExprControlCollection.h"
#include "ExprGrapher2d.h"

#define P3D_CONFIG_ENVVAR "P3D_CONFIG_PATH"

ExprDialog::ExprDialog(QWidget* parent) : QDialog(parent), _currentEditorIdx(0) {
    this->setMinimumWidth(600);
    QVBoxLayout* rootLayout = new QVBoxLayout(0);
    rootLayout->setMargin(2);
    this->setLayout(rootLayout);

    showEditorTimer = new QTimer();
    connect(showEditorTimer, SIGNAL(timeout()), SLOT(_showEditor()));

    QSplitter* vsplitter = new QSplitter(Qt::Vertical, this);
    rootLayout->addWidget(vsplitter);

    QTabWidget* topTabWidget = new QTabWidget();
    vsplitter->addWidget(topTabWidget);

    QWidget* previewLibraryWidget = new QWidget();
    QHBoxLayout* previewLibraryLayout = new QHBoxLayout();
    previewLibraryWidget->setLayout(previewLibraryLayout);
    topTabWidget->addTab(previewLibraryWidget, tr("Preview / Library"));

    QWidget* bottomWidget = new QWidget();
    vsplitter->addWidget(bottomWidget);
    QVBoxLayout* bottomLayout = new QVBoxLayout();
    bottomLayout->setMargin(1);
    bottomWidget->setLayout(bottomLayout);

    // setup preview
    QWidget* leftWidget = new QWidget();
    leftWidget->setFixedWidth(450);
    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->setMargin(0);
    leftWidget->setLayout(leftLayout);
    QHBoxLayout* previewLayout = new QHBoxLayout();
    grapher = new ExprGrapherWidget(this, 200, 200);
    previewLayout->addWidget(grapher, 0);
    previewCommentLabel = new QLabel();
    previewLayout->addWidget(previewCommentLabel, 1, Qt::AlignLeft | Qt::AlignTop);
    leftLayout->addLayout(previewLayout);
    previewLibraryLayout->addWidget(leftWidget);

    // setup button bar
    // QWidget* buttonBarWidget=new QWidget();
    QHBoxLayout* buttonBarLayout = new QHBoxLayout();
    // buttonBarWidget->setLayout(buttonBarLayout);
    buttonBarLayout->setMargin(1);
    previewButton = new QPushButton(tr("Preview"));
    buttonBarLayout->addWidget(previewButton);
    saveButton = new QPushButton(tr("Save"));
    buttonBarLayout->addWidget(saveButton);
    saveAsButton = new QPushButton(tr("Save As"));
    buttonBarLayout->addWidget(saveAsButton);
    saveLocalButton = new QPushButton(tr("Save Local"));
    saveLocalButton->setEnabled(false);
    buttonBarLayout->addWidget(saveLocalButton);
    clearButton = new QPushButton(tr("Clear"));
    buttonBarLayout->addWidget(clearButton);
    bottomLayout->addLayout(buttonBarLayout);

    controls = new ExprControlCollection();

    // controls
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidget(controls);
    // scrollArea->setWidget(new QLabel("test\nweird\nfds\nfdsahsha\nfsdajdlsa\nfasdjjhsafd\nfasdhjdfsa\nfdasjdfsha"));
    scrollArea->setFocusPolicy(Qt::NoFocus);
    scrollArea->setMinimumHeight(100);
    scrollArea->setFixedWidth(450);
    scrollArea->setWidgetResizable(true);
    leftLayout->addWidget(scrollArea, 1);

    // make button bar
    editor = new ExprEditor(this, controls);
    connect(editor, SIGNAL(apply()), SLOT(verifiedApply()));
    connect(editor, SIGNAL(preview()), SLOT(previewExpression()));
    connect(grapher, SIGNAL(preview()), SLOT(previewExpression()));
    bottomLayout->addWidget(editor);

    // make expression library browser
    browser = new ExprBrowser(0, editor);
    previewLibraryLayout->addWidget(browser);

    // dialog buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout(0);
    buttonLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
    applyButton = new QPushButton(tr("Apply"));
    buttonLayout->addWidget(applyButton);
    acceptButton = new QPushButton(tr("Accept"));
    buttonLayout->addWidget(acceptButton);
    cancelButton = new QPushButton(tr("Cancel"));
    buttonLayout->addWidget(cancelButton);
    connect(applyButton, SIGNAL(clicked()), this, SLOT(verifiedApply()));
    connect(acceptButton, SIGNAL(clicked()), this, SLOT(verifiedAccept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    rootLayout->addLayout(buttonLayout);

    setupHelp(topTabWidget);

    // connect buttons
    connect(previewButton, SIGNAL(clicked()), SLOT(previewExpression()));
    connect(clearButton, SIGNAL(clicked()), SLOT(clearExpression()));
    connect(saveButton, SIGNAL(clicked()), browser, SLOT(saveExpression()));
    connect(saveAsButton, SIGNAL(clicked()), browser, SLOT(saveExpressionAs()));
    connect(saveLocalButton, SIGNAL(clicked()), browser, SLOT(saveLocalExpressionAs()));
}

void ExprDialog::showEditor(int idx) {
    _currentEditorIdx = idx;
    showEditorTimer->setSingleShot(true);
    showEditorTimer->start();
}

void ExprDialog::_showEditor() { controls->showEditor(_currentEditorIdx); }

void ExprDialog::show() {
    // populate the expressions
    browser->getExpressionDirs();
    browser->expandAll();
    QDialog::show();
}

int ExprDialog::exec() {
    // populate the expressions
    browser->getExpressionDirs();
    browser->expandAll();
    return QDialog::exec();
}

void ExprDialog::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) return;
    return QDialog::keyPressEvent(event);
}

void ExprDialog::closeEvent(QCloseEvent* event) {
    emit dialogClosed();
    QDialog::closeEvent(event);
}

void ExprDialog::reject() {
    emit dialogClosed();
    QDialog::reject();
}

void ExprDialog::verifiedApply() {
    applyExpression();
    if (grapher->expr.isValid()) {
        emit expressionApplied();
    } else {
        QMessageBox msgBox;
        msgBox.setText(tr("Your expression had possible errors."));
        msgBox.setInformativeText(tr("Do you want to accept your expression anyways?"));
        msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
        switch (msgBox.exec()) {
            case QMessageBox::Ok:
                emit expressionApplied();
                break;
            default:
                break;
        }
    }
}

void ExprDialog::verifiedAccept() {
    applyExpression();
    if (grapher->expr.isValid()) {
        emit expressionApplied();
        emit dialogClosed();
        accept();
    } else {
        QMessageBox msgBox;
        msgBox.setText(tr("Your expression had possible errors."));
        msgBox.setInformativeText(tr("Do you want to accept your expression anyways?"));
        msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
        switch (msgBox.exec()) {
            case QMessageBox::Ok:
                emit expressionApplied();
                emit dialogClosed();
                accept();
                break;
            default:
                break;
        }
    }
}

void ExprDialog::setupHelp(QTabWidget* tab) {
    QWidget* browserspace = new QWidget(tab);
    helpBrowser = new QTextBrowser(browserspace);
    tab->addTab(browserspace, tr("Help"));

    // Locate help docs relative to location of the app itself
    QFile* helpDoc = new QFile(QString::fromLatin1("%1/../share/doc/SeExpr2/SeExpressions.html").arg(QCoreApplication::applicationDirPath()));
    if (helpDoc->exists()) {
        QString sheet =
            QString::fromLatin1("body {background-color: #eeeeee; color: #000000;} \na {color: #3333ff; text-decoration: none;}\n");
        helpBrowser->document()->setDefaultStyleSheet(sheet);
        helpBrowser->setSource(QUrl::fromLocalFile(helpDoc->fileName()));
    }

    QPushButton* backPb = new QPushButton(tr("Back"));
    // backPb->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowLeft));
    backPb->setEnabled(false);
    QPushButton* forwardPb = new QPushButton(tr("Forward"));
    //    forwardPb->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowRight));
    forwardPb->setEnabled(false);

    QVBoxLayout* helpLayout = new QVBoxLayout(browserspace);
    QHBoxLayout* helpPbLayout = new QHBoxLayout;
    helpLayout->addLayout(helpPbLayout);
    helpPbLayout->addWidget(backPb);
    helpPbLayout->addWidget(forwardPb);
    // helpPbLayout->addItem(new QSpacerItem(0,0, QSizePolicy::MinimumExpanding,
    //                            QSizePolicy::Minimum));
    QHBoxLayout* findBar = new QHBoxLayout();
    helpPbLayout->addWidget(new QLabel(tr("Find")), /*stretch*/ false);
    helpFindBox = new QLineEdit;
    helpPbLayout->addWidget(helpFindBox, /*stretch*/ false);
    connect(helpFindBox, SIGNAL(returnPressed()), this, SLOT(findNextInHelp()));
    QPushButton* nextButton = new QPushButton(tr("Find Next"));
    QPushButton* prevButton = new QPushButton(tr("Find Prev"));
    helpPbLayout->addWidget(nextButton, /*stretch*/ false);
    helpPbLayout->addWidget(prevButton, /*stretch*/ false);
    connect(nextButton, SIGNAL(clicked()), this, SLOT(findNextInHelp()));
    connect(prevButton, SIGNAL(clicked()), this, SLOT(findPrevInHelp()));
    helpPbLayout->addLayout(findBar, /*stretch*/ false);
    helpLayout->addWidget(helpBrowser, /*stretch*/ true);
    helpBrowser->setMinimumHeight(120);

    // wire up help browser forward/back buttons
    connect(backPb, SIGNAL(clicked()), helpBrowser, SLOT(backward()));
    connect(forwardPb, SIGNAL(clicked()), helpBrowser, SLOT(forward()));
    connect(helpBrowser, SIGNAL(backwardAvailable(bool)), backPb, SLOT(setEnabled(bool)));
    connect(helpBrowser, SIGNAL(forwardAvailable(bool)), forwardPb, SLOT(setEnabled(bool)));
}

void ExprDialog::findHelper(QTextDocument::FindFlags flags) {
    QTextDocument* doc = helpBrowser->document();
    if (prevFind != helpFindBox->text()) {
        prevFind = helpFindBox->text();
        helpBrowser->setTextCursor(QTextCursor(doc));
    }
    QTextCursor blah = doc->find(helpFindBox->text(), helpBrowser->textCursor(), flags);
    helpBrowser->setTextCursor(blah);
}

void ExprDialog::findNextInHelp() { findHelper(0); }

void ExprDialog::findPrevInHelp() { findHelper(QTextDocument::FindBackward); }

void ExprDialog::previewExpression() {
    applyExpression();
    emit preview();
}

void ExprDialog::applyExpression() {
    editor->clearErrors();
    // set new expression
    grapher->expr.setExpr(editor->getExpr().toStdString());
    grapher->update();

    // set the label widget to mention that variables will not be previewed
    bool empty = true;
    if (grapher->expr.varmap.size() > 0) {
        std::stringstream s;
        s << tr("<b>Variables not supported in preview (assumed zero):</b><br>").toStdString();
        int count = 0;
        for (BasicExpression::VARMAP::iterator i = grapher->expr.varmap.begin(); i != grapher->expr.varmap.end(); ++i) {
            count++;
            s << "$" << i->first << " ";
            if (count % 4 == 0) s << "<br>";
        }
        previewCommentLabel->setText(QString::fromStdString(s.str()));
        empty = false;
    } else
        previewCommentLabel->setText(QString());
    // set the label widget to mention that variables will not be previewed
    if (grapher->expr.funcmap.size() > 0) {
        std::stringstream s;
        s << tr("<b>Functions not supported in preview (assumed zero):</b><br>").toStdString();
        int count = 0;
        for (BasicExpression::FUNCMAP::iterator i = grapher->expr.funcmap.begin(); i != grapher->expr.funcmap.end();
             ++i) {
            count++;
            s << "" << i->first << "() ";
            if (count % 4 == 0) s << "<br>";
        }
        previewCommentLabel->setText(QString::fromStdString(s.str()));
        empty = false;
    }
    if (empty) previewCommentLabel->setText(QString());

    // put errors into editor module
    bool valid = grapher->expr.isValid();
    if (!valid) {
        const std::vector<SeExpr2::Expression::Error>& errors = grapher->expr.getErrors();
        for (unsigned int i = 0; i < errors.size(); i++) {
            editor->addError(errors[i].startPos, errors[i].endPos, errors[i].error);
        }
        editor->nextError();
    }
}

void ExprDialog::clearExpression() {
    browser->clearSelection();
    editor->setExpr(QString(), false);
    grapher->expr.setExpr(std::string());
    grapher->update();
}
