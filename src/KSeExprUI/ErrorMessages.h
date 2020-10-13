// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: LicenseRef-Apache-2.0

/*
 * @file ErrorCode.h
 * @author L. E. Segovia, for Google Summer of Code 2020
 */

#ifndef __SeExpr_ErrorCode_h_
#define __SeExpr_ErrorCode_h_

#include <QObject>
#include <KSeExpr/ErrorCode.h>

class ErrorMessages : public QObject
{

public:
    static QString message(KSeExpr::ErrorCode code);
private:
    Q_OBJECT;
};

#endif
