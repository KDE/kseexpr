// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * @file ExprColorCurve.h
 * @brief Contains PyQt4 Ramp Widget to emulate Maya's ramp widget
 * @author Arthur Shek
 * @version ashek     05/04/09  Initial Version
 */

#pragma once

#include <vector>

#include <QComboBox>
#include <QGraphicsProxyWidget>
#include <QGraphicsView>
#include <QLineEdit>
#include <QObject>

#include <KSeExpr/Curve.h>

#include "ExprCurve.h"

/*
  This class overrides QGraphicsScene so we can handle mouse
  press, drag and keyboard events
*/
class CCurveScene : public QGraphicsScene
{
    Q_OBJECT

    using T_CURVE = KSeExpr::Curve<KSeExpr::Vec3d>;
    using T_INTERP = T_CURVE::InterpType;

public:
    CCurveScene();
    ~CCurveScene() override;

    void addPoint(double x, KSeExpr::Vec3d y, T_INTERP interp, bool select = true);

    void removePoint(int index);
    void removeAll();

    void keyPressEvent(QKeyEvent *event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    void drawRect();

    void drawPoints();

    QPixmap &getPixmap();
    void emitCurveChanged();

    void rebuildCurve();

    std::vector<T_CURVE::CV> _cvs; // unsorted cvs

    friend class ExprColorCurve;

public Q_SLOTS:
    void interpChanged(int interp);
    void selPosChanged(double pos);
    void selValChanged(const KSeExpr::Vec3d &val);
    void resize(int width, int height);

Q_SIGNALS:
    void cvSelected(double x, KSeExpr::Vec3d y, T_INTERP interp);
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

class ExprCBoxWidget : public QWidget
{
    Q_OBJECT
public:
    ExprCBoxWidget(CCurveScene *curveScene, QWidget *parent = 0)
        : QWidget(parent)
        , _curveScene(curveScene)
    {
    }
    ~ExprCBoxWidget() override = default;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    CCurveScene *_curveScene;
};

class ExprCSwatchFrame : public QFrame
{
    Q_OBJECT
public:
    ExprCSwatchFrame(KSeExpr::Vec3d value, QWidget *parent = nullptr);
    ~ExprCSwatchFrame() override = default;

    void setValue(const KSeExpr::Vec3d &value);
    KSeExpr::Vec3d getValue() const;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

Q_SIGNALS:
    void selValChangedSignal(KSeExpr::Vec3d value);
    void swatchChanged(QColor color);

private:
    KSeExpr::Vec3d _value;
    QColor _color;
};

class ExprColorCurve : public QWidget
{
    Q_OBJECT

    using T_CURVE = KSeExpr::Curve<KSeExpr::Vec3d>;
    using T_INTERP = T_CURVE::InterpType;

public:
    ExprColorCurve(QWidget *parent = nullptr, QString pLabel = QString(), QString vLabel = QString(), QString iLabel = QString(), bool expandable = true);
    
    ~ExprColorCurve() override = default;

    // Convenience Functions
    void addPoint(double x, KSeExpr::Vec3d y, T_INTERP interp, bool select = false);
    void setSwatchColor(QColor color);
    QColor getSwatchColor();

    CCurveScene *_scene;

public Q_SLOTS:
    void cvSelectedSlot(double pos, KSeExpr::Vec3d val, T_INTERP interp);
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
