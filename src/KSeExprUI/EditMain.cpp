// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0

#include <iostream>
#include <string>

#include <QApplication>
#include "SeExprEdDialog.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    SeExprEdDialog dialog(0);
    dialog.setWindowTitle("Expression Editor");
    dialog.show();

    if (argc < 2 || std::string(argv[1]) != "-automatedTest") {
        if (dialog.exec() == QDialog::Accepted)
            std::cerr << "returned expression: " << dialog.getExpressionString() << std::endl;
    } else {
        std::string str = "$u + $v";
        dialog.setExpressionString(str);
        if (dialog.getExpressionString() != str) {
            std::cerr << "test failed: " << dialog.getExpressionString() << " != " << str << std::endl;
            return 1;
        }
    }

    return 0;
}
