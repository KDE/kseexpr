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
    ExprEditor(QWidget* parent, ExprControlCollection* controls);
    virtual ~ExprEditor();

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
    void appendStr(const std::string& str);
  public
Q_SLOTS:
    // Insert string
    void insertStr(const std::string& str);

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
