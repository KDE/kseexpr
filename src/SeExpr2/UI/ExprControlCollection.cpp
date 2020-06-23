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
* @file ExprControlCollection.cpp
* @brief Manages/creates a bunch of ExprControls by using expression text
* @author  aselle
*/

#include <QPushButton>

#include "Debug.h"
#include "Editable.h"
#include "ExprAddDialog.h"
#include "ExprControlCollection.h"

ExprControlCollection::ExprControlCollection(QWidget* parent, bool showAddButton)
    : QWidget(parent), count(0), showAddButton(showAddButton), editableExpression(0) {
    controlLayout = new QVBoxLayout();
    controlLayout->setMargin(0);
    controlLayout->setSpacing(0);
    controlLayout->insertStretch(-1, 100);

    if (showAddButton) {
        QPushButton* button = new QPushButton(tr("Add new variable"));
        button->setFocusPolicy(Qt::NoFocus);
        QHBoxLayout* buttonLayout = new QHBoxLayout();
        buttonLayout->insertStretch(-1, 100);
        buttonLayout->addWidget(button, 0);
        controlLayout->addLayout(buttonLayout);
        connect(button, SIGNAL(clicked()), SLOT(addControlDialog()));
    }
    setLayout(controlLayout);
}

ExprControlCollection::~ExprControlCollection() { delete editableExpression; }

void ExprControlCollection::addControlDialog() {
    ExprAddDialog* dialog = new ExprAddDialog(count, this);
    if (dialog->exec()) {
        QString s;
        switch (dialog->tabWidget->currentIndex()) {
            case 0:
                s = QString::fromLatin1("%1 = curve(%2,0,0,4,1,1,4);\n")
                        .arg(dialog->variableName->text())
                        .arg(dialog->curveLookup->text());
                break;
            case 1:
                s = QString::fromLatin1("%1 = ccurve(%2,0,[0,0,0],4,1,[1,1,1],4);\n")
                        .arg(dialog->variableName->text())
                        .arg(dialog->colorCurveLookup->text());
                break;
            case 2:
                s = QString::fromLatin1("%1 = %2; # %3,%4\n")
                    .arg(dialog->variableName->text())
                    .arg(dialog->intDefault->text())
                    .arg(dialog->intMin->text())
                    .arg(dialog->intMax->text());
                break;
            case 3:
                s = QString::fromLatin1("%1 = %2; # %3, %4\n")
                        .arg(dialog->variableName->text())
                        .arg(dialog->floatDefault->text())
                        .arg(crack_atof(dialog->floatMin->text().toStdString().c_str()), 0, 'f', 3)
                        .arg(crack_atof(dialog->floatMax->text().toStdString().c_str()), 0, 'f', 3);
                break;
            case 4:
                s = QString::fromLatin1("%1 = [%2,%3,%4]; # %5, %6\n")
                        .arg(dialog->variableName->text())
                        .arg(dialog->vectorDefault0->text())
                        .arg(dialog->vectorDefault1->text())
                        .arg(dialog->vectorDefault2->text())
                        .arg(crack_atof(dialog->vectorMin->text().toStdString().c_str()), 0, 'f', 3)
                        .arg(crack_atof(dialog->vectorMax->text().toStdString().c_str()), 0, 'f', 3);
                break;
            case 5:
                s = QString::fromLatin1("%1 = [%2,%3,%4];\n")
                        .arg(dialog->variableName->text())
                        .arg(dialog->color.redF())
                        .arg(dialog->color.greenF())
                        .arg(dialog->color.blueF());
                break;
            case 6:
                s = QString::fromLatin1("%1 = swatch(%2,%3);\n")
                        .arg(dialog->variableName->text())
                        .arg(dialog->swatchLookup->text())
                        .arg(QString::fromLatin1(dialog->initSwatch()));
                break;
            case 7:
                s = QString::fromLatin1("\"%1\" #%2 %3\n")
                        .arg(dialog->stringDefaultWidget->text())
                        .arg(dialog->stringTypeWidget->currentText())
                        .arg(dialog->stringNameWidget->text());
                break;
#ifdef SEEXPR_ENABLE_ANIMCURVE
            case 8:
                s = QString::fromLatin1("%1 = animCurve(%2,\"constant\",\"constant\",0,\"%3\");")
                        .arg(dialog->variableName->text())
                        .arg(dialog->animCurveLookup->text())
                        .arg(dialog->animCurveLink->text());
                break;
#endif
#ifdef SEEXPR_ENABLE_DEEPWATER
            case 9:
                s = QString::fromLatin1("%1 = deepWater(%2,9,30,0,1,0,5,0,0,[0,0,0],0,0,0);\n")
                        .arg(dialog->variableName->text())
                        .arg(dialog->deepWaterLookup->text());
                break;
#endif
        }
        emit insertString(s.toStdString());
    }
}

