// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: LicenseRef-Apache-2.0
/*
* @file ExprColorCurveUI.cpp
* @brief Contains PyQt4 Ramp Widget to emulate Maya's ramp widget
* @author Arthur Shek
* @version ashek     05/04/09  Initial Version
*/
#include <iostream>
#include <algorithm>

#include <QColorDialog>
#include <QDoubleValidator>
#include <QFormLayout>
#include <QToolButton>
#include <QGraphicsSceneMouseEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QResizeEvent>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QMenu>

#include <KSeExpr/ExprBuiltins.h>

#include "ExprColorCurve.h"

CCurveScene::CCurveScene()
    : _curve(new T_CURVE), _width(320), _height(50), _color(KSeExpr::Vec3d(.5)), _interp(T_CURVE::kMonotoneSpline),
      _selectedItem(-1), _pixmapDirty(true), _baseRectW(0), _baseRect(0), _lmb(false) {
    rebuildCurve();
    resize(_width, _height);
}

CCurveScene::~CCurveScene() { delete _curve; }

void CCurveScene::resize(const int width, const int height) {
    // width and height already have the 8 px padding factored in
    // MAKE SURE THIS NEVER UNDERFLOWS THE PIXMAPSIZE -- amyspark
    _width = std::max(width - 16, 1);
    _height = std::max(height - 16, 1);
    setSceneRect(-9, -2, width, height);
    drawRect();
    drawPoints();
    _pixmap = QPixmap(_width, _height);
    _pixmapDirty = true;
}

void CCurveScene::rebuildCurve() {
    delete _curve;
    _curve = new T_CURVE;
    for (unsigned int i = 0; i < _cvs.size(); i++) _curve->addPoint(_cvs[i]._pos, _cvs[i]._val, _cvs[i]._interp);
    _curve->preparePoints();
}

void CCurveScene::addPoint(double x, const KSeExpr::Vec3d y, const T_INTERP interp, const bool select) {
    x = KSeExpr::clamp(x, 0, 1);

    _cvs.push_back(T_CURVE::CV(x, y, T_INTERP(interp)));
    int newIndex = _cvs.size() - 1;

    rebuildCurve();

    if (select) {
        _selectedItem = newIndex;
        emit cvSelected(x, y, interp);
    }
    _pixmapDirty = true;
    _baseRectW->update();
    drawPoints();
}

void CCurveScene::removePoint(const int index) {
    _cvs.erase(_cvs.begin() + index);
    _selectedItem = -1;
    rebuildCurve();

    _pixmapDirty = true;
    _baseRectW->update();
    drawPoints();
    emitCurveChanged();
}

void CCurveScene::removeAll() { _cvs.clear(); }

void CCurveScene::keyPressEvent(QKeyEvent *event) {
    if (((event->key() == Qt::Key_Backspace) || (event->key() == Qt::Key_Delete)) && (_selectedItem >= 0)) {
        // user hit delete with cv selected
        removePoint(_selectedItem);
    }
}

void CCurveScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    _lmb = true;
    QPointF pos = mouseEvent->scenePos();
    // get items under mouse click
    QList<QGraphicsItem *> itemList = items(pos);
    if (itemList.empty()) {
        _selectedItem = -1;
        emit cvSelected(-1, KSeExpr::Vec3d(0.0), _interp);
        drawPoints();
    } else if (itemList[0]->zValue() == 2) {
        // getting here means we've selected a current point
        const int numCircle = _circleObjects.size();
        for (int i = 0; i < numCircle; i++) {
            QGraphicsItem *obj = _circleObjects[i];
            if (obj == itemList[0]) {
                _selectedItem = i;
                _color = _cvs[i]._val;
                _interp = _cvs[i]._interp;
                emit cvSelected(_cvs[i]._pos, _cvs[i]._val, _cvs[i]._interp);
            }
        }
        drawPoints();
    } else {
        if (mouseEvent->buttons() == Qt::LeftButton) {
            // getting here means we want to create a new point
            double myx = pos.x() / _width;
            T_INTERP interpFromNearby = _curve->getLowerBoundCV(KSeExpr::clamp(myx, 0, 1))._interp;
            if (interpFromNearby == T_CURVE::kNone) interpFromNearby = T_CURVE::kMonotoneSpline;
            addPoint(myx, _curve->getValue(myx), interpFromNearby);
            emitCurveChanged();
        } else {
            _selectedItem = -1;
            drawPoints();
        }
    }
}

void CCurveScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    if (_lmb) {
        QPointF point = mouseEvent->scenePos();
        if (_selectedItem >= 0) {
            // clamp motion to inside curve area
            double pos = KSeExpr::clamp(point.x() / _width, 0, 1);
            _cvs[_selectedItem]._pos = pos;
            rebuildCurve();
            _pixmapDirty = true;
            _baseRectW->update();
            emit cvSelected(pos, _cvs[_selectedItem]._val, _cvs[_selectedItem]._interp);
            drawPoints();
            emitCurveChanged();
        }
    }
}

void CCurveScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
    if (_selectedItem >= 0) {
        QMenu *menu = new QMenu(event->widget());
        QAction *deleteAction = menu->addAction(tr("Delete Point"));
        QAction *action = menu->exec(event->screenPos());
        if (action == deleteAction) removePoint(_selectedItem);
    }
}

void CCurveScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    Q_UNUSED(mouseEvent);
    _lmb = false;
}

// user selected a different interpolation type, redraw
void CCurveScene::interpChanged(const int interp) {
    _interp = (T_INTERP)interp;
    if (_selectedItem >= 0) {
        _cvs[_selectedItem]._interp = _interp;
        rebuildCurve();
        _pixmapDirty = true;
        _baseRectW->update();
        emitCurveChanged();
    }
}

// user entered a different point position, redraw
void CCurveScene::selPosChanged(double pos) {
    if (_selectedItem >= 0) {
        pos = KSeExpr::clamp(pos, 0, 1);
        _cvs[_selectedItem]._pos = pos;
        rebuildCurve();
        _pixmapDirty = true;
        _baseRectW->update();
        drawPoints();
        emitCurveChanged();
    }
}

// user entered a different point value, redraw
void CCurveScene::selValChanged(const KSeExpr::Vec3d &val) {
    _color = val;
    if (_selectedItem >= 0) {
        _cvs[_selectedItem]._val = val;
        rebuildCurve();
        _pixmapDirty = true;
        _baseRectW->update();
        drawPoints();
        emitCurveChanged();
    }
}

// return points in reverse order in order to use same parsing in editor
void CCurveScene::emitCurveChanged() { emit curveChanged(); }

QPixmap &CCurveScene::getPixmap() {
    if (_pixmapDirty) {
        QByteArray buf;
        buf.append(QString::fromLatin1("P6\n%1 %2\n255\n").arg(_width).arg(_height));
        buf.append(getCPixmap());
        _pixmap.loadFromData(buf, "PPM");
        _pixmapDirty = false;
    }
    return _pixmap;
}

QByteArray CCurveScene::getCPixmap() {
    // create pixmap, set to gray
    const int len = 3 * _width * _height;
    QByteArray pixmap(len, 127);

    double paramInc = 1.0 / (_width - 2);
    double param = 0.5 * paramInc;  // start at pixel center
    // add black lines to left
    char *ptr = pixmap.data();
    *ptr++ = 0;
    *ptr++ = 0;
    *ptr++ = 0;
    for (int i = 1; i < _width - 1; i++) {
        KSeExpr::Vec3d color = _curve->getValue(param);
        *ptr++ = char(std::min(std::max(0.0, 255 * color[0]), 255.0) + 0.5);
        *ptr++ = char(std::min(std::max(0.0, 255 * color[1]), 255.0) + 0.5);
        *ptr++ = char(std::min(std::max(0.0, 255 * color[2]), 255.0) + 0.5);
        param += paramInc;
    }
    // add black lines to right
    *ptr++ = 0;
    *ptr++ = 0;
    *ptr++ = 0;

    for (int i = 1; i < _height - 1; i++) {
        memcpy(pixmap.data() + (i * _width * 3), pixmap.data() + ((i - 1) * _width * 3), _width * 3);
    }

    // add black lines to top and bottom
    memset(pixmap.data(), 0, _width * 3);
    memset(pixmap.data() + ((_height - 1) * _width * 3), 0, _width * 3);

    return pixmap;
}

// draws the base gray outline rectangle
void CCurveScene::drawRect() {
    if (_baseRectW == 0) {
        _baseRectW = new ExprCBoxWidget(this);
        // Disable the obtrusive grey background.
        // It's noticeable with the sunken border of the CCurve. -amyspark
        _baseRectW->setStyleSheet("background-color: transparent;");
    }
    if (_baseRect == 0) {
        _baseRect = addWidget(_baseRectW);
    }
    _baseRectW->setMinimumWidth(_width);
    _baseRect->widget()->update();
    _baseRect->setZValue(0);
}

