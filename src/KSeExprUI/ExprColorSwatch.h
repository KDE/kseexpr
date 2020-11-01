// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <KSeExpr/Vec.h>
#include <QFrame>
#include <QObject>
#include <QWidget>
#include <vector>

class QGridLayout;

class ExprColorFrame : public QFrame
{
    Q_OBJECT
public:
    ExprColorFrame(KSeExpr::Vec3d value, QWidget *parent = nullptr);
    ~ExprColorFrame() override = default;

    void setValue(const KSeExpr::Vec3d &value);
    KSeExpr::Vec3d getValue() const;
    bool selected() const
    {
        return _selected;
    };

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private Q_SLOTS:
    void deleteSwatchMenu(const QPoint &pos);

Q_SIGNALS:
    void selValChangedSignal(KSeExpr::Vec3d value);
    void swatchChanged(QColor color);
    void deleteSwatch(ExprColorFrame *swatch);

private:
    KSeExpr::Vec3d _value {};
    QColor _color {};
    bool _selected {};
};

// Simple color widget with or without index label
class ExprColorWidget : public QWidget
{
    Q_OBJECT
public:
    ExprColorWidget(KSeExpr::Vec3d value, int index, bool indexLabel, QWidget *parent);
    ExprColorFrame *getColorFrame();

private:
    ExprColorFrame *_colorFrame;
};

class ExprColorSwatchWidget : public QWidget
{
    Q_OBJECT

public:
    ExprColorSwatchWidget(bool indexLabel, QWidget *parent = nullptr);
    ~ExprColorSwatchWidget() override = default;

    // Convenience Functions
    void addSwatch(KSeExpr::Vec3d &val, int index = -1);
    void setSwatchColor(int index, QColor color);
    QColor getSwatchColor(int index);

private Q_SLOTS:
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
