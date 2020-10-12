/*
* Copyright Disney Enterprises, Inc.  All rights reserved.
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
* @file ExprControlCollection.h
* @brief Manages/creates a bunch of ExprControls by using expression text
* @author  aselle
*/

#include <QColor>
#include <QComboBox>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>

/// This class is the UI for adding widgets
class ExprAddDialog : public QDialog {
    Q_OBJECT;

  public:
    QLineEdit *variableName;
    QTabWidget *tabWidget;
    QLineEdit *intDefault;
    QLineEdit *intMin;
    QLineEdit *intMax;
    QLineEdit *floatDefault;
    QLineEdit *floatMin;
    QLineEdit *floatMax;
    QLineEdit *vectorDefault0;
    QLineEdit *vectorDefault1;
    QLineEdit *vectorDefault2;
    QLineEdit *vectorMin;
    QLineEdit *vectorMax;
    QLineEdit *curveLookup;
    QLineEdit *colorCurveLookup;
#ifdef SEEXPR_ENABLE_ANIMCURVE
    QLineEdit *animCurveLookup;
    QLineEdit *animCurveLink;
#endif
    QLineEdit *swatchLookup;
#ifdef SEEXPR_ENABLE_DEEPWATER
    QLineEdit *deepWaterLookup;
#endif
    QRadioButton *rainbowPaletteBtn;
    QRadioButton *grayPaletteBtn;
    QColor color;
    QPushButton *colorWidget;
    QComboBox *stringTypeWidget;
    QLineEdit *stringDefaultWidget;
    QLineEdit *stringNameWidget;

    ExprAddDialog(int &count, QWidget *parent = 0);
    const char *initSwatch();
  private Q_SLOTS:
    void colorChooseClicked();
};
