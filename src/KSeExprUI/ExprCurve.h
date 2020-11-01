// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later
/*
* @file ExprCurve.h
* @brief Contains PyQt4 Ramp Widget to emulate Maya's ramp widget
* @author Arthur Shek
* @version ashek     05/04/09  Initial Version
*/

#pragma once

#include <cstddef>
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
    ~CurveGraphicsView() override = default;

    void resizeEvent(QResizeEvent *event) override;

Q_SIGNALS:
    void resizeSignal(int width, int height);
};

/*
  This class overrides QGraphicsScene so we can handle mouse
  press, drag and keyboard events
*/
class CurveScene : public QGraphicsScene {
    Q_OBJECT

    using T_CURVE = KSeExpr::Curve<double>;
    using T_INTERP = T_CURVE::InterpType;

  public:
    CurveScene();
    ~CurveScene() override;

    void addPoint(double x, double y, T_INTERP interp, bool select = true);

    void removePoint(int index);
    void removeAll();

     void keyPressEvent(QKeyEvent *event) override;

     void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
     void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
     void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
     void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
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
    void interpChanged(int interp);
    void selPosChanged(double pos);
    void selValChanged(double val);
    void resize(int width, int height);

Q_SIGNALS:
    void cvSelected(double x, double y, T_INTERP interp);
    void curveChanged();

  private:
    int _width;
    int _height;
    T_INTERP _interp;
    std::vector<QGraphicsEllipseItem *> _circleObjects;
    int _selectedItem;
    QGraphicsPolygonItem *_curvePoly{nullptr};
    QGraphicsRectItem *_baseRect{nullptr};
    bool _lmb;
};

class ExprCurve : public QWidget {
    Q_OBJECT

    using T_CURVE = KSeExpr::Curve<double>;
    using T_INTERP = T_CURVE::InterpType;

  public:
    ExprCurve(QWidget *parent = nullptr,
              QString pLabel = QString(),
              QString vLabel = QString(),
              QString iLabel = QString(),
              bool expandable = true);
    ~ExprCurve() override = default;

    // Convenience Functions
    void addPoint(double x, double y, T_INTERP interp, bool select = false)
    {
        _scene->addPoint(x, y, interp, select);
    }

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
    QLineEdit *_selPosEdit{nullptr};
    QLineEdit *_selValEdit{nullptr};
    QComboBox *_interpComboBox{nullptr};
};
