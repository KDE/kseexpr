// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <KSeExpr/ExprNode.h>
#include <KSeExpr/ExprPatterns.h>
#include <sstream>

#include "ControlSpec.h"
#include "Utils.h"

namespace KSeExpr
{
SpecExaminer::~SpecExaminer()
{
    for (auto& i: _specList)
        delete i;
};

bool SpecExaminer::examine(const ExprNode *examinee)
{
    if (const ExprScalarAssignSpec *s_spec = ExprScalarAssignSpec::match(examinee)) {
        _specList.push_back(s_spec);
        return false;
    } else if (const ExprVectorAssignSpec *v_spec = ExprVectorAssignSpec::match(examinee)) {
        _specList.push_back(v_spec);
        return false;
    } else if (const ExprCurveAssignSpec<double> *c_spec = ExprCurveAssignSpec<double>::match(examinee)) {
        _specList.push_back(c_spec);
        return false;
    } else if (const ExprCurveAssignSpec<Vec3d> *cc_spec = ExprCurveAssignSpec<Vec3d>::match(examinee)) {
        _specList.push_back(cc_spec);
        return false;
    } else if (const ExprStrSpec *str_spec = ExprStrSpec::match(examinee)) {
        _specList.push_back(str_spec);
        return false;
    };

    return true;
};

inline std::vector<const ControlSpec *>::const_iterator SpecExaminer::begin() const
{
    return _specList.begin();
};

inline std::vector<const ControlSpec *>::const_iterator SpecExaminer::end() const
{
    return _specList.end();
};

//! Returns true if no newline separates comment and node
inline bool isWS(const char *source, int start, int end)
{
    for (int i = start; i < end; ++i)
        if (source[i] != '\n')
            return false;
    return true;
};

//! Checks if there is whitespace in the range specified in the string
inline std::string findComment(const ExprNode &node)
{
    const Expression &expr = *node.expr();
    using Comments = std::vector<std::pair<int, int> >;
    const Comments &comments = expr.getComments();
    const std::string &s = expr.getExpr();

    // TODO: user lower_bound to make this O(lg n) instead of O(n)
    for (const auto & i: comments) {
        if (i.first >= node.endPos() && isWS(s.c_str(), node.endPos(), i.first))
            return s.substr(i.first, i.second - i.first + 1);
    }
    return "";
}

ExprScalarAssignSpec::ExprScalarAssignSpec(const ExprAssignNode &node)
    : ControlSpec(node)
    , _min(0)
    , _max(1)
    , _val(dynamic_cast<const ExprNumNode *>(node.child(0))->value())
{
    _name = node.name();
    std::string comment = findComment(node);
    // TODO: handle integer case
    int numParsed = sscanf(comment.c_str(), "#%lf,%lf\n", &_min, &_max);
    if (numParsed != 2) {
        _min = 0;
        _max = 1;
    }
}

std::string ExprScalarAssignSpec::toString() const
{
    std::stringstream ss;

    ss << _name << ": " << value() << " in [" << _min << "," << _max << "]" << std::endl;

    return ss.str();
}

const ExprScalarAssignSpec *ExprScalarAssignSpec::match(const ExprNode *node)
{
    if (const ExprAssignNode *assign = isScalarAssign(node))
        return new ExprScalarAssignSpec(*assign);

    return nullptr;
}

ExprVectorAssignSpec::ExprVectorAssignSpec(const ExprAssignNode &node)
    : ControlSpec(node)
    , _min(0)
    , _max(1)
    , _val(Vec3d(dynamic_cast<const ExprNumNode *>(node.child(0)->child(0))->value(), dynamic_cast<const ExprNumNode *>(node.child(0)->child(1))->value(), dynamic_cast<const ExprNumNode *>(node.child(0)->child(2))->value()))
{
    _name = node.name();
    std::string comment = findComment(node);
    int numParsed = sscanf(comment.c_str(), "#%lf,%lf\n", &_min, &_max);
    if (numParsed != 2) {
        _min = 0;
        _max = 1;
    }

std::string ExprVectorAssignSpec::toString() const
{
    std::stringstream ss;

    ss << _name << ": " << value() << " in [" << _min << "," << _max << "]" << std::endl;
    ;

    return ss.str();
}

template<class T>
ExprCurveAssignSpec<T>::ExprCurveAssignSpec(const ExprAssignNode &node)
    : ControlSpec(node)
    , _vec()
{
    _name = node.name();
    const auto *cnode = dynamic_cast<const ExprFuncNode *>(node.child(0));
    _lookupText = cnode->child(0)->toString();
    int num = cnode->numChildren();
    for (int i = 1; i < num - 2; i += 3)
        _vec.push_back(typename Curve<T>::CV(
            dynamic_cast<const ExprNumNode *>(cnode->child(i))->value(), dynamic_cast<const ExprNumNode *>(cnode->child(i + 1))->value(), (typename Curve<T>::InterpType) dynamic_cast<const ExprNumNode *>(cnode->child(i + 2))->value()));
}

const ExprVectorAssignSpec *ExprVectorAssignSpec::match(const ExprNode *node)
{
    if (const ExprAssignNode *assign = isVectorAssign(node)) {
        return new ExprVectorAssignSpec(*assign);
    }

    return nullptr;
}

template<class T> std::string ExprCurveAssignSpec<T>::toString() const
{
    std::stringstream ss;

    ss << _name << ": "
       << "curve(" << _lookupText;
    int num = _vec.size();
    for (int i = 0; i < num; ++i)
        ss << _vec[i]._pos << _vec[i]._val << (int)_vec[i]._interp;
    ss << ");";

    return ss.str();
}

template<class T> const ExprCurveAssignSpec<T> *ExprCurveAssignSpec<T>::match(const ExprNode *node)
{
    if (const ExprAssignNode *assign = isCurveAssign(node))
        return new ExprCurveAssignSpec(*assign);

    return 0;
}

#if 0

ExprCcurveAssignSpec::
ExprCcurveAssignSpec(const ExprAssignNode& node)
    : ControlSpec(node),
     _vec()
{
    _name=node.name();
    const ExprFuncNode* cnode(static_cast<const ExprFuncNode*>(node.child(0)));
    _lookupText=cnode->child(0)->toString();
    int num = cnode->numChildren();
    for(int i = 1; i < num - 2; i += 3)
        if(dynamic_cast<const ExprNumNode*>(cnode->child(i+1)))
            _vec.push_back(Curve<Vec3d>::CV(
                    static_cast<const ExprNumNode*>(cnode->child(i))->value(),
                    static_cast<const ExprNumNode*>(cnode->child(i+1))->value(),
                    (Curve<Vec3d>::InterpType) static_cast<const ExprNumNode*>(cnode->child(i+2))->value()));
}

std::string
ExprCcurveAssignSpec::toString() const
{
    std::stringstream ss;

    ss << _name
       << " = "
       << "ccurve("
       << _lookupText;
    int num = _vec.size();
    for(int i = 0; i < num; ++i)
        ss << ", "
           << _vec[i]._pos
           << ", "
           << _vec[i]._val
           << ", "
           << (int)_vec[i]._interp;
    ss << ");";

    return ss.str();
}

const ExprCcurveAssignSpec*
ExprCcurveAssignSpec::match(const ExprNode* node)
{
    if(const ExprAssignNode* assign = isCcurveAssign(node))
        return new ExprCcurveAssignSpec(*assign);


    return 0;
}

#endif

std::string ExprStrSpec::toString() const
{
    std::stringstream ss;
    ss << _name << ": \"" + _str + "\" ";
    switch (_type) {
    case STRING:
        ss << "STRING";
        break;
    case FILE:
        ss << "FILE";
        break;
    case DIRECTORY:
        ss << "DIRECTORY";
        break;
    default:
        ss << "INVALID";
        break;
    }
    return ss.str();
}

const ExprStrSpec *ExprStrSpec::match(const ExprNode *node)
{
    if (const ExprStrNode *strnode = isString(node)) {
        std::string comment = findComment(*node);
        std::string name {};
        std::string type {};
        bool parsed = KSeExpr::Utils::parseTypeNameComment(comment, type, name);

        if (parsed) {
            if (type == "string")
                return new ExprStrSpec(*strnode, name.c_str(), STRING);
            else if (type == "file")
                return new ExprStrSpec(*strnode, name.c_str(), FILE);
            else if (type == "directory")
                return new ExprStrSpec(*strnode, name.c_str(), DIRECTORY);
        }
    }
    return nullptr;
}
} // namespace KSeExpr
