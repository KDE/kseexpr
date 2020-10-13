// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0

#include <gtest/gtest.h>

#include <KSeExpr/Expression.h>
#include <KSeExpr/ExprFunc.h>

using namespace KSeExpr;


struct StringFunc : public ExprFuncSimple {
    StringFunc() : ExprFuncSimple(true) {
    }

    struct StringData : public KSeExpr::ExprFuncNode::Data, public std::string
    {
    };

    virtual ExprType prep(ExprFuncNode* node, bool scalarWanted, ExprVarEnvBuilder& envBuilder) const {
        bool constant = true;
        for (int i = 0, iend = node->numChildren(); i < iend; ++i) {
            KSeExpr::ExprType t = node->child(i)->prep(!scalarWanted, envBuilder);
            if (t.isString() == false) {
                return KSeExpr::ExprType().Error().Varying();
            }
            if (t.isLifetimeConstant() == false) {
                constant = false;
            }
        }
        return constant == true ? KSeExpr::ExprType().String().Constant() : KSeExpr::ExprType().String().Varying();
    }

    virtual ExprFuncNode::Data* evalConstant(const ExprFuncNode* node, ArgHandle args) const {
        return new StringData();
    }

    virtual void eval(ArgHandle args) {
        StringData& data = *reinterpret_cast<StringData*>(args.data);
        data.clear();
        for (int i = 0, iend = args.nargs(); i < iend; ++i) {
            data += args.inStr(i);
            if (i != iend - 1) {
                data += "/";
            }
        }
        args.outStr = const_cast<char*>(data.c_str());
    }
} joinPath;
ExprFunc joinPathFunc(joinPath, 2, 100);


struct StringExpression : public Expression {
    // Define simple string variable type that just stores the value it returns
    struct Var : public ExprVarRef {
        std::string value;
        Var() : ExprVarRef(ExprType().String().Varying()) {}
        void eval(double*) { }
        void eval(const char** result) { result[0] = value.c_str(); }
        Var& operator = (const char* input) { value = input; return *this; }
    };
    mutable Var stringVar;

    // Custom variable resolver, only allow ones we specify
    ExprVarRef* resolveVar(const std::string& name) const {
        if (name == "stringVar") return &stringVar;
        return 0;
    }

    // Custom function resolver
    ExprFunc* resolveFunc(const std::string& name) const {
        if (name == "join_path") return &joinPathFunc;
        return 0;
    }

    // Constructor
    StringExpression(const std::string& str)
        : Expression(str, ExprType().String()) {}
};

TEST(StringTests, Constant) {
    StringExpression expr("\"hello world !\"");
    EXPECT_TRUE(expr.isValid() == true);
    EXPECT_TRUE(expr.returnType().isString() == true);
    EXPECT_TRUE(expr.isConstant() == true);
    EXPECT_STREQ(expr.evalStr(), "hello world !");

    // check that strings are correctly unescaped
    StringExpression expr7("\"hello\\\\\\t\\n\\\"world\\\"\"");
    EXPECT_TRUE(expr7.isValid() == true);
    EXPECT_TRUE(expr7.returnType().isString() == true);
    EXPECT_TRUE(expr7.isConstant() == true);
    EXPECT_STREQ(expr7.evalStr(), "hello\\\t\n\"world\"");
}

TEST(StringTests, Variable) {
    StringExpression expr("stringVar");
    expr.stringVar = "hey, it's working !";
    EXPECT_TRUE(expr.isValid());
    EXPECT_TRUE(expr.returnType().isString());
    EXPECT_TRUE(!expr.isConstant());
    EXPECT_STREQ(expr.evalStr(), "hey, it's working !");
}

