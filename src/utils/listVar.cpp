// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <KSeExpr/ExprPatterns.h>
#include <KSeExpr/ExprWalker.h>
#include <KSeExpr/Expression.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace KSeExpr;

/// Examiner that builds a list of all variable references
class VarListExaminer : public ConstExaminer
{
public:
    bool examine(T_NODE *examinee) override
    {
        if (const ExprVarNode *var = isVariable(examinee)) {
            _varList.push_back(var);
            return false;
        };
        return true;
    }
    void post(T_NODE *) override {};
    void reset() override
    {
        _varList.clear();
    };
    inline int length() const
    {
        return _varList.size();
    };
    inline const ExprVarNode *var(int i) const
    {
        return _varList[i];
    };

private:
    std::vector<const ExprVarNode *> _varList;
};

/**
   @file listVar.cpp
*/
//! Simple expression class to list out variable uses
class ListVarExpr : public Expression
{
public:
    //! Constructor that takes the expression to parse
    ListVarExpr(const std::string &expr)
        : Expression(expr)
        , examiner()
        , walker(&examiner) {};

    //! Empty constructor
    ListVarExpr()
        : Expression()
        , examiner()
        , walker(&examiner) {};

    void walk()
    {
        _hasWalked = true;
        walker.walk(_parseTree);
    };

    bool hasWalked() const
    {
        return _hasWalked;
    };

    int count() const
    {
        if (isValid() && _hasWalked) {
            return examiner.length();
        };
        return 0;
    };

private:
    bool _hasWalked{};
    VarListExaminer examiner;
    ConstWalker walker;

    //! resolve function that only supports one external variable 'x'
    ExprVarRef *resolveVar(const std::string &) const override
    {
        return nullptr;
    };
};

void quit(const std::string &str)
{
    if (str == "quit" || str == "q")
        exit(0);
}

int main(int, char *[])
{
    ListVarExpr expr;
    std::string str;

    std::cout << "SeExpr Basic Variable Use Finder\n *Note: Does not "
              << "find variables being assigned to, only use.*";

    while (true) {
        std::cout << std::endl << "> ";
        // std::cin >> str;
        getline(std::cin, str);

        if (std::cin.eof()) {
            std::cout << std::endl;
            str = "q";
        };

        quit(str);
        expr.setExpr(str);

        if (!expr.isValid()) {
            std::cerr << "Expression failed: " << expr.parseError() << std::endl;
        } else {
            std::cout << "   " << expr.evalFP() << std::endl;
            expr.walk();
            std::cout << "   number of variable refs: " << expr.count() << std::endl;
        };
    };

    return 0;
}
