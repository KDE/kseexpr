// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0

/*
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
    QLineEdit *swatchLookup;
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
