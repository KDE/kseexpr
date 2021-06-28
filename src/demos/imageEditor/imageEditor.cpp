// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

/**
   @file imageEditor.cpp
*/

#include <iostream>
#include <string>

#include <QApplication>
#include <QDialog>
#include <QDir>
#include <QImage>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

#include <KSeExpr/Expression.h>
#include <KSeExprUI/ErrorMessages.h>
#include <KSeExprUI/ExprBrowser.h>
#include <KSeExprUI/ExprControlCollection.h>
#include <KSeExprUI/ExprEditor.h>

#include "ImageEditorDialog.h"

//-- IMAGE SYNTHESIZER CLASSES AND METHODS --//

constexpr double clamp(double x)
{
    return std::max(0., std::min(255., x)); // NOLINT readability-magic-numbers
}

// Simple image synthesizer expression class to support demo image editor
class ImageSynthExpression : public KSeExpr::Expression
{
public:
    // Constructor that takes the expression to parse
    ImageSynthExpression(const std::string &expr)
        : KSeExpr::Expression(expr)
    {
    }

    // Simple variable that just returns its internal value
    struct Var : public KSeExpr::ExprVarRef {
        Var(const double val)
            : KSeExpr::ExprVarRef(KSeExpr::ExprType().FP(1).Varying())
            , val(val)
        {
        }
        Var()
            : KSeExpr::ExprVarRef(KSeExpr::ExprType().FP(1).Varying())
        {
        }
        double val {0.0}; // independent variable
        void eval(double *result) override
        {
            result[0] = val;
        }
        void eval(const char **) override
        {
            assert(false);
        }
    };
    // variable map
    mutable std::map<std::string, Var> vars;

    // resolve function that only supports one external variable 'x'
    KSeExpr::ExprVarRef *resolveVar(const std::string &name) const override
    {
        auto i = vars.find(name);
        if (i != vars.end())
            return &i->second;
        return nullptr;
    }
};

class ImageSynthesizer
{
public:
    using ImageData = std::shared_ptr<std::vector<unsigned char>>;
    ImageSynthesizer() = default;
    ImageData evaluateExpression(const std::string &exprStr) const
    {
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
            for (const auto &arg : expr.parseErrorArgs()) {
                message = message.arg(QString::fromStdString(arg));
            }
            std::cerr << "Parse error: " << message.toStdString() << std::endl;
            for (const auto &occurrence : expr.getErrors()) {
                QString message = ErrorMessages::message(occurrence.error);
                for (const auto &arg : occurrence.ids) {
                    message = message.arg(QString::fromStdString(arg));
                }
                std::cerr << "Prep error: " << message.toStdString() << std::endl;
            }
            return nullptr;
        }

        // evaluate expression
        std::cerr << "Evaluating expression..." << std::endl;
        std::vector<unsigned char> image(_width * _height * 4);
        double one_over_width = 1. / _width;
        double one_over_height = 1. / _height;
        double &u = expr.vars["u"].val;
        double &v = expr.vars["v"].val;
        for (size_t row {}; row < _height; row++) {
            for (size_t col {}; col < _width; col++) {
                auto i = (row * _width + col) * 4;
                u = one_over_width * (col + .5);  // NOLINT readability-magic-constants
                v = one_over_height * (row + .5); // NOLINT readability-magic-constants
                KSeExpr::Vec3d result = KSeExpr::Vec3dConstRef(expr.evalFP());
                image[i] = clamp(result[2] * 256.);     // NOLINT readability-magic-numbers
                image[i + 1] = clamp(result[1] * 256.); // NOLINT readability-magic-numbers
                image[i + 2] = clamp(result[0] * 256.); // NOLINT readability-magic-numbers
                image[i + 3] = 255;                     // NOLINT readability-magic-numbers
            }
        }

        return std::make_shared<std::vector<unsigned char>>(image);
    }

private:
    size_t _width {256};  // NOLINT readability-magic-numbers
    size_t _height {256}; // NOLINT readability-magic-numbers
};

