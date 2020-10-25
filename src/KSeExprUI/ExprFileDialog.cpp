// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

// NOTE: This is based on Dan's paint3d FileDialog

#include "ExprFileDialog.h"

#include <QMenu>
#include <QPalette>
#include <QTimer>
#include <QToolButton>
#include <QUrl>
#include <array>
#include <iostream>

using std::max;
using std::min;

static const std::array<const char *, 21> folder_fav = {"17 16 4 1",         "# c #000000",       ". c None",          "a c #ffff98",       "b c #cc0000",       ".................", ".................",
                                                        "...#####.........", "..#aaaaa#........", ".###############.", ".#aaaaaaaaaaaaa#.", ".#aaaa##a##aaaa#.", ".#aaa#bb#bb#aaa#.", ".#aaa#bbbbb#aaa#.",
                                                        ".#aaa#bbbbb#aaa#.", ".#aaaa#bbb#aaaa#.", ".#aaaaa#b#aaaaa#.", ".#aaaaaa#aaaaaa#.", ".#aaaaaaaaaaaaa#.", ".###############.", "................."};

void ExprPreviewWidget::makePreview(const QString &path)
{
    QFileInfo fi(path);

    if (fi.isDir()) {
        QString s = fi.absoluteFilePath() + QString::fromLatin1("/preview.tif");
        if (!QFile::exists(s))
            s = fi.absoluteFilePath() + QString::fromLatin1("/preview.png");
        if (!QFile::exists(s))
            _pm->setPixmap(QPixmap()); // nothing to preview

        QPixmap pix(s);
        if (!pix.isNull())
            _pm->setPixmap(pix);
        else
            _pm->setPixmap(QPixmap());
    } else if (fi.exists()) {
        QImage img(fi.absoluteFilePath());
        if (!img.isNull())
            _pm->setPixmap(QPixmap::fromImage(img.scaled(128, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
        else
            _pm->setPixmap(QPixmap());
    } else
        _pm->setPixmap(QPixmap());
    _pm->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
}

ExprPreviewWidget::ExprPreviewWidget(QWidget *parent)
    : QWidget(parent)
{
    _pm = new QLabel(this);
    _pm->setFrameStyle(QFrame::StyledPanel);
    _pm->setBackgroundRole(QPalette::Base);
    _pm->setAutoFillBackground(true);
    auto *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(_pm);
    setLayout(layout);
}

ExprFileDialog::ExprFileDialog(QWidget *parent)
    : QFileDialog(parent)
{
    // QStringList pathlist(QString(globals.startpath.c_str()));
    // addLookInEntries(pathlist);

    // disconnect broken return press handling (mishandles new directory names)
    QList<QLineEdit *> lineedits = findChildren<QLineEdit *>();
    if (!lineedits.empty())
        _nameEdit = (QLineEdit *)lineedits.at(0);
    if (_nameEdit) {
        _nameEdit->disconnect(SIGNAL(returnPressed()));
        connect(_nameEdit, SIGNAL(returnPressed()), SLOT(editReturnPress()));
    }

    // connect custom ok clicked handler
    QList<QPushButton *> myWidgets = findChildren<QPushButton *>();
    for (auto *const item : myWidgets) {
        if (item->text().contains(tr("Open")))
            _okButton = item;
    }
    if (_okButton)
        connect(_okButton, SIGNAL(clicked()), SLOT(handleOk()));

    connect(this, SIGNAL(currentChanged(const QString &)), this, SLOT(selChanged(const QString &)));

    // don't create missing directories by default
    _favDir = QString();
    _temppath = QString();

    setMinimumWidth(680);
    resize(840, 440);
}

void ExprFileDialog::handleOk()
{
    if (fileMode() != QFileDialog::DirectoryOnly)
        return;
    QString entry = _nameEdit->text();
    if (entry.isEmpty())
        return;

    // create directory if needed
    if (_createDir) {
        QDir d(directory());
        if (!d.exists(entry)) {
            if (d.mkdir(entry)) {
                _temppath = directory().absolutePath();
                setDirectory(_temppath + QLatin1Char('/') + entry);
                _nameEdit->setText(QString());
                if (_okButton)
                    _okButton->animateClick(); // retry click to accept entry

                QTimer::singleShot(200, this, SLOT(resetDir()));
            }
        }
    }
}

void ExprFileDialog::editReturnPress()
{
    if (!_nameEdit)
        return;

    QString str = _nameEdit->text();
    if (str.contains(QLatin1Char('/'))) {
        QDir d;
        if (d.cd(str)) {
            setDirectory(str);
            _nameEdit->setText(QString());
        } else {
            int slashcount = str.count(QLatin1Char('/'));

            QString foundDir;
            for (int i = 0; i < slashcount; i++) {
                QString section = str.section(QLatin1Char('/'), 0, i);
                if (d.cd(section))
                    foundDir = section;
            }
            if (foundDir.length()) {
                setDirectory(foundDir);
                QString remainder = str.right(str.length() - (foundDir.length() + 1));
                _nameEdit->setText(remainder);
            }

            if (d.cd(str))
                setDirectory(str);
        }
    } else if (fileMode() == QFileDialog::DirectoryOnly)
        handleOk();
    else
        accept();
}

void ExprFileDialog::addFavoritesButton(const QString &dirname, const QString &linkname, const QString &linkdir)
{
    auto *layout = findChild<QGridLayout *>(QString::fromLatin1("gridLayout"));
    if (!layout)
        return;

    QDir d;

    std::string favlocation = getenv("HOME");
    favlocation += "/paint3d/favorites/";

    QString dirpath = QString::fromStdString(favlocation);
    if (!d.cd(dirpath))
        d.mkpath(dirpath);
    dirpath += dirname;
    if (!d.cd(dirpath))
        d.mkpath(dirpath);

    if (!(linkdir.isEmpty() || linkname.isEmpty())) {
        if (!QFile::exists(dirpath + linkname))
            QFile::link(linkdir, dirpath + linkname);
    }

    _favDir = dirpath;

    static QPixmap folderFav(folder_fav.data());
    auto *fav = new QToolButton(this);
    fav->setFixedSize(18, 18);
    fav->setIcon(folderFav);
    fav->setToolTip(tr("Favorites"));

    layout->addWidget(fav, 0, 3);

    connect(fav, SIGNAL(clicked()), SLOT(gotoFavorites()));
}

void ExprFileDialog::gotoFavorites()
{
    if (!_favDir.isEmpty())
        setDirectory(_favDir);
}

void ExprFileDialog::addLookInEntries(const QStringList &paths)
{
    if (paths.isEmpty())
        return;

    QStringList h = history();
    for (const auto &it : paths) {
        if (!h.contains(it))
            h.push_back(it);
    }
    setHistory(h);
}

void ExprFileDialog::saveLookInEntries()
{
    _lookInList = history();
}

void ExprFileDialog::restoreLookInEntries()
{
    setHistory(_lookInList);
}

static QStringList makeFiltersList(const QString &filter)
{
    if (filter.isEmpty())
        return QStringList();

    int i = filter.indexOf(QString::fromLatin1(";;"), 0);
    QString sep = QString::fromLatin1(";;");
    if (i == -1) {
        if (filter.indexOf(QString::fromLatin1("\n"), 0) != -1) {
            sep = QString::fromLatin1("\n");
            i = filter.indexOf(sep, 0);
        }
    }

    return filter.split(sep);
}

QString ExprFileDialog::getOpenFileName(const QString &caption, const QString &startWith, const QString &filter)
{
    if (!filter.isEmpty()) {
        QStringList filters = makeFiltersList(filter);
        setNameFilters(filters);
    }

    if (!startWith.isEmpty())
        setDirectory(startWith);
    if (!caption.isNull())
        setWindowTitle(caption);
    setFileMode(QFileDialog::ExistingFile);
    setAcceptMode(QFileDialog::AcceptOpen);
    selectFile(QString());

    QString result;
    if (exec() == QDialog::Accepted) {
        result = selectedFiles().first();
        _workingDirectory = directory().absolutePath();
    }
    resetPreview();

    return result;
}

QStringList ExprFileDialog::getOpenFileNames(const QString &caption, const QString &startWith, const QString &filter)
{
    if (!filter.isEmpty()) {
        QStringList filters = makeFiltersList(filter);
        setNameFilters(filters);
    }

    if (!startWith.isEmpty())
        setDirectory(startWith);
    if (!caption.isNull())
        setWindowTitle(caption);
    setFileMode(QFileDialog::ExistingFiles);
    setAcceptMode(QFileDialog::AcceptOpen);
    selectFile(QString());

    QString result;
    QStringList lst;
    if (exec() == QDialog::Accepted) {
        lst = selectedFiles();
        _workingDirectory = directory().absolutePath();
    }
    resetPreview();

    return lst;
}

QString ExprFileDialog::getExistingDirectory(const QString &caption, const QString &startWith, const QString &filter)
{
    if (!filter.isEmpty()) {
        QStringList filters = makeFiltersList(filter);
        setNameFilters(filters);
    }

    if (!startWith.isEmpty())
        setDirectory(startWith);
    if (!caption.isNull())
        setWindowTitle(caption);
    setFileMode(QFileDialog::DirectoryOnly);
    selectFile(QString());

    QString result;
    if (exec() == QDialog::Accepted) {
        result = selectedFiles().first();
        _workingDirectory = directory().absolutePath();
    }
    resetPreview();

    return result;
}

QString ExprFileDialog::getExistingOrNewDirectory(const QString &caption, const QString &startWith, const QString &filter)
{
    _createDir = true;
    QString result = getExistingDirectory(caption, startWith, filter);
    _createDir = false;
    resetPreview();
    return result;
}

QString ExprFileDialog::getSaveFileName(const QString &caption, const QString &startWith, const QString &filter)
{
    if (!filter.isEmpty()) {
        QStringList filters = makeFiltersList(filter);
        setNameFilters(filters);
    }

    if (!startWith.isEmpty())
        setDirectory(startWith);
    if (!caption.isNull())
        setWindowTitle(caption);
    setFileMode(QFileDialog::AnyFile);
    setAcceptMode(QFileDialog::AcceptSave);
    selectFile(QString());

    QString result;
    if (exec() == QDialog::Accepted) {
        result = selectedFiles().first();
        _workingDirectory = directory().absolutePath();
    }
    resetPreview();

    return result;
}

void ExprFileDialog::setPreview()
{
    auto *layout = findChild<QGridLayout *>(QString::fromLatin1("gridLayout"));
    if (!layout)
        return;

    _pw = new ExprPreviewWidget(this);
    _pw->setFixedWidth(160);
    _pw->setMinimumHeight(160);
    layout->addWidget(_pw, 1, 3);
}

void ExprFileDialog::resetPreview()
{
    if (_pw)
        _pw->reset();
}

void ExprFileDialog::addCheckBox(const QString &s)
{
    auto *layout = findChild<QGridLayout *>(QString::fromLatin1("gridLayout"));
    if (!layout)
        return;

    _cb = new QCheckBox(s, this);
    _cb->setChecked(false);

    layout->addWidget(_cb, 4, _combo ? 2 : 0);
}

bool ExprFileDialog::checkBoxStatus()
{
    if (!_cb)
        return false;
    return _cb->isChecked();
}

void ExprFileDialog::showCheckBox()
{
    if (_cb)
        _cb->show();
}

void ExprFileDialog::hideCheckBox()
{
    if (_cb)
        _cb->hide();
}

void ExprFileDialog::addComboBox(const QString &s, const QStringList &sl)
{
    auto *layout = findChild<QGridLayout *>(QString::fromLatin1("gridLayout"));
    if (!layout)
        return;

    _combolabel = new QLabel(s, this);
    _combolabel->setFixedWidth(58);
    _combo = new QComboBox(this);
    _combo->setEditable(true);
    _combo->setFixedWidth(160);
    for (const auto &it : sl)
        _combo->addItem(it);

    int rownum = layout->rowCount();
    layout->addWidget(_combo, rownum, 1);
    layout->addWidget(_combolabel, rownum, 0);
}

void ExprFileDialog::showComboBox()
{
    if (_combo)
        _combo->show();
    if (_combolabel)
        _combolabel->show();
}

void ExprFileDialog::hideComboBox()
{
    if (_combo)
        _combo->hide();
    if (_combolabel)
        _combolabel->hide();
}

void ExprFileDialog::selChanged(const QString &path)
{
    if (_pw)
        _pw->makePreview(path);
}

void ExprFileDialog::setButtonName(const QString &str)
{
    if (_okButton)
        _okButton->setText(str);
}

void ExprFileDialog::addSidebarShortcut(const QString &s)
{
    QList<QUrl> urls = sidebarUrls();
    QUrl url = QUrl::fromLocalFile(s);
    if (url.isValid() && QFile::exists(s)) {
        urls.push_back(url);
        setSidebarUrls(urls);
    }
}
