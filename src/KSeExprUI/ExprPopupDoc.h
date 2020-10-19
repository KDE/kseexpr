// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef _ExprPopupDoc_h_
#define _ExprPopupDoc_h_

#include <QWidget>

class QLabel;
class ExprPopupDoc : public QWidget {
    Q_OBJECT;

  public:
    QLabel* label;
    ExprPopupDoc(QWidget* parent, const QPoint& cr, const QString& msg);

  protected:
    void mousePressEvent(QMouseEvent* event);
};

#endif
