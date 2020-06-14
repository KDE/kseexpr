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
*/

#ifndef _ExprColorSwatch_h_
#define _ExprColorSwatch_h_

#include <vector>
#include <QObject>
#include <QFrame>
#include <QWidget>
#include <SeExpr2/Vec.h>

class QGridLayout;

class ExprColorFrame : public QFrame {
    Q_OBJECT
  public:
    ExprColorFrame(SeExpr2::Vec3d value, QWidget *parent = 0);
    ~ExprColorFrame() {}

    void setValue(const SeExpr2::Vec3d &value);
    SeExpr2::Vec3d getValue() const;
    bool selected() {
        return _selected;
    };

  protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

  private
Q_SLOTS:
    void deleteSwatchMenu(const QPoint &pos);

Q_SIGNALS:
    void selValChangedSignal(SeExpr2::Vec3d value);
    void swatchChanged(QColor color);
    void deleteSwatch(ExprColorFrame *swatch);

  private:
    SeExpr2::Vec3d _value;
    QColor _color;
    bool _selected;
};

// Simple color widget with or without index label
class ExprColorWidget : public QWidget {
    Q_OBJECT
  public:
    ExprColorWidget(SeExpr2::Vec3d value, int index, bool indexLabel, QWidget *parent);
    ExprColorFrame *getColorFrame();

  private:
    ExprColorFrame *_colorFrame;
};

class ExprColorSwatchWidget : public QWidget {
    Q_OBJECT

  public:
    ExprColorSwatchWidget(bool indexLabel, QWidget *parent = 0);
    ~ExprColorSwatchWidget() {}

    // Convenience Functions
    void addSwatch(SeExpr2::Vec3d &val, int index = -1);
    void setSwatchColor(int index, QColor color);
    QColor getSwatchColor(int index);

  private
Q_SLOTS:
    void addNewColor();
    void removeSwatch(ExprColorFrame *);
    void internalSwatchChanged(QColor color);

Q_SIGNALS:
    void selValChangedSignal(SeExpr2::Vec3d val);
    void swatchChanged(int index, SeExpr2::Vec3d val);
    void swatchAdded(int index, SeExpr2::Vec3d val);
    void swatchRemoved(int index);

  private:
    QGridLayout *_gridLayout;
    int _columns;
    bool _indexLabel;
};
#endif
