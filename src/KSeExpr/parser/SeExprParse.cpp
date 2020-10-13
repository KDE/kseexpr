// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
#include "SeExprParse.h"
#include "ASTNode.h"

// typedef std::unique_ptr<ASTNode> ASTPtr;

//#ifdef TEST_PARSER

int main(int argc, char* argv[]) {
    std::ifstream ifs(argv[1]);
    std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    // std::cerr<<"PARSING! '"<<content<<"'"<<std::endl;;
    SeParser<ASTPolicy> parser(content);
    try {
        auto tree = parser.parse();
        tree->print(std::cout, 0, &content);
    }
    catch (const ParseError& e) {
        std::cerr << "parse error: " << e._errorStr << std::endl;
    }
    return 0;
}
//#endif
