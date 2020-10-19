#!/usr/bin/env bash

# SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
# SPDX-License-Identifier: GPL-3.0-or-later

$EXTRACT_TR_STRINGS $(find . -name \*.cpp -o -name \*.h -o -name \*.ui -o -name \*.qml) -o $podir/seexpr2_qt.pot
