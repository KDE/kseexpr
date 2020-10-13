// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: LicenseRef-Apache-2.0
#ifndef _ExprControl_h_
#define _ExprControl_h_

#include <memory>

#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>

#include "Editable.h"
#include "ExprCurve.h"
#include "ExprColorCurve.h"
#include "ExprColorSwatch.h"

typedef GenericCurveEditable<KSeExpr::Vec3d> ColorCurveEditable;
typedef GenericCurveEditable<double> CurveEditable;

/// Base class for all controls for Expressions
class ExprControl : public QWidget {
    Q_OBJECT;

  protected:
    int _id;
    bool _updating;  // whether to send events (i.e. masked when self editing)
    QHBoxLayout* hbox{nullptr};
    QCheckBox* _colorLinkCB{nullptr};
    QLabel* _label{nullptr};

    Editable* _editable{nullptr};

  public:
    ExprControl(int id, Editable* editable, bool showColorLink);
    virtual ~ExprControl() {}

    /// Interface for getting the color (used for linked color picking)
    virtual QColor getColor() { return QColor(); }
    /// Interface for setting the color (used for linked color picking)
    virtual void setColor(QColor color) {Q_UNUSED(color)};

Q_SIGNALS:
    // sends that the control has been changed to the control collection
    void controlChanged(int id);
    // sends the new color to the control collection
    void linkColorEdited(int id, QColor color);
    // sends that a color link is desired to the control collection
    void linkColorLink(int id);
  public
Q_SLOTS:
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
template <class T, class T2, class T3>
T clamp(const T val, const T2 minval, const T3 maxval) {
    if (val < minval)
        return minval;
    else if (val > maxval)
        return maxval;
    return val;
}

/// Line Editor Widget(used for numbers)
// TODO: can this now be removed?
class ExprLineEdit : public QLineEdit {
    Q_OBJECT
  public:
    ExprLineEdit(int id, QWidget* parent);
    virtual void setText(const QString& t) {
        if (_signaling) return;
        QLineEdit::setText(t);
    }

Q_SIGNALS:
    void textChanged(int id, const QString& text);

  private
Q_SLOTS:
    void textChangedCB(const QString& text);

  private:
    int _id;
    bool _signaling;
};

/// Generic Slider (used for int and float sliders)
class ExprSlider : public QSlider {
    Q_OBJECT
  public:
    ExprSlider(QWidget* parent = 0) : QSlider(parent) {}
    ExprSlider(Qt::Orientation orientation, QWidget* parent = 0) : QSlider(orientation, parent) {}
    virtual void mousePressEvent(QMouseEvent* e);
    virtual void mouseMoveEvent(QMouseEvent* e);
    virtual void paintEvent(QPaintEvent* e);
    virtual void leaveEvent(QEvent* event) {
        Q_UNUSED(event);
        update();
    }
    virtual void enterEvent(QEvent* event) {
        Q_UNUSED(event);
        update();
    }
    virtual void wheelEvent(QWheelEvent* e) { e->ignore(); }
};

/// Channel Slider (i.e. for colors)
class ExprChannelSlider : public QWidget {
    Q_OBJECT
  public:
    ExprChannelSlider(int id, QWidget* parent);
    virtual void paintEvent(QPaintEvent* e);
    virtual void mousePressEvent(QMouseEvent* e);
    virtual void mouseMoveEvent(QMouseEvent* e);
    virtual void wheelEvent(QWheelEvent* e) { e->ignore(); }
    float value() const { return _value; }
    void setDisplayColor(QColor c) { _col = c; }

  public
Q_SLOTS:
    void setValue(float value);

Q_SIGNALS:
    void valueChanged(int id, float value);

  private:
    int _id;
    float _value;
    QColor _col;
};

/// Number slider for either float or int data
class NumberControl : public ExprControl {
    Q_OBJECT

    /// Pointer to the number control model
    NumberEditable* _numberEditable{nullptr};
    /// Slider for the number
    ExprSlider* _slider{nullptr};
    /// Text box for the number
    ExprLineEdit* _edit{nullptr};

  public:
    NumberControl(int id, NumberEditable* number);

  private:
    /// Update the model with the value and notify the collection
    void setValue(float value);
    /// Update values in slider and textbox  given what the model contains
    void updateControl();
  private
Q_SLOTS:
    void sliderChanged(int val);
    void editChanged(int id, const QString& text);
};

/// A vector or color control (named vector because it edits a KSeExpr::Vec3d literal)
class VectorControl : public ExprControl {
    Q_OBJECT

    /// Number model
    VectorEditable* _numberEditable{nullptr};
    /// All three line edit widgets (for each component)
    ExprLineEdit* _edits[3]{nullptr};
    ExprCSwatchFrame* _swatch{nullptr};
    ;
    /// All three channel sliders (for each component)
    ExprChannelSlider* _sliders[3]{nullptr};

  public:
    VectorControl(int id, VectorEditable* number);

    QColor getColor();
    void setColor(QColor color);

  private:
    /// set the value in the model (in response to editing from controls)
    void setValue(int id, float value);
    /// update the individual slider and eidt box controls
    void updateControl();
  private
Q_SLOTS:
    void sliderChanged(int id, float val);
    void editChanged(int id, const QString& text);
    void swatchChanged(QColor color);
};

/// A control for editing strings, filenames, and directories
class StringControl : public ExprControl {
    Q_OBJECT

    /// model for the string control
    StringEditable* _stringEditable{nullptr};
    /// Edit box for the string
    QLineEdit* _edit{nullptr};

  public:
    StringControl(int id, StringEditable* stringEditable);

  private:
    void updateControl();
  private
Q_SLOTS:
    void textChanged(const QString& newText);
    void fileBrowse();
    void directoryBrowse();
};

/// Control for editing a normal curve ramp
class CurveControl : public ExprControl {
    Q_OBJECT

    /// curve model
    CurveEditable* _curveEditable{nullptr};
    /// curve edit widget
    ExprCurve* _curve{nullptr};

  public:
    CurveControl(int id, CurveEditable* stringEditable);
  private
Q_SLOTS:
    void curveChanged();
};

/// Control for editing a color ramp curve
class CCurveControl : public ExprControl {
    Q_OBJECT

    /// color curve model
    ColorCurveEditable* _curveEditable{nullptr};
    /// color curve widget
    ExprColorCurve* _curve{nullptr};

  public:
    CCurveControl(int id, ColorCurveEditable* stringEditable);
    QColor getColor();
    void setColor(QColor color);
  private
Q_SLOTS:
    void curveChanged();
};

/// A control for editing color swatches
class ColorSwatchControl : public ExprControl {
    Q_OBJECT

    /// model for the color swatches control
    ColorSwatchEditable* _swatchEditable{nullptr};
    /// Edit box for the color swatches
    ExprColorSwatchWidget* _swatch{nullptr};

  public:
    ColorSwatchControl(int id, ColorSwatchEditable* swatchEditable);
  private
Q_SLOTS:
    void buildSwatchWidget();
    void colorChanged(int index, KSeExpr::Vec3d value);
    void colorAdded(int index, KSeExpr::Vec3d value);
    void colorRemoved(int index);

  private:
    bool _indexLabel;
};

#endif
