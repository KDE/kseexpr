// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0

#ifndef _ExprColorSwatch_h_
#define _ExprColorSwatch_h_

#include <vector>
#include <QObject>
#include <QFrame>
#include <QWidget>
#include <KSeExpr/Vec.h>

class QGridLayout;

class ExprColorFrame : public QFrame {
    Q_OBJECT
  public:
    ExprColorFrame(KSeExpr::Vec3d value, QWidget *parent = 0);
    ~ExprColorFrame() {}

    void setValue(const KSeExpr::Vec3d &value);
    KSeExpr::Vec3d getValue() const;
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
    void selValChangedSignal(KSeExpr::Vec3d value);
    void swatchChanged(QColor color);
    void deleteSwatch(ExprColorFrame *swatch);

  private:
    KSeExpr::Vec3d _value;
    QColor _color;
    bool _selected;
};

// Simple color widget with or without index label
class ExprColorWidget : public QWidget {
    Q_OBJECT
  public:
    ExprColorWidget(KSeExpr::Vec3d value, int index, bool indexLabel, QWidget *parent);
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
    void addSwatch(KSeExpr::Vec3d &val, int index = -1);
    void setSwatchColor(int index, QColor color);
    QColor getSwatchColor(int index);

  private
Q_SLOTS:
    void addNewColor();
    void removeSwatch(ExprColorFrame *);
    void internalSwatchChanged(QColor color);

Q_SIGNALS:
    void selValChangedSignal(KSeExpr::Vec3d val);
    void swatchChanged(int index, KSeExpr::Vec3d val);
    void swatchAdded(int index, KSeExpr::Vec3d val);
    void swatchRemoved(int index);

  private:
    QGridLayout *_gridLayout;
    int _columns;
    bool _indexLabel;
};
#endif
