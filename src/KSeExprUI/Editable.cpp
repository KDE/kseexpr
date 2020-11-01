// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * @file Editable.h
 * @author Andrew Selle, L. E. Segovia
 */

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <utility>

#include "Debug.h"
#include "Editable.h"
#include "Utils.h"

Editable::Editable(const std::string &name, int startPos, int endPos)
    : name(name)
    , startPos(startPos)
    , endPos(endPos)
{
}

void Editable::updatePositions(const Editable &other)
{
    startPos = other.startPos;
    endPos = other.endPos;
}

std::string Editable::str() const
{
    return std::string("<unknown>");
}

NumberEditable::NumberEditable(const std::string &name, int startPos, int endPos, double val)
    : Editable(name, startPos, endPos)
    , v(val)
    , min(0)
    , max(1)
    , isInt(false)
{
}

bool NumberEditable::parseComment(const std::string &comment)
{
    if (comment.find('.') != std::string::npos || comment.find('e') != std::string::npos) {
        float fmin = NAN;
        float fmax = NAN;
        if (KSeExpr::Utils::parseRangeComment(comment, fmin, fmax)) {
            min = fmin;
            max = fmax;
            isInt = false;
        }
    }
    int imin = 0;
    int imax = 0;
    if (KSeExpr::Utils::parseRangeComment(comment, imin, imax)) {
        min = imin;
        max = imax;
        isInt = true;
    }
    return true;
}

std::string NumberEditable::str() const
{
    std::stringstream s;
    s << name << " " << v << " in [" << min << "," << max << "] subset " << (isInt ? "Integers" : "Reals");
    return s.str();
}

void NumberEditable::appendString(std::stringstream &stream) const
{
    stream << v;
}

bool NumberEditable::controlsMatch(const Editable &other) const
{
    if (const auto *o = dynamic_cast<const NumberEditable *>(&other)) {
        return min == o->min && max == o->max && v == o->v && isInt == o->isInt && name == o->name;
    } else
        return false;
}

VectorEditable::VectorEditable(const std::string &name, int startPos, int endPos, const KSeExpr::Vec3d &val)
    : Editable(name, startPos, endPos)
    , v(val)
    , min(0)
    , max(1)
    , isColor(true)
{
}

bool VectorEditable::parseComment(const std::string &comment)
{
    float fmin = NAN;
    float fmax = NAN;
    bool parsed = KSeExpr::Utils::parseRangeComment(comment, fmin, fmax);
    if (parsed) {
        if (fmin < 0.0 || fmax > 1.0) {
            isColor = false;
        }
        min = fmin;
        max = fmax;
    }
    return true;
}
std::string VectorEditable::str() const
{
    std::stringstream s;
    s << name << " " << v << " in [" << min << "," << max << "]";
    return s.str();
}

void VectorEditable::appendString(std::stringstream &stream) const
{
    printVal(stream, v);
}

bool VectorEditable::controlsMatch(const Editable &other) const
{
    if (const auto *o = dynamic_cast<const VectorEditable *>(&other)) {
        return min == o->min && max == o->max && v == o->v && name == o->name;
    } else
        return false;
}

StringEditable::StringEditable(int startPos, int endPos, const std::string &val)
    : Editable("unknown", startPos, endPos)
    , v(val)
{
}

bool StringEditable::parseComment(const std::string &comment)
{
    std::string namebuf {};
    std::string typebuf {};
    bool parsed = KSeExpr::Utils::parseTypeNameComment(comment, namebuf, typebuf);
    if (parsed) {
        name = namebuf;
        type = typebuf;
        return true;
    } else {
        return false;
    }
}

void StringEditable::appendString(std::stringstream &stream) const
{
    // TODO: escape strs
    stream << "\"" << v << "\"";
}

std::string StringEditable::str() const
{
    std::stringstream s;
    s << name << " " << type << " = " << v;
    return s.str();
}

bool StringEditable::controlsMatch(const Editable &other) const
{
    if (const auto *o = dynamic_cast<const StringEditable *>(&other)) {
        return v == o->v && type == o->type && name == o->name;
    } else
        return false;
}

ColorSwatchEditable::ColorSwatchEditable(const std::string &name, int startPos, int endPos)
    : Editable(name, startPos, endPos)
{
}

bool ColorSwatchEditable::parseComment(const std::string &comment)
{
    std::string labelbuf {};
    bool parsed = KSeExpr::Utils::parseLabelComment(comment, labelbuf);
    if (parsed) {
        labelType = labelbuf;
    }
    return true;
}

std::string ColorSwatchEditable::str() const
{
    std::stringstream s;
    s << name << " swatch";
    return s.str();
}

void ColorSwatchEditable::appendString(std::stringstream &stream) const
{
    for (const auto &color : colors) {
        stream << ",";
        printVal(stream, color);
    }
}

bool ColorSwatchEditable::controlsMatch(const Editable &other) const
{
    if (const auto *o = dynamic_cast<const ColorSwatchEditable *>(&other)) {
        return this->labelType == o->labelType
            && o->colors.size() == this->colors.size()
            && std::equal(o->colors.begin(), o->colors.end(), this->colors.begin());
    } else
        return false;
}

void ColorSwatchEditable::add(const KSeExpr::Vec3d &value)
{
    colors.push_back(value);
}

void ColorSwatchEditable::change(int index, const KSeExpr::Vec3d &value)
{
    colors[index] = value;
}

void ColorSwatchEditable::remove(int index)
{
    colors.erase(colors.begin() + index);
}

void ColorSwatchEditable::print()
{
    dbgSeExpr << "\nColorSwatchEditable:\n";
    for (const auto &color : colors) {
        dbgSeExpr << color[0] << ", " << color[1] << ", " << color[2];
    }
}
