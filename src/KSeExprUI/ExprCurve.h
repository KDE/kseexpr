// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: LicenseRef-Apache-2.0
/*
* @file ExprCurve.h
* @brief Contains PyQt4 Ramp Widget to emulate Maya's ramp widget
* @author Arthur Shek
* @version ashek     05/04/09  Initial Version
*/
#ifndef _ExprCurve_h_
#define _ExprCurve_h_

#include <vector>

#include <QObject>
#include <QComboBox>
#include <QGraphicsPolygonItem>
#include <QGraphicsView>
#include <QLineEdit>

#include <KSeExpr/Curve.h>

/*
  This class overrides QGraphicsView so we can get resize events
*/
class CurveGraphicsView : public QGraphicsView {
    Q_OBJECT
  public:
    CurveGraphicsView() {
        setTransformationAnchor(QGraphicsView::NoAnchor);
        setResizeAnchor(QGraphicsView::NoAnchor);
    }
    ~CurveGraphicsView() {}

    virtual void resizeEvent(QResizeEvent *event);

Q_SIGNALS:
    void resizeSignal(int width, int height);
};

/*
  This class overrides QGraphicsScene so we can handle mouse
  press, drag and keyboard events
*/
class CurveScene : public QGraphicsScene {
    Q_OBJECT

    typedef KSeExpr::Curve<double> T_CURVE;
    typedef T_CURVE::InterpType T_INTERP;

  public:
    CurveScene();
    ~CurveScene();

    void addPoint(double x, double y, const T_INTERP interp, const bool select = true);

    void removePoint(const int index);
    void removeAll();

    virtual void keyPressEvent(QKeyEvent *event);

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void drawRect();

    void drawPoly();

    void drawPoints();

    void emitCurveChanged();

    void rebuildCurve();

    std::vector<T_CURVE::CV> _cvs;  // unsorted cvs

    friend class ExprCurve;

  private:
    T_CURVE *_curve;
  public
Q_SLOTS:
    void interpChanged(const int interp);
    void selPosChanged(double pos);
    void selValChanged(double val);
    void resize(const int width, const int height);

Q_SIGNALS:
    void cvSelected(double x, double y, T_INTERP interp);
    void curveChanged();

  private:
    int _width;
    int _height;
    T_INTERP _interp;
    std::vector<QGraphicsEllipseItem *> _circleObjects;
    int _selectedItem;
    QGraphicsPolygonItem *_curvePoly;
    QGraphicsRectItem *_baseRect;
    bool _lmb;
};

class ExprCurve : public QWidget {
    Q_OBJECT

    typedef KSeExpr::Curve<double> T_CURVE;
    typedef T_CURVE::InterpType T_INTERP;

  public:
    ExprCurve(QWidget *parent = 0,
              QString pLabel = QString(),
              QString vLabel = QString(),
              QString iLabel = QString(),
              bool expandable = true);
    ~ExprCurve() {}

    // Convenience Functions
    void addPoint(const double x, const double y, const T_INTERP interp, bool select = false);

    CurveScene *_scene;

  public
Q_SLOTS:
    void cvSelectedSlot(double pos, double val, T_INTERP interp);
    void selPosChanged();
    void selValChanged();
    void openDetail();

Q_SIGNALS:
    void selPosChangedSignal(double pos);
    void selValChangedSignal(double val);

  private:
    QLineEdit *_selPosEdit;
    QLineEdit *_selValEdit;
    QComboBox *_interpComboBox;
};
#endif
