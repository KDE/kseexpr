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
* @file ExprAddDialog.cpp
* @brief Manages/creates a new variable by using ExprControls
* @author  aselle, amyspark
*/

#include <QColorDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

#include "ExprAddDialog.h"

ExprAddDialog::ExprAddDialog(int& count, QWidget* parent) : QDialog(parent) {
    QVBoxLayout* verticalLayout;
    verticalLayout = new QVBoxLayout();
    verticalLayout->setSpacing(3);
    verticalLayout->setMargin(3);
    setLayout(verticalLayout);
    QHBoxLayout* horizontalLayout = new QHBoxLayout();

    horizontalLayout->addWidget(new QLabel(tr("Variable")));
    // TODO would be nice to unique this over multiple sessions
    variableName = new QLineEdit(QString::fromLatin1("$var%1").arg(count++));

    horizontalLayout->addWidget(variableName);
    verticalLayout->addLayout(horizontalLayout);

    tabWidget = new QTabWidget();

    // Curve
    {
        QWidget* curveTab = new QWidget();
        QFormLayout* curveLayout = new QFormLayout(curveTab);
        curveLayout->setWidget(0, QFormLayout::LabelRole, new QLabel(tr("Lookup")));
        curveLookup = new QLineEdit(tr("$u"));
        curveLayout->setWidget(0, QFormLayout::FieldRole, curveLookup);
        tabWidget->addTab(curveTab, QString(tr("Curve")));
    }

    // Color Curve
    {
        QWidget* colorCurveTab = new QWidget();
        QFormLayout* colorCurveLayout = new QFormLayout(colorCurveTab);
        colorCurveLayout->setWidget(0, QFormLayout::LabelRole, new QLabel(tr("Lookup")));
        colorCurveLookup = new QLineEdit(tr("$u"));
        colorCurveLayout->setWidget(0, QFormLayout::FieldRole, colorCurveLookup);
        tabWidget->addTab(colorCurveTab, QString(tr("Color Curve")));
    }

    // Integer
    {
        QWidget* intTab = new QWidget();
        QFormLayout* intFormLayout = new QFormLayout(intTab);
        intFormLayout->setWidget(0, QFormLayout::LabelRole, new QLabel(tr("Default")));
        intFormLayout->setWidget(1, QFormLayout::LabelRole, new QLabel(tr("Min")));
        intFormLayout->setWidget(2, QFormLayout::LabelRole, new QLabel(tr("Max")));
        intDefault = new QLineEdit(tr("0"));
        intFormLayout->setWidget(0, QFormLayout::FieldRole, intDefault);
        intMin = new QLineEdit(tr("0"));
        intFormLayout->setWidget(1, QFormLayout::FieldRole, intMin);
        intMax = new QLineEdit(tr("10"));
        intFormLayout->setWidget(2, QFormLayout::FieldRole, intMax);
        tabWidget->addTab(intTab, QString(tr("Int")));
    }

    // Float
    {
        QWidget* floatTab = new QWidget();
        QFormLayout* floatFormLayout = new QFormLayout(floatTab);
        floatFormLayout->setWidget(0, QFormLayout::LabelRole, new QLabel(tr("Default")));
        floatFormLayout->setWidget(1, QFormLayout::LabelRole, new QLabel(tr("Min")));
        floatFormLayout->setWidget(2, QFormLayout::LabelRole, new QLabel(tr("Max")));
        floatDefault = new QLineEdit(tr("0"));
        floatFormLayout->setWidget(0, QFormLayout::FieldRole, floatDefault);
        floatMin = new QLineEdit(tr("0"));
        floatFormLayout->setWidget(1, QFormLayout::FieldRole, floatMin);
        floatMax = new QLineEdit(tr("1"));
        floatFormLayout->setWidget(2, QFormLayout::FieldRole, floatMax);

        tabWidget->addTab(floatTab, QString(tr("Float")));
    }

    // Vector
    {
        QWidget* vectorTab = new QWidget();
        QFormLayout* vectorFormLayout = new QFormLayout(vectorTab);
        vectorFormLayout->setWidget(0, QFormLayout::LabelRole, new QLabel(tr("Default")));
        vectorFormLayout->setWidget(1, QFormLayout::LabelRole, new QLabel(tr("Min")));
        vectorFormLayout->setWidget(2, QFormLayout::LabelRole, new QLabel(tr("Max")));
        vectorDefault0 = new QLineEdit(tr("0"));
        vectorDefault1 = new QLineEdit(tr("0"));
        vectorDefault2 = new QLineEdit(tr("0"));
        QHBoxLayout* compLayout = new QHBoxLayout();
        compLayout->addWidget(vectorDefault0);
        compLayout->addWidget(vectorDefault1);
        compLayout->addWidget(vectorDefault2);
        vectorFormLayout->setLayout(0, QFormLayout::FieldRole, compLayout);
        vectorMin = new QLineEdit(tr("0"));
        vectorFormLayout->setWidget(1, QFormLayout::FieldRole, vectorMin);
        vectorMax = new QLineEdit(tr("1"));
        vectorFormLayout->setWidget(2, QFormLayout::FieldRole, vectorMax);

        tabWidget->addTab(vectorTab, QString(tr("Vector")));
    }

    // Color
    {
        QWidget* colorTab = new QWidget();
        QFormLayout* colorLayout = new QFormLayout(colorTab);
        colorWidget = new QPushButton();
        colorWidget->setFixedWidth(30);
        colorWidget->setFixedWidth(30);
        colorLayout->setWidget(0, QFormLayout::LabelRole, new QLabel(tr("Color")));
        colorLayout->setWidget(0, QFormLayout::FieldRole, colorWidget);
        color = Qt::red;
        QPixmap colorPix(30, 30);
        colorPix.fill(color);
        colorWidget->setIcon(QIcon(colorPix));
        tabWidget->addTab(colorTab, QString(tr("Color")));

        connect(colorWidget, SIGNAL(clicked()), this, SLOT(colorChooseClicked()));
    }

    // Color Swatch
    {
        QWidget* swatchTab = new QWidget();
        QFormLayout* swatchLayout = new QFormLayout(swatchTab);
        swatchLookup = new QLineEdit(tr("$u"));
        swatchLayout->setWidget(0, QFormLayout::LabelRole, new QLabel(tr("Lookup")));
        swatchLayout->setWidget(0, QFormLayout::FieldRole, swatchLookup);
        rainbowPaletteBtn = new QRadioButton(tr("Rainbow"));
        rainbowPaletteBtn->setChecked(true);
        grayPaletteBtn = new QRadioButton(tr("Shades of Gray"));
        swatchLayout->setWidget(1, QFormLayout::LabelRole, new QLabel(tr("Colors")));
        swatchLayout->setWidget(1, QFormLayout::FieldRole, rainbowPaletteBtn);
        swatchLayout->setWidget(2, QFormLayout::LabelRole, new QLabel(QString()));
        swatchLayout->setWidget(2, QFormLayout::FieldRole, grayPaletteBtn);
        tabWidget->addTab(swatchTab, QString(tr("Swatch")));
    }

    // String literal
    {
        QWidget* stringTab = new QWidget();
        QFormLayout* stringLayout = new QFormLayout(stringTab);
        stringTypeWidget = new QComboBox();
        stringTypeWidget->addItem(tr("string"));
        stringTypeWidget->addItem(tr("file"));
        stringTypeWidget->addItem(tr("directory"));
        stringDefaultWidget = new QLineEdit();
        stringNameWidget = new QLineEdit(QString::fromLatin1("str1"));

        stringLayout->setWidget(0, QFormLayout::LabelRole, new QLabel(tr("String Name")));
        stringLayout->setWidget(0, QFormLayout::FieldRole, stringNameWidget);
        stringLayout->setWidget(1, QFormLayout::LabelRole, new QLabel(tr("String Type")));
        stringLayout->setWidget(1, QFormLayout::FieldRole, stringTypeWidget);
        stringLayout->setWidget(2, QFormLayout::LabelRole, new QLabel(tr("String Default")));
        stringLayout->setWidget(3, QFormLayout::FieldRole, stringDefaultWidget);

        tabWidget->addTab(stringTab, QString(tr("String")));
    }
#ifdef SEEXPR_ENABLE_ANIMCURVE
    // Anim Curve
    {
        QWidget* curveTab = new QWidget();
        QFormLayout* curveLayout = new QFormLayout(curveTab);
        curveLayout->setWidget(0, QFormLayout::LabelRole, new QLabel(tr("Lookup")));
        curveLayout->setWidget(1, QFormLayout::LabelRole, new QLabel(tr("Link")));
        animCurveLookup = new QLineEdit(QString::fromLatin1("$frame"));
        animCurveLink = new QLineEdit(QString());
        curveLayout->setWidget(0, QFormLayout::FieldRole, animCurveLookup);
        curveLayout->setWidget(1, QFormLayout::FieldRole, animCurveLink);
        tabWidget->addTab(curveTab, QString(tr("AnimCurve")));
    }
#endif
#ifdef SEEXPR_ENABLE_DEEPWATER
    // DeepWater
    {
        QWidget* deepWaterTab = new QWidget();
        QFormLayout* deepWaterLayout = new QFormLayout(deepWaterTab);
        deepWaterLayout->setWidget(0, QFormLayout::LabelRole, new QLabel(tr("Lookup")));
        deepWaterLookup = new QLineEdit(tr("$u"));
        deepWaterLayout->setWidget(0, QFormLayout::FieldRole, deepWaterLookup);
        tabWidget->addTab(deepWaterTab, QString(tr("Deep Water")));
    }
#endif

    verticalLayout->addWidget(tabWidget);

    QDialogButtonBox* buttonBox = new QDialogButtonBox();
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);

    verticalLayout->addWidget(buttonBox);

    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    tabWidget->setCurrentIndex(0);
}

void ExprAddDialog::colorChooseClicked() {
    color = QColorDialog::getColor(color);
    if (color.isValid()) {
        QPixmap colorPix(30, 30);
        colorPix.fill(color);
        ((QPushButton*)sender())->setIcon(QIcon(colorPix));
    }
}

const char* ExprAddDialog::initSwatch() {
    if (rainbowPaletteBtn->isChecked())
        return ("[1,0,0],[1,.6,0],[1,1,0],[0,1,0],[0,1,1],[0,0,1],[.6,.1,.6],[1,0,1],[1,1,1],[0,0,0]");
    else if (grayPaletteBtn->isChecked())
        return (
            "[1,1,1],[.9,.9,.9],[.8,.8,.8],[.7,.7,.7],[.6,.6,.6],[.5,.5,.5],[.4,.4,.4],[.3,.3,.3],[.2,.2,.2],[0,0,0]");
    else
        return ("[1,1,1],[.5,.5,.5],[0,0,0]");
}
