// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <KSeExpr/Curve.h>
#include <KSeExpr/Vec.h>
#include <cstdio>
#include <sstream>
#include <vector>

/// Mini parse tree node... Only represents literals, and lists of literals
struct ExprSpecNode {
    int startPos;
    int endPos;

    ExprSpecNode(int startPos, int endPos)
        : startPos(startPos)
        , endPos(endPos)
    {
    }

    virtual ~ExprSpecNode() = default;

    ExprSpecNode &operator=(ExprSpecNode &&) = default;
    ExprSpecNode &operator=(const ExprSpecNode &) = default;
    ExprSpecNode(const ExprSpecNode &) = default;
    ExprSpecNode(ExprSpecNode &&) = default;
};

struct ExprSpecScalarNode : public ExprSpecNode {
    double v;

    ExprSpecScalarNode(int startPos, int endPos, double scalar)
        : ExprSpecNode(startPos, endPos)
        , v(scalar)
    {
    }
};

struct ExprSpecVectorNode : public ExprSpecNode {
    KSeExpr::Vec3d v;
    ExprSpecVectorNode(int startPos, int endPos, ExprSpecNode *x, ExprSpecNode *y, ExprSpecNode *z)
        : ExprSpecNode(startPos, endPos)
    {
        v = KSeExpr::Vec3d(dynamic_cast<ExprSpecScalarNode *>(x)->v, dynamic_cast<ExprSpecScalarNode *>(y)->v, dynamic_cast<ExprSpecScalarNode *>(z)->v);
    }
};

struct ExprSpecListNode : public ExprSpecNode {
    std::vector<ExprSpecNode *> nodes;
    ExprSpecListNode(int startPos, int endPos)
        : ExprSpecNode(startPos, endPos)
    {
    }

    void add(ExprSpecNode *node)
    {
        startPos = std::min(node->startPos, startPos);
        endPos = std::max(node->endPos, endPos);
        nodes.push_back(node);
    }
};

struct ExprSpecStringNode : public ExprSpecNode {
    std::string v;
    ExprSpecStringNode(int startPos, int endPos, const char *s)
        : ExprSpecNode(startPos, endPos)
        , v(s)
    {
    }
};

struct ExprSpecCurveNode : public ExprSpecNode {
    ExprSpecNode *args;
    ExprSpecCurveNode(ExprSpecNode *args)
        : ExprSpecNode(args->startPos, args->endPos)
        , args(args)
    {
    }
};

struct ExprSpecCCurveNode : public ExprSpecNode {
    ExprSpecNode *args;
    ExprSpecCCurveNode(ExprSpecNode *args)
        : ExprSpecNode(args->startPos, args->endPos)
        , args(args)
    {
    }
};

struct ExprSpecColorSwatchNode : public ExprSpecNode {
    ExprSpecNode *args;
    ExprSpecColorSwatchNode(ExprSpecNode *args)
        : ExprSpecNode(args->startPos, args->endPos)
        , args(args)
    {
    }
};