//-- IMAGE EDITOR DIALOG METHODS --//

ImageEditorDialog::ImageEditorDialog(QWidget *parent)
    : QDialog(parent)
{
    _imageSynthesizer = new ImageSynthesizer();

    this->setWindowTitle("Image Synthesis Editor");

    // Image Previewer
    _imageLabel = new QLabel();
    _imageLabel->setFixedSize(256, 256); // NOLINT readability-magic-numbers
    _imageLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    auto *imagePreviewWidget = new QWidget();
    auto *imagePreviewLayout = new QHBoxLayout(imagePreviewWidget);
    imagePreviewLayout->addStretch();
    imagePreviewLayout->addWidget(_imageLabel);
    imagePreviewLayout->addStretch();

    // Expression controls
    auto *controls = new ExprControlCollection();
    auto *scrollArea = new QScrollArea();
    scrollArea->setMinimumHeight(100); // NOLINT readability-magic-numbers
    scrollArea->setFixedWidth(450);    // NOLINT readability-magic-numbers
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(controls);

    // Expression editor
    _editor = new ExprEditor(this);
    _editor->setControlCollectionWidget(controls);

    // Expression browser
    auto *browser = new ExprBrowser(nullptr, _editor);

    // Add user expressions, example expressions to browser list.
    browser->addUserExpressionPath("imageEditor");
#ifdef IMAGE_EDITOR_ROOT
    browser->addPath("Examples", QDir::toNativeSeparators(QString("%1/share/KSeExpr/expressions").arg(IMAGE_EDITOR_ROOT)).toStdString());
#else
    browser->addPath("Examples", "./src/demos/imageEditor");
#endif
    browser->update();

    // Create apply button and connect to image preview.
    auto *applyButton = new QPushButton("Apply");
    connect(applyButton, SIGNAL(clicked()), (ImageEditorDialog *)this, SLOT(applyExpression()));

    // Layout widgets: Top section contains left side with previewer and
    // controls, right side with browser.  Bottom section contains editor
    // and apply button.
    auto *rootLayout = new QVBoxLayout();
    this->setLayout(rootLayout);

    auto *topWidget = new QWidget();
    auto *topLayout = new QHBoxLayout();
    topLayout->setContentsMargins(0, 0, 0, 0);
    topWidget->setLayout(topLayout);

    auto *leftWidget = new QWidget();
    auto *leftLayout = new QVBoxLayout();
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftWidget->setLayout(leftLayout);
    leftLayout->addWidget(imagePreviewWidget);
    leftLayout->addWidget(scrollArea, 1);

    auto *bottomWidget = new QWidget();
    auto *bottomLayout = new QVBoxLayout();
    bottomLayout->setContentsMargins(0, 0, 0, 0);
    bottomWidget->setLayout(bottomLayout);

    auto *buttonWidget = new QWidget();
    auto *buttonLayout = new QHBoxLayout(nullptr);
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
void ImageEditorDialog::applyExpression()
{
    std::string exprStr = _editor->getExpr().toStdString();
    if (exprStr.empty()) {
        QMessageBox::information(this, this->windowTitle(), "No expression entered in the editor.");
    } else {
        auto data = _imageSynthesizer->evaluateExpression(exprStr);
        if (!data) {
            QMessageBox::critical(this, this->windowTitle(), "Error evaluating expression to create preview image.");
        } else {
            QImage image(data->data(), 256, 256, QImage::Format_RGB32); // NOLINT readability-magic-numbers
            QPixmap imagePixmap = QPixmap::fromImage(image);
            _imageLabel->setPixmap(imagePixmap);
            // amyspark: ensure the old image gets dropped only after it was replaced
            imageData = data;
        }
    }
}

//-- MAIN --//

int main(int argc, char *argv[])
{
    auto app = std::make_unique<QApplication>(argc, argv);
    auto dialog = std::make_unique<ImageEditorDialog>(nullptr);
    dialog->show();
    app->exec();
    return 0;
}
