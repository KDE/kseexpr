// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * @file ExprHighlighter.cpp
 * @brief A Qt syntax highlighter for the SeExpr language
 * @author  aselle, amyspark
 */

#include "ExprHighlighter.h"

#include <QRegularExpression>

ExprHighlighter::ExprHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
    , lightness(130)
{
    init();
}

ExprHighlighter::ExprHighlighter(QTextEdit *edit)
    : QSyntaxHighlighter(edit)
    , lightness(130)
{
    init();
}

void ExprHighlighter::fixStyle(const QPalette &palette)
{
    lightness = palette.color(QPalette::Base).value() < 127 ? 250 : 130;
    init();
}

void ExprHighlighter::init()
{
    HighlightingRule rule;
    highlightingRules.clear();

    // Operator highlighting, disabled for now
    // operatorFormat.setForeground(QColor::fromHsv(50,128,lightness));
    // QStringList operatorPatterns;
    // operatorPatterns<<"(?:->)|(?:[()\\+-/\\*%\\^:\\?\\[\\]])";
    // foreach (QString pattern,operatorPatterns){
    //    rule.pattern = QRegularExpression(pattern);
    //    rule.format=operatorFormat;
    //    highlightingRules.append(rule);
    //}

    numberFormat.setForeground(QColor::fromHsv(37, 200, lightness));
    rule.pattern = QRegularExpression(QString::fromLatin1("\\b[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)\\b")); // \\b?[^\\$][A-Za-z][A-Za-z0-9]*\\b");
    rule.format = numberFormat;
    highlightingRules.append(rule);

    variableFormat.setForeground(QColor::fromHsv(200, 153, lightness));
    // variableFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression(QString::fromLatin1("\\$[A-Za-z][A-Za-z0-9]*\\b"));
    rule.format = variableFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(QColor::fromHsv(54, 49, lightness));
    rule.pattern = QRegularExpression(QString::fromLatin1("#[^\n]*"));
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);
}

void ExprHighlighter::highlightBlock(const QString &text)
{
    foreach (HighlightingRule rule, highlightingRules) {
        QRegularExpression expression(rule.pattern);

        QRegularExpressionMatchIterator it = expression.globalMatch(text);
        while (it.hasNext()) {
            QRegularExpressionMatch match = it.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }

    }
    setCurrentBlockState(0);
}
