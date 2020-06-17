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
*/

#include <QColorDialog>
#include <QDoubleValidator>
#include <QGraphicsSceneMouseEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QResizeEvent>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QPainter>
#include <QMenu>
#include <QLabel>
#include <QToolButton>

#include <SeExpr2/ExprBuiltins.h>
#ifdef SEEXPR_USE_QDGUI
#include <qdgui/QdColorPickerDialog.h>
#endif

#include "ExprColorSwatch.h"

// Simple color frame for swatch
ExprColorFrame::ExprColorFrame(SeExpr2::Vec3d value, QWidget *parent) : QFrame(parent), _value(value) {
    setValue(_value);
    setFrameStyle(QFrame::Box | QFrame::Plain);
    QPalette pal = palette();
    pal.setColor(backgroundRole(), pal.highlight().color());
    setPalette(pal);
    setAutoFillBackground(true);
}

void ExprColorFrame::setValue(const SeExpr2::Vec3d &value) {
    _color = QColor(int(255 * value[0] + 0.5), int(255 * value[1] + 0.5), int(255 * value[2] + 0.5));
    _value = value;
    update();
}

SeExpr2::Vec3d ExprColorFrame::getValue() const { return _value; }

void ExprColorFrame::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter p(this);
    p.fillRect(contentsRect(), _color);
}

void ExprColorFrame::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton)
        deleteSwatchMenu(event->pos());
    else {

#ifdef SEEXPR_USE_QDGUI
        QColor color = QdColorPickerDialog::chooseColorFromDialog(_color, this);
#else
        QColor color = QColorDialog::getColor(_color);
#endif

        if (color.isValid()) {
            _value[0] = color.red() / 255.0;
            _value[1] = color.green() / 255.0;
            _value[2] = color.blue() / 255.0;
            update();
            _color = color;
            emit selValChangedSignal(_value);
            emit swatchChanged(color);
        }
    }
}

void ExprColorFrame::deleteSwatchMenu(const QPoint &pos) {
    QMenu *menu = new QMenu(this);
    QAction *deleteAction = menu->addAction(tr("Delete Swatch"));
    menu->addAction(tr("Cancel"));
    QAction *action = menu->exec(mapToGlobal(pos));
    if (action == deleteAction) emit deleteSwatch(this);
}

// Simple color widget with or without index label
ExprColorWidget::ExprColorWidget(SeExpr2::Vec3d value, int index, bool indexLabel, QWidget *parent) : QWidget(parent) {
    _colorFrame = new ExprColorFrame(value);
    _colorFrame->setFixedWidth(32);
    _colorFrame->setFixedHeight(16);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->setContentsMargins(0, 0, 0, 0);
    vbox->setSpacing(0);
    vbox->addWidget(_colorFrame);

    if (indexLabel) {
        QLabel *label = new QLabel(tr("%1").arg(index));
        vbox->addWidget(label);
    }

    setLayout(vbox);
    // emit swatchAdded(index, val);
}

ExprColorFrame *ExprColorWidget::getColorFrame() { return _colorFrame; }