TEST(StringTests, FunctionConst) {
    StringExpression expr("join_path(\"/home/foo\", \"some\", \"relative\", \"path\")");
    EXPECT_TRUE(expr.isValid() == true);
    EXPECT_TRUE(expr.returnType().isString() == true);
    EXPECT_TRUE(expr.isConstant() == true);
    EXPECT_STREQ(expr.evalStr(), "/home/foo/some/relative/path");

    StringExpression expr2("sprintf(\"%04d\", 42)");
    EXPECT_TRUE(expr2.isValid() == true);
    EXPECT_TRUE(expr2.returnType().isString() == true);
    EXPECT_TRUE(expr2.isConstant() == true);
    EXPECT_STREQ(expr2.evalStr(), "0042");
}

TEST(StringTests, FunctionVarying) {
    StringExpression expr("join_path(stringVar, \"some\", \"relative\", \"path\")");
    expr.stringVar = "/home/foo";
    EXPECT_TRUE(expr.isValid() == true);
    EXPECT_TRUE(expr.returnType().isString() == true);
    EXPECT_TRUE(expr.isConstant() == false);
    EXPECT_STREQ(expr.evalStr(), "/home/foo/some/relative/path");
}

TEST(StringTests, BinaryOp) {
    StringExpression expr1("\"hello \" + \"world!\"");
    EXPECT_TRUE(expr1.isValid() == true);
    EXPECT_TRUE(expr1.returnType().isString() == true);
    EXPECT_TRUE(expr1.isConstant() == true);
    EXPECT_STREQ(expr1.evalStr(), "hello world!");

    StringExpression expr2("\"hello \" + \"world\" + \"!\"");
    EXPECT_TRUE(expr2.isValid() == true);
    EXPECT_TRUE(expr2.returnType().isString() == true);
    EXPECT_TRUE(expr2.isConstant() == true);
    EXPECT_STREQ(expr2.evalStr(), "hello world!");

    StringExpression expr3("stringVar + \"world!\"");
    expr3.stringVar = "hello ";
    EXPECT_TRUE(expr3.isValid() == true);
    EXPECT_TRUE(expr3.returnType().isString() == true);
    EXPECT_TRUE(expr3.isConstant() == false);
    EXPECT_STREQ(expr3.evalStr(), "hello world!");

    StringExpression expr4("join_path(\"a\", \"b\") + \"/c/\" + stringVar");
    expr4.stringVar = "d";
    EXPECT_TRUE(expr4.isValid() == true);
    EXPECT_TRUE(expr4.returnType().isString() == true);
    EXPECT_TRUE(expr4.isConstant() == false);
    EXPECT_STREQ(expr4.evalStr(), "a/b/c/d");

    StringExpression expr5("v = 'o' + 'k';\nif ('fo' + 'o' != 'foo') {\n    v = 'error';\n}\nv");
    EXPECT_TRUE(expr5.isValid() == true);
    EXPECT_TRUE(expr5.returnType().isString() == true);
    EXPECT_TRUE(expr5.isConstant() == true);
    EXPECT_STREQ(expr5.evalStr(), "ok");

    StringExpression expr6("v = 'err' + 'or';\nif ('fo' + 'o' == 'foo') {\n    v = 'ok';\n}\nv");
    EXPECT_TRUE(expr6.isValid() == true);
    EXPECT_TRUE(expr6.returnType().isString() == true);
    EXPECT_TRUE(expr6.isConstant() == true);
    EXPECT_STREQ(expr6.evalStr(), "ok");

    StringExpression expr7("v = 'err' + 'or';\nif ('fo' + 'e' != 'foo') {\n    v = 'ok';\n}\nv");
    EXPECT_TRUE(expr7.isValid() == true);
    EXPECT_TRUE(expr7.returnType().isString() == true);
    EXPECT_TRUE(expr7.isConstant() == true);
    EXPECT_STREQ(expr7.evalStr(), "ok");

    StringExpression expr8("v = 'ok';\nif ('fo' + 'e' == 'foo') {\n    v = 'error';\n}\nv");
    EXPECT_TRUE(expr8.isValid() == true);
    EXPECT_TRUE(expr8.returnType().isString() == true);
    EXPECT_TRUE(expr8.isConstant() == true);
    EXPECT_STREQ(expr8.evalStr(), "ok");
}
