
#include "Utils.h"
#include <SeExpr2/Utils.h>

bool SeExpr2::Utils::parseRangeComment(const std::string &comment, double_t &from, double_t &to)
{
    if (comment.find_first_of('#') != 0) {
        return false;
    }

    auto div = comment.find_first_of(' ');

    if (div == std::string::npos) {
        return false;
    }

    std::string first {trim(comment.substr(1, div - 1))};

    std::string second {trim(comment.substr(div + 1, std::string::npos))};

    auto i = SeExpr2::Utils::atof(first);

    auto j = SeExpr2::Utils::atof(second);

    if (std::isfinite(i) && std::isfinite(j)) {
        from = i, to = j;
        return true;
    }

    return false;
}

bool SeExpr2::Utils::parseRangeComment(const std::string &comment, float_t &from, float_t &to)
{
    if (comment.find_first_of('#') != 0) {
        return false;
    }

    auto div = comment.find_first_of(' ');

    if (div == std::string::npos) {
        return false;
    }

    std::string first {trim(comment.substr(1, div - 1))};

    std::string second {trim(comment.substr(div + 1, std::string::npos))};

    auto i = static_cast<float_t>(SeExpr2::Utils::atof(first));

    auto j = static_cast<float_t>(SeExpr2::Utils::atof(second));

    if (std::isfinite(i) && std::isfinite(j)) {
        from = i, to = j;
        return true;
    }

    return false;
}

bool SeExpr2::Utils::parseRangeComment(const std::string &comment, int32_t &from, int32_t &to)
{
    if (comment.find_first_of('#') != 0) {
        return false;
    }

    auto div = comment.find_first_of(' ');

    if (div == std::string::npos) {
        return false;
    }

    std::string first {trim(comment.substr(1, div - 1))};

    std::string second {trim(comment.substr(div + 1, std::string::npos))};

    try {
        auto i = SeExpr2::Utils::strtol(first);
        auto j = SeExpr2::Utils::strtol(second);

        from = i, to = j;
        return true;
    }
    catch(const std::exception&) {
        return false;
    }

    return false;
}

bool SeExpr2::Utils::parseTypeNameComment(const std::string &comment, std::string &type, std::string &name)
{
    if (comment.find_first_of('#') != 0) {
        return false;
    }

    auto div = comment.find_first_of(' ');

    if (div == std::string::npos) {
        return false;
    }

    auto first = comment.substr(1, div - 1);

    auto second = comment.substr(div + 1, std::string::npos);

    type.replace(0, type.size(), first);

    name.replace(0, name.size(), second);

    return true;
}

bool SeExpr2::Utils::parseLabelComment(const std::string &comment, std::string &label)
{
    if (comment.find_first_of('#') != 0) {
        return false;
    }

    auto first = comment.substr(1, std::string::npos);

    label.replace(0, label.size(), first);

    return true;
}
