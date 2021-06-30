// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Utils.h"
#include <KSeExpr/Utils.h>

bool KSeExpr::Utils::parseRangeComment(const std::string &comment, double &from, double &to)
{
    if (comment.find_first_of('#') != 0) {
        return false;
    }

    auto trimmed_comment = trim(comment.substr(1, std::string::npos));

    auto div = trimmed_comment.find_first_of(' ');

    if (div == std::string::npos) {
        return false;
    }

    std::string first {trim(trimmed_comment.substr(1, div))};

    std::string second {trim(trimmed_comment.substr(div + 1, std::string::npos))};

    auto i = KSeExpr::Utils::atof(first);

    auto j = KSeExpr::Utils::atof(second);

    if (std::isfinite(i) && std::isfinite(j)) {
        from = i, to = j;
        return true;
    }

    return false;
}

bool KSeExpr::Utils::parseRangeComment(const std::string &comment, float &from, float &to)
{
    if (comment.find_first_of('#') != 0) {
        return false;
    }

    auto trimmed_comment = trim(comment.substr(1, std::string::npos));

    auto div = trimmed_comment.find_first_of(' ');

    if (div == std::string::npos) {
        return false;
    }

    std::string first {trim(trimmed_comment.substr(1, div))};

    std::string second {trim(trimmed_comment.substr(div + 1, std::string::npos))};

    auto i = static_cast<float>(KSeExpr::Utils::atof(first));

    auto j = static_cast<float>(KSeExpr::Utils::atof(second));

    if (std::isfinite(i) && std::isfinite(j)) {
        from = i, to = j;
        return true;
    }

    return false;
}

bool KSeExpr::Utils::parseRangeComment(const std::string &comment, int32_t &from, int32_t &to)
{
    if (comment.find_first_of('#') != 0) {
        return false;
    }

    auto trimmed_comment = trim(comment.substr(1, std::string::npos));

    auto div = trimmed_comment.find_first_of(' ');

    if (div == std::string::npos) {
        return false;
    }

    std::string first {trim(trimmed_comment.substr(0, div))};

    std::string second {trim(trimmed_comment.substr(div + 1, std::string::npos))};

    try {
        auto i = KSeExpr::Utils::strtol(first);
        auto j = KSeExpr::Utils::strtol(second);

        from = i, to = j;
        return true;
    }
    catch(const std::exception&) {
        return false;
    }

    return false;
}

bool KSeExpr::Utils::parseTypeNameComment(const std::string &comment, std::string &type, std::string &name)
{
    if (comment.find_first_of('#') != 0) {
        return false;
    }

    auto trimmed_comment = trim(comment.substr(1, std::string::npos));

    auto div = trimmed_comment.find_first_of(' ');

    if (div == std::string::npos) {
        return false;
    }

    auto first = trimmed_comment.substr(1, div);

    auto second = trimmed_comment.substr(div + 1, std::string::npos);

    type.replace(0, type.size(), first);

    name.replace(0, name.size(), second);

    return true;
}

bool KSeExpr::Utils::parseLabelComment(const std::string &comment, std::string &label)
{
    if (comment.find_first_of('#') != 0) {
        return false;
    }

    auto trimmed_comment = trim(comment.substr(1, std::string::npos));

    auto first = trimmed_comment.substr(1, std::string::npos);

    label.replace(0, label.size(), first);

    return true;
}
