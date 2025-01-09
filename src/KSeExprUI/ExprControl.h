// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <array>
#include <atomic>
#include <cstddef>
#include <memory>

#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QEnterEvent>

#include "Editable.h"
#include "ExprColorCurve.h"
#include "ExprColorSwatch.h"
#include "ExprCurve.h"


/// Base class for all controls for Expressions
class ExprControl : public QWidget
{
    Q_OBJECT

protected:
    int _id;
    std::atomic<bool> _updating {}; // whether to send events (i.e. masked when self editing)
    QHBoxLayout *hbox {nullptr};
    QCheckBox *_colorLinkCB {nullptr};
    QLabel *_label {nullptr};

    Editable *_editable {nullptr};

public:
    ExprControl(int id, Editable *editable, bool showColorLink);
    ~ExprControl() override = default;

    /// Interface for getting the color (used for linked color picking)
    virtual QColor getColor()
    {
        return {};
    }
    /// Interface for setting the color (used for linked color picking)
    virtual void setColor(QColor) {};

Q_SIGNALS:
    // sends that the control has been changed to the control collection
    void controlChanged(int id);
    // sends the new color to the control collection
    void linkColorEdited(int id, QColor color);
    // sends that a color link is desired to the control collection
    void linkColorLink(int id);
public Q_SLOTS:
    // receives that the link should be changed to the given state (0=off,1=on)
    void linkStateChange(int state);

public:
    // notifies this that the link should be disconnected
    void linkDisconnect(int newId);

protected:
    // Allows to adapt the widget contents - amyspark
    void resizeEvent(QResizeEvent *event) override;
};

/// clamp val to the specified range [minval,maxval]
template<class T, class T2, class T3> T clamp(const T val, const T2 minval, const T3 maxval)
{
    assert(!(maxval < minval));
    if (val < minval)
        return minval;
    else if (val > maxval)
        return maxval;
    return val;
}

/// Line Editor Widget(used for numbers)
// TODO: can this now be removed?
class ExprLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    ExprLineEdit(int id, QWidget *parent);
    virtual void setText(const QString &t)
    {
        if (_signaling)
            return;
        QLineEdit::setText(t);
    }

Q_SIGNALS:
    void textChanged(int id, const QString &text);

private Q_SLOTS:
    void textChangedCB(const QString &text);

private:
    int _id;
    std::atomic<bool> _signaling{false};
};

/// Generic Slider (used for int and float sliders)
class ExprSlider : public QSlider
{
    Q_OBJECT
public:
    ExprSlider(QWidget *parent = nullptr)
        : QSlider(parent)
    {
    }
    ExprSlider(Qt::Orientation orientation, QWidget *parent = nullptr)
        : QSlider(orientation, parent)
    {
    }
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void paintEvent(QPaintEvent *e) override;
    void leaveEvent(QEvent *) override
    {
        update();
    }
    void enterEvent(QEnterEvent *) override
    {
        update();
    }
    void wheelEvent(QWheelEvent *e) override
    {
        e->ignore();
    }
};

/// Channel Slider (i.e. for colors)
class ExprChannelSlider : public QWidget
{
    Q_OBJECT
public:
    ExprChannelSlider(int id, QWidget *parent);
    void paintEvent(QPaintEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *e) override
    {
        e->ignore();
    }
    double value() const
    {
        return _value;
    }
    void setDisplayColor(QColor c)
    {
        _col = c;
    }

public Q_SLOTS:
    void setValue(double value);

Q_SIGNALS:
    void valueChanged(int id, double value);

private:
    int _id;
    double _value;
    QColor _col;
};

/// Number slider for either float or int data
class NumberControl : public ExprControl
{
    Q_OBJECT

    /// Pointer to the number control model
    NumberEditable *_numberEditable {nullptr};
    /// Slider for the number
    ExprSlider *_slider {nullptr};
    /// Text box for the number
    ExprLineEdit *_edit {nullptr};

public:
    NumberControl(int id, NumberEditable *editable);

private:
    /// Update the model with the value and notify the collection
    void setValue(double value);
    /// Update values in slider and textbox  given what the model contains
    void updateControl();
private Q_SLOTS:
    void sliderChanged(int val);
    void editChanged(int id, const QString &text);
};

/// A vector or color control (named vector because it edits a KSeExpr::Vec3d literal)
class VectorControl : public ExprControl
{
    Q_OBJECT

    /// Number model
    VectorEditable *_numberEditable {nullptr};
    /// All three line edit widgets (for each component)
    std::array<ExprLineEdit *, 3> _edits{};
    ExprCSwatchFrame *_swatch {nullptr};

    /// All three channel sliders (for each component)
    std::array<ExprChannelSlider *, 3> _sliders{};

public:
    VectorControl(int id, VectorEditable *editable);

    QColor getColor() override;
    void setColor(QColor color) override;

private:
    /// set the value in the model (in response to editing from controls)
    void setValue(int n, double value);
    /// update the individual slider and eidt box controls
    void updateControl();
private Q_SLOTS:
    void sliderChanged(int id, double val);
    void editChanged(int id, const QString &text);
    void swatchChanged(QColor color);
};

/// A control for editing strings, filenames, and directories
class StringControl : public ExprControl
{
    Q_OBJECT

    /// model for the string control
    StringEditable *_stringEditable {nullptr};
    /// Edit box for the string
    QLineEdit *_edit {nullptr};

public:
    StringControl(int id, StringEditable *stringEditable);

private:
    void updateControl();
private Q_SLOTS:
    void textChanged(const QString &newText);
    void fileBrowse();
    void directoryBrowse();
};

/// Control for editing a normal curve ramp
class CurveControl : public ExprControl
{
    Q_OBJECT

    /// curve model
    CurveEditable *_curveEditable {nullptr};
    /// curve edit widget
    ExprCurve *_curve {nullptr};

public:
    CurveControl(int id, CurveEditable *stringEditable);
private Q_SLOTS:
    void curveChanged();
};

/// Control for editing a color ramp curve
class CCurveControl : public ExprControl
{
    Q_OBJECT

    /// color curve model
    ColorCurveEditable *_curveEditable {nullptr};
    /// color curve widget
    ExprColorCurve *_curve {nullptr};

public:
    CCurveControl(int id, ColorCurveEditable *stringEditable);
    QColor getColor() override;
    void setColor(QColor color) override;
private Q_SLOTS:
    void curveChanged();
};

/// A control for editing color swatches
class ColorSwatchControl : public ExprControl
{
    Q_OBJECT

    /// model for the color swatches control
    ColorSwatchEditable *_swatchEditable {nullptr};
    /// Edit box for the color swatches
    ExprColorSwatchWidget *_swatch {nullptr};

public:
    ColorSwatchControl(int id, ColorSwatchEditable *swatchEditable);
private Q_SLOTS:
    void buildSwatchWidget();
    void colorChanged(int index, KSeExpr::Vec3d value);
    void colorAdded(int index, KSeExpr::Vec3d value);
    void colorRemoved(int index);

private:
    bool _indexLabel;
};
