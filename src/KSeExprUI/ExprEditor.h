// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * @file ExprEditor.h
 * @brief This provides an expression editor for SeExpr syntax with auto ui features
 * @author  aselle
 */
#pragma once

#include <QListWidget>
#include <QTimer>
#include <atomic>

#include "ExprControlCollection.h"
#include "ExprTextEdit.h"

class ExprEditor : public QWidget
{
    Q_OBJECT

public:
    ExprEditor(QWidget *parent);
    ~ExprEditor() override;
    virtual void setControlCollectionWidget(ExprControlCollection *widget);
    ExprControlCollection *controlCollectionWidget() const;

public Q_SLOTS:
    void exprChanged();
    void rebuildControls();
    void controlChanged(int id);
    void nextError();
    void selectError();
    void sendApply();
    void sendPreview();
    // void handlePreviewTimer();
Q_SIGNALS:
    void apply();
    void preview();

public:
    // Get the expression that is in the editor
    QString getExpr();
    // Sets the expression that is in the editor
    void setExpr(const QString &expression, bool apply = false);
    // Append string
    void appendStr(const QString &str);
public Q_SLOTS:
    // Insert string
    void insertStr(const QString &str);

public:
    // Adds an error and its associated position
    void addError(int startPos, int endPos, const QString &error);
    // Removes all errors and hides the completion widget
    void clearErrors();
    // Removes all extra completion symbols
    void clearExtraCompleters();
    // Registers an extra function and associated do cstring
    void registerExtraFunction(const QString &name, const QString &docString);
    // Register an extra variable (i.e. $P, or $u, something provided by resolveVar)
    void registerExtraVariable(const QString &name, const QString &docString);
    // Replace extras
    void replaceExtras(const ExprCompletionModel &completer);
    // Updates the completion widget, must call after registering any new functions/variables
    void updateCompleter();
    // Updates style
    void updateStyle();

    // Expose the text editor widget to receive customizations in Krita. -amyspark
public:
    ExprTextEdit *exprTe {nullptr};

private:
    ExprControlCollection *controls {nullptr};
    QListWidget *errorWidget {nullptr};

    QTimer *controlRebuildTimer {nullptr};
    QTimer *previewTimer {nullptr};

    std::atomic<bool> _updatingText {false};
    int errorHeight;
};
