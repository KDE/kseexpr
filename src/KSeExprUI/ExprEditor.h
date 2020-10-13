// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: LicenseRef-Apache-2.0
/*
* @file ExprEditor.h
* @brief This provides an expression editor for SeExpr syntax with auto ui features
* @author  aselle
*/
#ifndef ExprEditor_h
#define ExprEditor_h

#include <QListWidget>
#include <QTimer>

#include "ExprTextEdit.h"
#include "ExprControlCollection.h"

class ExprEditor : public QWidget {
    Q_OBJECT

  public:
    ExprEditor(QWidget* parent);
    virtual ~ExprEditor();
    virtual void setControlCollectionWidget(ExprControlCollection* widget);
    ExprControlCollection* controlCollectionWidget() const;

  public
Q_SLOTS:
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
    void setExpr(const QString& expression, const bool apply = false);
    // Append string
    void appendStr(const QString& str);
  public
Q_SLOTS:
    // Insert string
    void insertStr(const QString& str);

  public:
    // Adds an error and its associated position
    void addError(const int startPos, const int endPos, const QString& error);
    // Removes all errors and hides the completion widget
    void clearErrors();
    // Removes all extra completion symbols
    void clearExtraCompleters();
    // Registers an extra function and associated do cstring
    void registerExtraFunction(const QString& name, const QString& docString);
    // Register an extra variable (i.e. $P, or $u, something provided by resolveVar)
    void registerExtraVariable(const QString& name, const QString& docString);
    // Replace extras
    void replaceExtras(const ExprCompletionModel& completer);
    // Updates the completion widget, must call after registering any new functions/variables
    void updateCompleter();
    // Updates style
    void updateStyle();

  // Expose the text editor widget to receive customizations in Krita. -amyspark
  public:
    ExprTextEdit* exprTe;

  private:
    ExprControlCollection* controls;
    QListWidget* errorWidget;

    QTimer* controlRebuildTimer;
    QTimer* previewTimer;

    bool _updatingText;
    int errorHeight;
};

#endif
