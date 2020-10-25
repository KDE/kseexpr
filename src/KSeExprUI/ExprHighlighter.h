// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * @file ExprHighlighter.h
 * @brief A Qt syntax highlighter for the SeExpr language
 * @author  aselle, amyspark
 */

#pragma once

#include <QPalette>
#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QTextEdit>


class ExprHighlighter : public QSyntaxHighlighter
{
    struct HighlightingRule {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat variableFormat;
    QTextCharFormat numberFormat;
    QTextCharFormat operatorFormat;

    int lightness;

public:
    ExprHighlighter(QTextDocument *parent);
    ExprHighlighter(QTextEdit *edit);

    void fixStyle(const QPalette &palette);

    void init();
    void highlightBlock(const QString &text) override;
};
