// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

/**
   @file ImageEditorDialog.h
*/

#include <memory>
#include <QDialog>

class QLabel;
class ExprEditor;
class ImageSynthesizer;

class ImageEditorDialog : public QDialog
{
    Q_OBJECT
public:
    ImageEditorDialog(QWidget *parent = nullptr);

protected Q_SLOTS:
    void applyExpression();

private:
    std::shared_ptr<std::vector<unsigned char>> imageData{nullptr};
    QLabel *_imageLabel {nullptr};
    ExprEditor *_editor {nullptr};
    ImageSynthesizer *_imageSynthesizer {nullptr};
};