// Grid layout of color swatches
ExprColorSwatchWidget::ExprColorSwatchWidget(bool indexLabel, QWidget *parent)
    : QWidget(parent), _columns(8), _indexLabel(indexLabel) {
    QHBoxLayout *hboxLayout = new QHBoxLayout();
    hboxLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(hboxLayout);

    QToolButton *addBtn = new QToolButton;
    addBtn->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    QAction *detailAction = new QAction(tr("&Add..."));
    detailAction->setIcon(QIcon::fromTheme("list-add"));
    addBtn->setDefaultAction(detailAction);
    QVBoxLayout *swatchControlLayout = new QVBoxLayout();
    swatchControlLayout->setContentsMargins(0, 0, 0, 0);
    swatchControlLayout->setAlignment(Qt::AlignLeft | Qt::AlignCenter);
    QHBoxLayout *addRemoveBtnLayout = new QHBoxLayout();
    addRemoveBtnLayout->setContentsMargins(0, 0, 0, 0);
    addRemoveBtnLayout->setSpacing(0);
    addRemoveBtnLayout->addWidget(addBtn);
    swatchControlLayout->addLayout(addRemoveBtnLayout);
    swatchControlLayout->addStretch();

    QHBoxLayout *paletteLayout = new QHBoxLayout();
    paletteLayout->setContentsMargins(0, 0, 0, 0);
    QWidget *colorGrid = new QWidget();
    colorGrid->setMinimumWidth(256);
    _gridLayout = new QGridLayout();
    _gridLayout->setContentsMargins(0, 0, 0, 0);
    _gridLayout->setSpacing(0);
    paletteLayout->addLayout(_gridLayout);
    paletteLayout->addStretch();
    colorGrid->setLayout(paletteLayout);

    hboxLayout->addWidget(colorGrid);
    hboxLayout->addLayout(swatchControlLayout);
    hboxLayout->addStretch();

    // SIGNALS
    connect(addBtn, SIGNAL(triggered(QAction *)), this, SLOT(addNewColor()));
}

void ExprColorSwatchWidget::addNewColor() {
    SeExpr2::Vec3d val(.5);
    addSwatch(val, -1);
}

void ExprColorSwatchWidget::addSwatch(SeExpr2::Vec3d &val, int index) {
    if (index == -1 || index > _gridLayout->count()) index = _gridLayout->count();
    ExprColorWidget *widget = new ExprColorWidget(val, index, _indexLabel, this);
    ExprColorFrame *swatchFrame = widget->getColorFrame();
    _gridLayout->addWidget(widget, index / _columns, index % _columns);
    connect(swatchFrame, SIGNAL(swatchChanged(QColor)), this, SLOT(internalSwatchChanged(QColor)));
    connect(swatchFrame, SIGNAL(deleteSwatch(ExprColorFrame *)), this, SLOT(removeSwatch(ExprColorFrame *)));
    emit swatchAdded(index, val);
}

void ExprColorSwatchWidget::internalSwatchChanged(QColor newcolor) {
    Q_UNUSED(newcolor);
    ExprColorFrame *swatchFrame = (ExprColorFrame *)sender();
    SeExpr2::Vec3d value = swatchFrame->getValue();
    int index = _gridLayout->indexOf(swatchFrame->parentWidget());
    emit swatchChanged(index, value);
}

void ExprColorSwatchWidget::removeSwatch(ExprColorFrame *widget) {
    QWidget *parentWidget = widget->parentWidget();
    // Find given widget to remove from grid layout
    for (int i = 0; i < _gridLayout->count(); i++) {
        if (_gridLayout->itemAt(i)->widget() == parentWidget) {
            _gridLayout->removeWidget(parentWidget);
            parentWidget->deleteLater();
            emit swatchRemoved(i);
            break;
        }
    }
}

void ExprColorSwatchWidget::setSwatchColor(int index, QColor color) {
    if (index >= 0 && index < _gridLayout->count()) {
        SeExpr2::Vec3d newColor(color.redF(), color.greenF(), color.blueF());
        QLayoutItem *layoutItem = _gridLayout->itemAt(index);
        if (layoutItem && layoutItem->widget()) {
            QWidget *widget = layoutItem->widget();
            ExprColorFrame *cFrame = ((ExprColorWidget *)widget)->getColorFrame();
            cFrame->setValue(newColor);
        }
    }
}

QColor ExprColorSwatchWidget::getSwatchColor(int index) {
    if (index >= 0 && index < _gridLayout->count()) {
        QLayoutItem *layoutItem = _gridLayout->itemAt(index);
        if (layoutItem && layoutItem->widget()) {
            QWidget *widget = layoutItem->widget();
            ExprColorFrame *cFrame = ((ExprColorWidget *)widget)->getColorFrame();
            SeExpr2::Vec3d val = cFrame->getValue();
            return QColor::fromRgbF(val[0], val[1], val[2], 1);
        }
    }
    return QColor();
}
