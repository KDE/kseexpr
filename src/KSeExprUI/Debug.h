// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

/*
 * @file Debug.h
 * @author L. E. Segovia, for Google Summer of Code 2020
 */

#ifndef __SeExpr_Debug_h_
#define __SeExpr_Debug_h_

#include <QDebug>
#include <QLoggingCategory>

extern const QLoggingCategory &SEEXPR_UI_log();

#define dbgSeExpr qCDebug(SEEXPR_UI_log)
#define warnSeExpr qCWarning(SEEXPR_UI_log)
#define errorSeExpr qCCritical(SEEXPR_UI_log)

#endif
