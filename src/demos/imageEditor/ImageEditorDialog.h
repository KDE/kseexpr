// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

/**
   @file ImageEditorDialog.h
*/

#include <QDialog>

class QLabel;
class ExprEditor;
class ImageSynthesizer;

class ImageEditorDialog : public QDialog {
    Q_OBJECT
  public:
    ImageEditorDialog(QWidget *parent = 0);

  private:
    QLabel *_imageLabel;
    ExprEditor *_editor;
    ImageSynthesizer *_imageSynthesizer;
  private
Q_SLOTS:
    void applyExpression();
};