bool ExprControlCollection::rebuildControls(const QString& expressionText, std::vector<QString>& variables) {
    // parse a new editable expression so  we can check if we need to make new controls
    EditableExpression* newEditable = new EditableExpression;
    newEditable->setExpr(expressionText.toStdString());

    // check for new variables

    bool newVariables = true;
    if (editableExpression && editableExpression->getVariables() == newEditable->getVariables()) newVariables = false;
    if (newVariables) {
        const std::vector<std::string>& vars = newEditable->getVariables();
        variables.clear();
        for (size_t k = 0; k < vars.size(); k++) {
            variables.push_back(QString::fromLatin1("$%1").arg(QString::fromStdString(vars[k])));
        }
    }

    if (newEditable->size() == 0 && !editableExpression) return false;

    if (editableExpression && editableExpression->controlsMatch(*newEditable)) {
        // controls match so we only need to update positions (i.e. if the user typed and shifted some controls)
        editableExpression->updateString(*newEditable);
        delete newEditable;
    } else {
        // controls did not match

        // delete old controls
        for (unsigned int i = 0; i < _controls.size(); i++) {
            controlLayout->removeWidget(_controls[i]);
            delete _controls[i];
        }
        _linkedId = -1;
        _controls.clear();

        // swap to new editable expression
        delete editableExpression;
        editableExpression = newEditable;

        // build new controls
        for (size_t i = 0; i < editableExpression->size(); i++) {
            Editable* editable = (*editableExpression)[i];
            ExprControl* widget = 0;
            // Create control "factory" (but since its only used here...)
            if (NumberEditable* x = dynamic_cast<NumberEditable*>(editable))
                widget = new NumberControl(i, x);
            else if (VectorEditable* x = dynamic_cast<VectorEditable*>(editable))
                widget = new VectorControl(i, x);
            else if (StringEditable* x = dynamic_cast<StringEditable*>(editable))
                widget = new StringControl(i, x);
            else if (CurveEditable* x = dynamic_cast<CurveEditable*>(editable))
                widget = new CurveControl(i, x);
            else if (ColorCurveEditable* x = dynamic_cast<ColorCurveEditable*>(editable))
                widget = new CCurveControl(i, x);
#ifdef SEEXPR_ENABLE_ANIMCURVE
            else if (AnimCurveEditable* x = dynamic_cast<AnimCurveEditable*>(editable)) 
                widget = new AnimCurveControl(i, x);
#endif
            else if (ColorSwatchEditable* x = dynamic_cast<ColorSwatchEditable*>(editable))
                widget = new ColorSwatchControl(i, x);
#ifdef SEEXPR_ENABLE_DEEPWATER
            else if (DeepWaterEditable* x = dynamic_cast<DeepWaterEditable*>(editable))
                widget = new DeepWaterControl(i, x);
#endif
            else {
                dbgSeExpr << "SeExpr editor logic error, cannot find a widget for the given editable";
            }
            if (widget) {
                // successfully made widget
                int insertPoint = controlLayout->count() - 1;
                if (showAddButton) insertPoint--;
                controlLayout->insertWidget(insertPoint, widget);
                _controls.push_back(widget);
                connect(widget, SIGNAL(controlChanged(int)), SLOT(singleControlChanged(int)));
                connect(widget, SIGNAL(linkColorEdited(int, QColor)), SLOT(linkColorEdited(int, QColor)));
                connect(widget, SIGNAL(linkColorLink(int)), SLOT(linkColorLink(int)));
            } else {
                dbgSeExpr << "Expr Editor Logic ERROR did not make widget";
            }
        }
    }
    return newVariables;
}

void ExprControlCollection::showEditor(int idx) {
#ifdef SEEXPR_ENABLE_ANIMCURVE
    if (idx < 0 || idx >= (int)_controls.size()) return;

    /* Right now we only launch the anim curve editor.
     * It would be better to launch them generically. */
    AnimCurveControl* control = dynamic_cast<AnimCurveControl*>(_controls[idx]);
    if (!control) return;

    control->editGraphClicked();
#endif
}

void ExprControlCollection::linkColorLink(int id) {
    _linkedId = id;
    for (unsigned int i = 0; i < _controls.size(); i++) {
        _controls[i]->linkDisconnect(_linkedId);
    }
}

void ExprControlCollection::linkColorEdited(int id, QColor color) {
    if (id == _linkedId) emit linkColorOutput(color);
}

void ExprControlCollection::linkColorInput(QColor color) {
    // TODO: fix
    if (_linkedId < 0 || _linkedId >= (int)_controls.size()) return;
    _controls[_linkedId]->setColor(color);
}

void ExprControlCollection::updateText(const int id, QString& text) {
    Q_UNUSED(id);
    if (editableExpression) text = QString::fromStdString(editableExpression->getEditedExpr());
}

void ExprControlCollection::singleControlChanged(int id) { emit controlChanged(id); }