// draws the cv points
void CCurveScene::drawPoints() {
    while (_circleObjects.size()) {
        delete _circleObjects[0];
        _circleObjects.erase(_circleObjects.begin());
    }
    const int numCV = _cvs.size();
    for (int i = 0; i < numCV; i++) {
        const T_CURVE::CV &pt = _cvs[i];
        QPen pen;
        if (i == _selectedItem) {
            pen = QPen(QColor(255, 170, 0), 1.0);
        } else {
            pen = QPen(Qt::black, 1.0);
        }
        _circleObjects.push_back(addEllipse(
            pt._pos * _width - 4,
            _height + 3,
            8,
            8,
            pen,
            QBrush(QColor(int(255 * pt._val[0] + 0.5), int(255 * pt._val[1] + 0.5), int(255 * pt._val[2] + 0.5)))));
        QGraphicsEllipseItem *circle = _circleObjects.back();
        circle->setFlag(QGraphicsItem::ItemIsMovable, true);
        circle->setZValue(2);
    }
}

void ExprCBoxWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter p(this);
    p.drawPixmap(0, 0, _curveScene->getPixmap());
}

void ExprCSwatchFrame::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter p(this);
    p.fillRect(contentsRect(), _color);
}

ExprCSwatchFrame::ExprCSwatchFrame(KSeExpr::Vec3d value, QWidget *parent) : QFrame(parent), _value(value) {
    _color = QColor(int(255 * _value[0] + 0.5), int(255 * _value[1] + 0.5), int(255 * _value[2] + 0.5));
}

void ExprCSwatchFrame::setValue(const KSeExpr::Vec3d &value) {
    _color = QColor(int(255 * value[0] + 0.5), int(255 * value[1] + 0.5), int(255 * value[2] + 0.5));
    // setPalette(QPalette(_color));
    _value = value;
    repaint();
}

KSeExpr::Vec3d ExprCSwatchFrame::getValue() const { return _value; }

void ExprCSwatchFrame::mousePressEvent(QMouseEvent *event) {
    Q_UNUSED(event);
    QColor color = QColorDialog::getColor(_color);
    if (color.isValid()) {
        _value[0] = color.red() / 255.0;
        _value[1] = color.green() / 255.0;
        _value[2] = color.blue() / 255.0;
        setPalette(QPalette(color));
        _color = color;
        emit selValChangedSignal(_value);
        emit swatchChanged(color);
    }
}

ExprColorCurve::ExprColorCurve(QWidget *parent, QString pLabel, QString vLabel, QString iLabel, bool expandable)
    : QWidget(parent), _scene(0), _selPosEdit(0), _selValEdit(0), _interpComboBox(0) {
    Q_UNUSED(iLabel);
    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->setMargin(0);

    QWidget *edits = new QWidget;
    QFormLayout *editsLayout = new QFormLayout;
    editsLayout->setMargin(0);
    edits->setLayout(editsLayout);

    _selPosEdit = new QLineEdit;
    QDoubleValidator *posValidator = new QDoubleValidator(0.0, 1.0, 6, _selPosEdit);
    _selPosEdit->setValidator(posValidator);
    QString posLabel;
    if (pLabel.isEmpty()) {
        posLabel = tr("Selected Position:");
    } else {
        posLabel = pLabel;
    }
    editsLayout->addRow(posLabel, _selPosEdit);

    _selValEdit = new ExprCSwatchFrame(KSeExpr::Vec3d(.5));
    _selValEdit->setMinimumHeight(_selPosEdit->minimumSizeHint().height());
    _selValEdit->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    QString valLabel;
    if (vLabel.isEmpty()) {
        valLabel = tr("Selected Color:");
    } else {
        valLabel = vLabel;
    }
    editsLayout->addRow(valLabel, _selValEdit);

    _interpComboBox = new QComboBox;
    _interpComboBox->addItem(tr("None"));
    _interpComboBox->addItem(tr("Linear"));
    _interpComboBox->addItem(tr("Smooth"));
    _interpComboBox->addItem(tr("Spline"));
    _interpComboBox->addItem(tr("MSpline"));
    _interpComboBox->setCurrentIndex(4);

    editsLayout->addWidget(_interpComboBox);

    CurveGraphicsView *curveView = new CurveGraphicsView;
    curveView->setFrameShape(QFrame::StyledPanel);
    curveView->setFrameShadow(QFrame::Sunken);
    curveView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    curveView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _scene = new CCurveScene;
    curveView->setScene(_scene);
    curveView->setTransform(QTransform().scale(1, -1));
    curveView->setRenderHints(QPainter::Antialiasing);

    mainLayout->addWidget(edits);
    mainLayout->addWidget(curveView);
    if (expandable) {
        QToolButton *expandButton = new QToolButton(this);
        expandButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        QIcon expandIcon = QIcon::fromTheme("arrow-right", QIcon::fromTheme("go-next"));
        QAction *detailAction = new QAction(expandIcon, tr("&Expand..."));
        expandButton->setDefaultAction(detailAction);
        mainLayout->addWidget(expandButton);
        // open a the detail widget when clicked
        connect(expandButton, SIGNAL(triggered(QAction *)), this, SLOT(openDetail()));
    }
    mainLayout->setStretchFactor(curveView, 100);
    setLayout(mainLayout);

    // SIGNALS

    // when a user selects a cv, update the fields on left
    connect(_scene,
            SIGNAL(cvSelected(double, KSeExpr::Vec3d, T_INTERP)),
            this,
            SLOT(cvSelectedSlot(double, KSeExpr::Vec3d, T_INTERP)));
    // when a user selects a different interp, the curve has to redraw
    connect(_interpComboBox, SIGNAL(activated(int)), _scene, SLOT(interpChanged(int)));
    // when a user types a different position, the curve has to redraw
    connect(_selPosEdit, SIGNAL(returnPressed()), this, SLOT(selPosChanged()));
    connect(this, SIGNAL(selPosChangedSignal(double)), _scene, SLOT(selPosChanged(double)));
    // when a user selects a different color, the ramp has to redraw
    connect(_selValEdit,
            SIGNAL(selValChangedSignal(KSeExpr::Vec3d)),
            _scene,
            SLOT(selValChanged(KSeExpr::Vec3d)));
    connect(_selValEdit, SIGNAL(swatchChanged(QColor)), this, SLOT(internalSwatchChanged(QColor)));
    // when the widget is resized, resize the curve widget
    connect(curveView, SIGNAL(resizeSignal(int, int)), _scene, SLOT(resize(int, int)));
}

