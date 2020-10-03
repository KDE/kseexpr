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
* @file Editable.h
* @author Andrew Selle, L. E. Segovia
*/

#include <cstdio>

#include "Debug.h"
#include "Editable.h"
#include "Utils.h"


Editable::Editable(const std::string& name, int startPos, int endPos)
    : name(name), startPos(startPos), endPos(endPos)
{

}

void Editable::updatePositions(const Editable& other)
{
    startPos = other.startPos;
    endPos = other.endPos;
}

Editable::~Editable()
{

}

std::string Editable::str() const
{
    return std::string("<unknown>");
}

NumberEditable::NumberEditable(const std::string& name, int startPos, int endPos, double val)
    : Editable(name, startPos, endPos), v(val), min(0), max(1), isInt(false)
{

}

bool NumberEditable::parseComment(const std::string& comment)
{
    if (comment.find('.') != std::string::npos || comment.find('e') != std::string::npos) {
        float fmin, fmax;
        if (SeExpr2::Utils::parseRangeComment(comment, fmin, fmax))
        {
            min = fmin;
            max = fmax;
            isInt = false;
        }
    }
    int imin, imax;
    if (SeExpr2::Utils::parseRangeComment(comment, imin, imax)) {
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

void NumberEditable::appendString(std::stringstream& stream) const
{
    stream << v;
}

bool NumberEditable::controlsMatch(const Editable& other) const
{
    if (const NumberEditable* o = dynamic_cast<const NumberEditable*>(&other)) {
        return min == o->min && max == o->max && v == o->v && isInt == o->isInt && name == o->name;
    } else
        return false;
}

VectorEditable::VectorEditable(const std::string& name, int startPos, int endPos, const SeExpr2::Vec3d& val)
    : Editable(name, startPos, endPos), v(val), min(0), max(1), isColor(true)
{

}

bool VectorEditable::parseComment(const std::string& comment)
{
    float fmin, fmax;
    bool parsed = SeExpr2::Utils::parseRangeComment(comment, fmin, fmax);
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

void VectorEditable::appendString(std::stringstream& stream) const
{
    printVal(stream, v);
}

bool VectorEditable::controlsMatch(const Editable& other) const
{
    if (const VectorEditable* o = dynamic_cast<const VectorEditable*>(&other)) {
        return min == o->min && max == o->max && v == o->v && name == o->name;
    } else
        return false;
}

StringEditable::StringEditable(int startPos, int endPos, const std::string& val)
    : Editable("unknown", startPos, endPos), v(val)
{

}

bool StringEditable::parseComment(const std::string& comment)
{
    std::string namebuf{}, typebuf{};
    bool parsed = SeExpr2::Utils::parseTypeNameComment(comment, namebuf, typebuf);
    if (parsed) {
        name = namebuf;
        type = typebuf;
        return true;
    } else {
        return false;
    }
}

void StringEditable::appendString(std::stringstream& stream) const
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

bool StringEditable::controlsMatch(const Editable& other) const
{
    if (const StringEditable* o = dynamic_cast<const StringEditable*>(&other)) {
        return v == o->v && type == o->type && name == o->name;
    } else
        return false;
}

#ifdef SEEXPR_ENABLE_ANIMCURVE
AnimCurveEditable::AnimCurveEditable(const std::string& name, int startPos, int endPos)
    : Editable(name, startPos, endPos)
#ifdef SEEXPR_USE_ANIMLIB
      ,
      curve(animlib::AnimAttrID())
#endif
{

}

AnimCurveEditable::~AnimCurveEditable()
{

}

bool AnimCurveEditable::parseComment(const std::string& comment)
{
    animationSystemCurve = comment;
    return true;
}

std::string AnimCurveEditable::str() const
{
    std::stringstream s;
    s << name << " ccurve";
    return s.str();
}

void AnimCurveEditable::appendString(std::stringstream& stream) const
{
#ifdef SEEXPR_USE_ANIMLIB
        if (newText.length() > 0)
            stream << newText;
        else {
            stream << ",\"" << animlib::AnimCurve::infinityTypeToString(curve.getPreInfinity()) << "\"";
            stream << ",\"" << animlib::AnimCurve::infinityTypeToString(curve.getPostInfinity()) << "\"";
            stream << "," << curve.isWeighted();
            stream << ",\"" << link << "\"";
            for (auto it = curve.getFirstKey(), itend = curve.getEndKey(); it != itend; ++it) {
                const animlib::AnimKeyframe& key = *it;
                stream << "," << key.getTime() << "," << key.getValue() << "," << key.getInWeight() << ","
                       << key.getOutWeight() << "," << key.getInAngle() << "," << key.getOutAngle() << ",\""
                       << animlib::AnimKeyframe::tangentTypeToString(key.getInTangentType()) << "\",\""
                       << animlib::AnimKeyframe::tangentTypeToString(key.getOutTangentType()) << "\","
                       << key.isWeightsLocked();
            }
        }
#else
        UNUSED(stream);
#endif
}

bool AnimCurveEditable::controlsMatch(const Editable& other) const
{
    if (const AnimCurveEditable* o = dynamic_cast<const AnimCurveEditable*>(&other)) {
        // TODO: fix  this
        //            return cvs==o->cvs && name==o->name;
        UNUSED(o);
        return false;
    } else
        return false;
}
#endif

ColorSwatchEditable::ColorSwatchEditable(const std::string& name, int startPos, int endPos)
    : Editable(name, startPos, endPos)
{

}

bool ColorSwatchEditable::parseComment(const std::string& comment)
{
    std::string labelbuf{};
    bool parsed = SeExpr2::Utils::parseLabelComment(comment, labelbuf);
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

void ColorSwatchEditable::appendString(std::stringstream& stream) const
{
    for (size_t i = 0, sz = colors.size(); i < sz; i++) {
        const SeExpr2::Vec3d& color = colors[i];
        stream << ",";
        printVal(stream, color);
    }
}

bool ColorSwatchEditable::controlsMatch(const Editable& other) const
{
    if (const ColorSwatchEditable* o = dynamic_cast<const ColorSwatchEditable*>(&other)) {
        // TODO: determine when controls match
        UNUSED(o);
        return false;
    } else
        return false;
}

void ColorSwatchEditable::add(const SeExpr2::Vec3d& value)
{
    colors.push_back(value);
}

void ColorSwatchEditable::change(int index, const SeExpr2::Vec3d& value)
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
    for (unsigned int i = 0; i < colors.size(); i++) {
        dbgSeExpr << colors[i][0] << ", " << colors[i][1] << ", " << colors[i][2];
    }
}

#ifdef SEEXPR_ENABLE_DEEPWATER
DeepWaterEditable::DeepWaterEditable(const std::string& name, int startPos, int endPos)
    : Editable(name, startPos, endPos)
{

}

void DeepWaterEditable::setParams(const SeDeepWaterParams& paramsIn)
{
    params = paramsIn;
}

bool DeepWaterEditable::parseComment(const std::string&)
{
    return true;
}

std::string DeepWaterEditable::str() const
{
    std::stringstream s;
    s << name << " deepWater";
    return s.str();
}

void DeepWaterEditable::appendString(std::stringstream& stream) const
{
    stream << "," << params.resolution;
    stream << "," << params.tileSize;
    stream << "," << params.lengthCutoff;
    stream << "," << params.amplitude;
    stream << "," << params.windAngle;
    stream << "," << params.windSpeed;
    stream << "," << params.directionalFactorExponent;
    stream << "," << params.directionalReflectionDamping << ",";
    printVal(stream, params.flowDirection);
    stream << "," << params.sharpen;
    stream << "," << params.time;
    stream << "," << params.filterWidth;
}

bool DeepWaterEditable::controlsMatch(const Editable& other) const
{
    if (const DeepWaterEditable* o = dynamic_cast<const DeepWaterEditable*>(&other)) {
        // TODO: determine when controls match
        UNUSED(o);
        return false;
    } else
        return false;
}

#endif  // SEEXPR_ENABLE_DEEPWATER
