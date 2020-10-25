// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ExprPopupDoc.h"
#include <QHBoxLayout>
#include <QLabel>


ExprPopupDoc::ExprPopupDoc(QWidget *parent, const QPoint &placecr, const QString &msg)
{
    Q_UNUSED(parent);
    label = new QLabel(msg);
    auto *layout = new QHBoxLayout;
    setLayout(layout);
    layout->addWidget(label);

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint | Qt::X11BypassWindowManagerHint);
    setAttribute(Qt::WA_ShowWithoutActivating);
    setFocusPolicy(Qt::NoFocus);
    move(placecr);
    raise();
    show();
}

void ExprPopupDoc::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    hide();
}