// CV selected, update the user interface fields.
void ExprColorCurve::cvSelectedSlot(const double pos, const KSeExpr::Vec3d val, const T_INTERP interp) {
    QString posStr;
    if (pos >= 0.0) {
        posStr.setNum(pos, 'f', 3);
        _selPosEdit->setText(posStr);
        _selValEdit->setValue(val);
        emit swatchChanged(QColor::fromRgbF(val[0], val[1], val[2], 1));
        _interpComboBox->setCurrentIndex(interp);
    }
}

// User entered new position, round and send signal to redraw curve.
void ExprColorCurve::selPosChanged() {
    double pos = KSeExpr::clamp(QString(_selPosEdit->text()).toFloat(), 0, 1);
    _selPosEdit->setText(QString(tr("%1")).arg(pos, 0, 'f', 3));
    emit selPosChangedSignal(pos);
}

void ExprColorCurve::addPoint(const double x, const KSeExpr::Vec3d y, const T_INTERP interp, const bool select) {
    _scene->addPoint(x, y, interp, select);
}

void ExprColorCurve::setSwatchColor(QColor color) {
    KSeExpr::Vec3d newColor(color.redF(), color.greenF(), color.blueF());
    _scene->selValChanged(newColor);
    _selValEdit->setValue(newColor);
}

QColor ExprColorCurve::getSwatchColor() {
    KSeExpr::Vec3d val = _selValEdit->getValue();
    return QColor::fromRgbF(val[0], val[1], val[2], 1);
}

void ExprColorCurve::internalSwatchChanged(QColor color) { emit swatchChanged(color); }

void ExprColorCurve::openDetail() {
    QDialog *dialog = new QDialog();
    dialog->setMinimumWidth(1024);
    dialog->setMinimumHeight(400);
    ExprColorCurve *curve = new ExprColorCurve(0, QString(), QString(), QString(), false);

    // copy points into new data
    const std::vector<T_CURVE::CV> &data = _scene->_cvs;
    typedef std::vector<T_CURVE::CV>::const_iterator ITERATOR;
    for (ITERATOR i = data.begin(); i != data.end(); ++i) curve->addPoint(i->_pos, i->_val, i->_interp);

    QVBoxLayout *layout = new QVBoxLayout();
    dialog->setLayout(layout);
    layout->addWidget(curve);

    dialog->setLayout(layout);
    layout->addWidget(curve);
    QDialogButtonBox *buttonbar = new QDialogButtonBox();
    buttonbar->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
    connect(buttonbar, SIGNAL(accepted()), dialog, SLOT(accept()));
    connect(buttonbar, SIGNAL(rejected()), dialog, SLOT(reject()));
    layout->addWidget(buttonbar);

    if (dialog->exec() == QDialog::Accepted) {
        // copy points back from child
        _scene->removeAll();
        const std::vector<T_CURVE::CV> &dataNew = curve->_scene->_cvs;
        typedef std::vector<T_CURVE::CV>::const_iterator ITERATOR;
        for (ITERATOR i = dataNew.begin(); i != dataNew.end(); ++i) addPoint(i->_pos, i->_val, i->_interp);
        _scene->emitCurveChanged();
    }
}
