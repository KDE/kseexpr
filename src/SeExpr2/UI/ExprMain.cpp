/*
* Copyright Disney Enterprises, Inc.  All rights reserved.
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
*/

#include <QApplication>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#include "ExprDialog.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    ExprDialog dialog(0);
    dialog.setWindowTitle("Expression Editor 2");
    dialog.show();

    if (argc < 2 || std::string(argv[1]) != "-automatedTest") {
        if (dialog.exec() == QDialog::Accepted)
            std::cerr << "returned expression: " << dialog.getExpressionString().toStdString() << std::endl;
    } else {
        std::string str = "$u + $v";
        dialog.setExpressionString(QString::fromStdString(str));
        if (dialog.getExpressionString() != QString::fromStdString(str)) {
            std::cerr << "test failed: " << dialog.getExpressionString().toStdString() << " != " << str << std::endl;
            return 1;
        }
    }

    return 0;
}
