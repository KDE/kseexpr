// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

/**
   @file imageEditor.cpp
*/

#include <iostream>
#include <string>
#include <png.h>

#include <QApplication>
#include <QDialog>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QImage>
#include <QPushButton>
#include <QMessageBox>

#include <KSeExpr/Expression.h>
#include <KSeExprUI/ErrorMessages.h>
#include <KSeExprUI/ExprControlCollection.h>
#include <KSeExprUI/ExprEditor.h>
#include <KSeExprUI/ExprBrowser.h>

#include "ImageEditorDialog.h"

//-- IMAGE SYNTHESIZER CLASSES AND METHODS --//

double clamp(double x) { return std::max(0., std::min(255., x)); }

// Simple image synthesizer expression class to support demo image editor
class ImageSynthExpression : public KSeExpr::Expression {
  public:
    // Constructor that takes the expression to parse
    ImageSynthExpression(const std::string &expr) : KSeExpr::Expression(expr) {}

    // Simple variable that just returns its internal value
    struct Var : public KSeExpr::ExprVarRef {
        Var(const double val) : KSeExpr::ExprVarRef(KSeExpr::ExprType().FP(1).Varying()), val(val) {}
        Var() : KSeExpr::ExprVarRef(KSeExpr::ExprType().FP(1).Varying()), val(0.0) {}
        double val;  // independent variable
        void eval(double *result) { result[0] = val; }
        void eval(const char **result) { assert(false); }
    };
    // variable map
    mutable std::map<std::string, Var> vars;

    // resolve function that only supports one external variable 'x'
    KSeExpr::ExprVarRef *resolveVar(const std::string &name) const {
        std::map<std::string, Var>::iterator i = vars.find(name);
        if (i != vars.end()) return &i->second;
        return 0;
    }
};

class ImageSynthesizer {
  public:
    ImageSynthesizer();
    unsigned char *evaluateExpression(const std::string &exprStr);

  private:
    int _width;
    int _height;
};

ImageSynthesizer::ImageSynthesizer() {
    _width = 256;
    _height = 256;
}

unsigned char *ImageSynthesizer::evaluateExpression(const std::string &exprStr) {
    ImageSynthExpression expr(exprStr);

    // make variables
    expr.vars["u"] = ImageSynthExpression::Var(0.);
    expr.vars["v"] = ImageSynthExpression::Var(0.);
    expr.vars["w"] = ImageSynthExpression::Var(_width);
    expr.vars["h"] = ImageSynthExpression::Var(_height);

    // check if expression is valid
    bool valid = expr.isValid();
    if (!valid) {
        std::cerr << "Invalid expression " << std::endl;
        auto message = ErrorMessages::message(expr.parseError());
        for(auto arg: expr.parseErrorArgs()) {
            message = message.arg(QString::fromStdString(arg));
        }
        std::cerr << "Parse error: " << message.toStdString() << std::endl;
        for (auto occurrence : expr.getErrors()) {
            QString message = ErrorMessages::message(occurrence.error);
            for (auto arg : occurrence.ids) {
                message = message.arg(QString::fromStdString(arg));
            }
            std::cerr << "Prep error: " << message.toStdString() << std::endl;
        }
        return NULL;
    }

    // evaluate expression
    std::cerr << "Evaluating expression..." << std::endl;
    unsigned char *image = new unsigned char[_width * _height * 4];
    double one_over_width = 1. / _width, one_over_height = 1. / _height;
    double &u = expr.vars["u"].val;
    double &v = expr.vars["v"].val;
    unsigned char *pixel = image;
    for (int row = 0; row < _height; row++) {
        for (int col = 0; col < _width; col++) {
            u = one_over_width * (col + .5);
            v = one_over_height * (row + .5);
            KSeExpr::Vec3d result = KSeExpr::Vec3dConstRef(expr.evalFP());
            pixel[0] = clamp(result[2] * 256.);
            pixel[1] = clamp(result[1] * 256.);
            pixel[2] = clamp(result[0] * 256.);
            pixel[3] = 255;
            pixel += 4;
        }
    }

    return image;
}

