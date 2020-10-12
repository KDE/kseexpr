/*
 * Copyright (C) 2020 L. E. Segovia <amy@amyspark.me>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License
 * and the following modification to it: Section 6 Trademarks.
 * deleted and replaced with:
 *
 * 6. Trademarks. This License does not grant permission to use the
 * trade names, trademarks, service marks, or product names of the
 * Licensor and its affiliates, except as required for reproducing
 * the content of the NOTICE file.
 *
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * @file Debug.h
 * @author L. E. Segovia, for Google Summer of Code 2020
 */

#include "Debug.h"

const QLoggingCategory &SEEXPR_UI_log() {
    static const QLoggingCategory category("seexpr.ui", QtInfoMsg);
    return category;
}
