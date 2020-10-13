// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: LicenseRef-Apache-2.0
/*
* @file ExprColorCurve.h
* @brief Contains PyQt4 Ramp Widget to emulate Maya's ramp widget
* @author Arthur Shek
* @version ashek     05/04/09  Initial Version
*/
#ifndef _ExprColorCurve_h_
#define _ExprColorCurve_h_

#include <vector>

#include <QObject>
#include <QComboBox>
#include <QGraphicsProxyWidget>
#include <QGraphicsView>
#include <QLineEdit>

#include <KSeExpr/Curve.h>
#include "ExprCurve.h"

/*
  This class overrides QGraphicsScene so we can handle mouse
  press, drag and keyboard events
*/
class CCurveScene : public QGraphicsScene {
    Q_OBJECT

    typedef KSeExpr::Curve<KSeExpr::Vec3d> T_CURVE;
    typedef T_CURVE::InterpType T_INTERP;

  public:
    CCurveScene();
    ~CCurveScene();

    void addPoint(double x, const KSeExpr::Vec3d y, const T_INTERP interp, const bool select = true);

    void removePoint(const int index);
    void removeAll();

    virtual void keyPressEvent(QKeyEvent *event);

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void drawRect();

    void drawPoints();

    QPixmap &getPixmap();
    void emitCurveChanged();

    void rebuildCurve();

    std::vector<T_CURVE::CV> _cvs;  // unsorted cvs

    friend class ExprColorCurve;

  public Q_SLOTS:
    void interpChanged(const int interp);
    void selPosChanged(double pos);
    void selValChanged(const KSeExpr::Vec3d &val);
    void resize(const int width, const int height);

  Q_SIGNALS:
    void cvSelected(double x, const KSeExpr::Vec3d y, const T_INTERP interp);
    void curveChanged();

  private:
    T_CURVE *_curve;

    QByteArray getCPixmap();

    int _width;
    int _height;
    KSeExpr::Vec3d _color;
    T_INTERP _interp;
    std::vector<QGraphicsEllipseItem *> _circleObjects;
    int _selectedItem;
    QPixmap _pixmap;
    bool _pixmapDirty;
    QWidget *_baseRectW;
    QGraphicsProxyWidget *_baseRect;
    bool _lmb;
};

class ExprCBoxWidget : public QWidget {
    Q_OBJECT
  public:
    ExprCBoxWidget(CCurveScene *curveScene, QWidget *parent = 0) : QWidget(parent), _curveScene(curveScene) {}
    ~ExprCBoxWidget() {}

  protected:
    virtual void paintEvent(QPaintEvent *event);

  private:
    CCurveScene *_curveScene;
};

class ExprCSwatchFrame : public QFrame {
    Q_OBJECT
  public:
    ExprCSwatchFrame(KSeExpr::Vec3d value, QWidget *parent = 0);
    ~ExprCSwatchFrame() {}

    void setValue(const KSeExpr::Vec3d &value);
    KSeExpr::Vec3d getValue() const;

  protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);

Q_SIGNALS:
    void selValChangedSignal(KSeExpr::Vec3d value);
    void swatchChanged(QColor color);

  private:
    KSeExpr::Vec3d _value;
    QColor _color;
};

class ExprColorCurve : public QWidget {
    Q_OBJECT

    typedef KSeExpr::Curve<KSeExpr::Vec3d> T_CURVE;
    typedef T_CURVE::InterpType T_INTERP;

  public:
    ExprColorCurve(QWidget *parent = 0,
                   QString pLabel = QString(),
                   QString vLabel = QString(),
                   QString iLabel = QString(),
                   bool expandable = true);
    ~ExprColorCurve() {}

    // Convenience Functions
    void addPoint(const double x, const KSeExpr::Vec3d y, const T_INTERP interp, bool select = false);
    void setSwatchColor(QColor color);
    QColor getSwatchColor();

    CCurveScene *_scene;

  public Q_SLOTS:
    void cvSelectedSlot(const double pos, const KSeExpr::Vec3d val, const T_INTERP interp);
    void selPosChanged();
    void openDetail();

  Q_SIGNALS:
    void selPosChangedSignal(double pos);
    void selValChangedSignal(KSeExpr::Vec3d val);
    void swatchChanged(QColor color);

  private Q_SLOTS:
    void internalSwatchChanged(QColor color);

  private:
    QLineEdit *_selPosEdit;
    ExprCSwatchFrame *_selValEdit;
    QComboBox *_interpComboBox;
};
#endif
