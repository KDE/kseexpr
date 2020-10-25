// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

/*
 * @file ExprControlCollection.h
 * @brief Manages/creates a bunch of ExprControls by using expression text
 * @author  aselle
 */

#pragma once

#include <QColor>
#include <QComboBox>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>

/// This class is the UI for adding widgets
class ExprAddDialog : public QDialog
{
    Q_OBJECT

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
    QLineEdit *swatchLookup;
    QRadioButton *rainbowPaletteBtn;
    QRadioButton *grayPaletteBtn;
    QColor color;
    QPushButton *colorWidget;
    QComboBox *stringTypeWidget;
    QLineEdit *stringDefaultWidget;
    QLineEdit *stringNameWidget;

    ExprAddDialog(int &count, QWidget *parent = nullptr);
    const char *initSwatch() const;
private Q_SLOTS:
    void colorChooseClicked();
};
