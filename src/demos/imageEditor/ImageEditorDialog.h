// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0

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
