// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

// NOTE: This is based on Dan's paint3d FileDialog

#pragma once

#include <QCheckBox>
#include <QComboBox>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>
#include <atomic>

class ExprPreviewWidget : public QWidget
{
public:
    ExprPreviewWidget(QWidget *parent);
    void makePreview(const QString &path);
    void reset()
    {
        _pm->setPixmap(QPixmap());
    }

private:
    QLabel *_pm;
};

class ExprFileDialog : public QFileDialog
{
    Q_OBJECT
public:
    ExprFileDialog(QWidget *parent = nullptr);
    void addLookInEntries(const QStringList &paths);
    void saveLookInEntries();
    void restoreLookInEntries();
    QString getOpenFileName(const QString &caption = QString(), const QString &startWith = QString(), const QString &filter = QString());
    QString getExistingDirectory(const QString &caption = QString(), const QString &startWith = QString(), const QString &filter = QString());
    QString getExistingOrNewDirectory(const QString &caption = QString(), const QString &startWith = QString(), const QString &filter = QString());
    QStringList getOpenFileNames(const QString &caption = QString(), const QString &startWith = QString(), const QString &filter = QString());
    QString getSaveFileName(const QString &caption = QString(), const QString &startWith = QString(), const QString &filter = QString());
    void setPreview();
    void resetPreview();
    void addCheckBox(const QString &s);
    void addFavoritesButton(const QString &dirname, const QString &linkname, const QString &linkdir);
    bool checkBoxStatus();
    void showCheckBox();
    void hideCheckBox();
    void addComboBox(const QString &s, const QStringList &sl);
    void showComboBox();
    void hideComboBox();
    QComboBox *getComboBox()
    {
        return _combo;
    }
    void setButtonName(const QString &str);
    void addSidebarShortcut(const QString &s);

private Q_SLOTS:
    void handleOk();
    void editReturnPress();
    void gotoFavorites();
    void selChanged(const QString &path);
    void resetDir()
    {
        if (!_temppath.isEmpty())
            setDirectory(_temppath);
        _temppath = QString();
    }

private:
    QString _workingDirectory, _favDir;
    QString _temppath;
    QStringList _lookInList;
    QLineEdit *_nameEdit {nullptr};
    QPushButton *_okButton {nullptr};
    std::atomic<bool> _createDir{};
    ExprPreviewWidget *_pw {nullptr};
    QCheckBox *_cb {nullptr};
    QLabel *_combolabel {nullptr};
    QComboBox *_combo {nullptr};
};