//-- IMAGE EDITOR DIALOG METHODS --//

ImageEditorDialog::ImageEditorDialog(QWidget *parent) : QDialog(parent) {
    _imageSynthesizer = new ImageSynthesizer();

    this->setWindowTitle("Image Synthesis Editor");

    // Image Previewer
    _imageLabel = new QLabel();
    _imageLabel->setFixedSize(256, 256);
    _imageLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    QWidget *imagePreviewWidget = new QWidget();
    QHBoxLayout *imagePreviewLayout = new QHBoxLayout(imagePreviewWidget);
    imagePreviewLayout->addStretch();
    imagePreviewLayout->addWidget(_imageLabel);
    imagePreviewLayout->addStretch();

    // Expression controls
    ExprControlCollection *controls = new ExprControlCollection();
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setMinimumHeight(100);
    scrollArea->setFixedWidth(450);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(controls);

    // Expression editor
    _editor = new ExprEditor(this);
    _editor->setControlCollectionWidget(controls);

    // Expression browser
    ExprBrowser *browser = new ExprBrowser(0, _editor);

    // Add user expressions, example expressions to browser list.
    browser->addUserExpressionPath("imageEditor");
#ifdef IMAGE_EDITOR_ROOT
    std::string exPathStr = IMAGE_EDITOR_ROOT;
    exPathStr += "/share/KSeExpr/expressions";
    browser->addPath("Examples", exPathStr);
#else
    browser->addPath("Examples", "./src/demos/imageEditor");
#endif
    browser->update();

    // Create apply button and connect to image preview.
    QPushButton *applyButton = new QPushButton("Apply");
    connect(applyButton, SIGNAL(clicked()), (ImageEditorDialog *)this, SLOT(applyExpression()));

    // Layout widgets: Top section contains left side with previewer and
    // controls, right side with browser.  Bottom section contains editor
    // and apply button.
    QVBoxLayout *rootLayout = new QVBoxLayout();
    this->setLayout(rootLayout);

    QWidget *topWidget = new QWidget();
    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->setContentsMargins(0, 0, 0, 0);
    topWidget->setLayout(topLayout);

    QWidget *leftWidget = new QWidget();
    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftWidget->setLayout(leftLayout);
    leftLayout->addWidget(imagePreviewWidget);
    leftLayout->addWidget(scrollArea, 1);

    QWidget *bottomWidget = new QWidget();
    QVBoxLayout *bottomLayout = new QVBoxLayout();
    bottomLayout->setContentsMargins(0, 0, 0, 0);
    bottomWidget->setLayout(bottomLayout);

    QWidget *buttonWidget = new QWidget();
    QHBoxLayout *buttonLayout = new QHBoxLayout(0);
    buttonWidget->setLayout(buttonLayout);
    buttonLayout->addWidget(applyButton);

    topLayout->addWidget(leftWidget);
    topLayout->addWidget(browser, 1);

    bottomLayout->addWidget(_editor);
    bottomLayout->addWidget(buttonWidget);

    rootLayout->addWidget(topWidget);
    rootLayout->addWidget(bottomWidget);
}

// Apply expression, if any, from the editor contents to the preview image
void ImageEditorDialog::applyExpression() {
    std::string exprStr = _editor->getExpr().toStdString();
    if (exprStr.empty()) {
        QMessageBox msgBox;
        msgBox.setText("No expression entered in the editor.");
        msgBox.exec();
    } else {
        QImage image(_imageSynthesizer->evaluateExpression(exprStr), 256, 256, QImage::Format_RGB32);
        if (image.isNull()) {
            QMessageBox msgBox;
            msgBox.setText("Error evaluating expression to create preview image.");
            msgBox.exec();
        } else {
            QPixmap imagePixmap = QPixmap::fromImage(image);
            _imageLabel->setPixmap(imagePixmap);
        }
    }
}

//-- MAIN --//

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    ImageEditorDialog *dialog = new ImageEditorDialog(0);
    dialog->show();
    app.exec();
    return 0;
}
