// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

/*
* @file Debug.h
 * @author L. E. Segovia, for Google Summer of Code 2020
 */

#include "Debug.h"

const QLoggingCategory &SEEXPR_UI_log() {
    static const QLoggingCategory category("seexpr.ui", QtInfoMsg);
    return category;
}
