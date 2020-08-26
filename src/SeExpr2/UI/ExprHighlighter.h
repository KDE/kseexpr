/*
* Copyright Disney Enterprises, Inc.  All rights reserved.
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
* @file ExprHighlighter.h
* @brief A Qt syntax highlighter for the SeExpr language
* @author  aselle, amyspark
*/

#ifndef _ExprHighlighter_h_
#define _ExprHighlighter_h_

#include <QPalette>
#include <QSyntaxHighlighter>
#include <QTextEdit>
#include <QTextDocument>

class ExprHighlighter : public QSyntaxHighlighter {
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
    ExprHighlighter(QTextDocument* parent);
    ExprHighlighter(QTextEdit* edit);

    void fixStyle(const QPalette& palette);

    void init();
    void highlightBlock(const QString& text);
};
#endif
