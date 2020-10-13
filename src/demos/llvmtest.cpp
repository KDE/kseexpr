// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0

#include <Expression.h>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;
using namespace KSeExpr;

class llvmexpr : public Expression {
  public:
    //! Constructor that takes the expression to parse
    llvmexpr(const std::string &expr, const ExprType &type = ExprType().FP(3)) : Expression(expr, type) {}
};

// TODO: turn off parsing output.
int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "need a filename as argument\n";
        return 1;
    }

    // Test fp
    std::string line;
    std::ifstream FSProfileData(argv[1], std::ifstream::in);
    while (std::getline(FSProfileData, line)) {
        if (!line.size() || line.at(0) == ';') continue;

        string::size_type start = line.find_first_of('"');
        ++start;
        string::size_type end = line.find_last_of('"');
        string exprStr(line, start, end - start);

        std::cout << left << "\n";
        std::istringstream iss(string(line, end + 1), std::istringstream::in);
        int dim = 0;
        iss >> dim;
        std::cout << "exprStr: " << exprStr << std::endl;
        std::cout << "dim: " << dim << std::endl;

        llvmexpr expr(exprStr, ExprType().FP(dim));
        if (!expr.isValid()) {
            std::cerr << expr.parseError() << std::endl;
            assert(false);
        }

        const double *result = expr.evalFP();

        for (int i = 0; i < dim; ++i) std::cout << result[i] << ' ';
        std::cout << std::endl;
    }

    if (!argv[2]) return 0;
    FSProfileData.close();

    // Test string
    FSProfileData.open(argv[2], std::ifstream::in);
    while (std::getline(FSProfileData, line)) {
        if (!line.size() || line.at(0) == ';') continue;

        std::cout << "exprStr: " << line << std::endl;
        llvmexpr expr(line, ExprType().String());
        if (!expr.isValid()) {
            std::cerr << expr.parseError() << std::endl;
            assert(false);
        }

        const char *result = expr.evalStr();

        std::cout << result;
    }
}
