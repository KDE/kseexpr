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
 * @file ErrorCode.h
 * @author L. E. Segovia, for Google Summer of Code 2020
 */

#ifndef __SeExpr_ErrorCode_h_
#define __SeExpr_ErrorCode_h_

#include <QObject>
#include <SeExpr2/ErrorCode.h>

class ErrorMessages : public QObject
{

public:
    static QString message(SeExpr2::ErrorCode code);
private:
    Q_OBJECT;
};

#endif
