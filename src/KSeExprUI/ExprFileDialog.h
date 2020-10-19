// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

// NOTE: This is based on Dan's paint3d FileDialog

#ifndef EXPRFILEDIALOG_H
#define EXPRFILEDIALOG_H

#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QPixmap>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QPushButton>

class ExprPreviewWidget : public QWidget {
  public:
    ExprPreviewWidget(QWidget* parent);
    void makePreview(const QString& path);
    void reset() { _pm->setPixmap(QPixmap()); }

  private:
    QLabel* _pm;
};

class ExprFileDialog : public QFileDialog {
    Q_OBJECT
  public:
    ExprFileDialog(QWidget* parent = 0);
    void addLookInEntries(QStringList paths);
    void saveLookInEntries();
    void restoreLookInEntries();
    QString getOpenFileName(const QString& caption = QString(),
                            const QString& startWith = QString(),
                            const QString& filter = QString());
    QString getExistingDirectory(const QString& caption = QString(),
                                 const QString& startWith = QString(),
                                 const QString& filter = QString());
    QString getExistingOrNewDirectory(const QString& caption = QString(),
                                      const QString& startWith = QString(),
                                      const QString& filter = QString());
    QStringList getOpenFileNames(const QString& caption = QString(),
                                 const QString& startWith = QString(),
                                 const QString& filter = QString());
    QString getSaveFileName(const QString& caption = QString(),
                            const QString& startWith = QString(),
                            const QString& filter = QString());
    void setPreview();
    void resetPreview();
    void addCheckBox(QString s);
    void addFavoritesButton(QString dirname, QString linkname, QString linkdir);
    bool checkBoxStatus();
    void showCheckBox();
    void hideCheckBox();
    void addComboBox(QString s, QStringList sl);
    void showComboBox();
    void hideComboBox();
    QComboBox* getComboBox() { return _combo; }
    void setButtonName(const QString& str);
    void addSidebarShortcut(const QString& s);

  private
Q_SLOTS:
    void handleOk();
    void editReturnPress();
    void gotoFavorites();
    void selChanged(const QString& path);
    void resetDir() {
        if (!_temppath.isEmpty()) setDirectory(_temppath);
        _temppath = QString();
    }

  private:
    QString _workingDirectory, _favDir;
    QString _temppath;
    QStringList _lookInList;
    QLineEdit* _nameEdit;
    QPushButton* _okButton;
    bool _createDir;
    ExprPreviewWidget* _pw;
    QCheckBox* _cb;
    QLabel* _combolabel;
    QComboBox* _combo;
};

#endif
