// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * @file ExprBrowser.cpp
 * @brief Qt browser widget for list of expressions
 * @author  aselle
 */
#include <array>
#include <cassert>

#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSizePolicy>
#include <QSortFilterProxyModel>
#include <QSpacerItem>
#include <QTabWidget>
#include <QTextBrowser>
#include <QTextStream>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>


#include "Debug.h"
#include "ExprBrowser.h"
#include "ExprEditor.h"

#define P3D_CONFIG_ENVVAR "P3D_CONFIG_PATH"

class ExprTreeItem
{
public:
    ExprTreeItem(ExprTreeItem *parent, const QString &label, const QString &path)
        : row(-1)
        , parent(parent)
        , label(label)
        , path(path)
        , populated(parent == nullptr)
    {
    }

    ~ExprTreeItem()
    {
        for (unsigned int i = 0; i < children.size(); i++)
            delete children[i];
    }

    ExprTreeItem *find(QString path)
    {
        if (this->path == path)
            return this;
        else {
            populate();
            for (auto & i : children) {
                ExprTreeItem *ret = i->find(path);
                if (ret)
                    return ret;
            }
        }
        return nullptr;
    }

    void clear()
    {
        for (unsigned int i = 0; i < children.size(); i++) {
            delete children[i];
        }
        children.clear();
    }

    void populate()
    {
        if (populated)
            return;
        populated = true;
        QFileInfo info(path);
        if (info.isDir()) {
            QFileInfoList infos = QDir(path).entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);

            // dbgSeExpr <<"is dir and populating "<<path.toStdString();
            for (QList<QFileInfo>::ConstIterator it = infos.constBegin(); it != infos.constEnd(); ++it) {
                const QFileInfo *fi = &*it;
                if (fi->isDir() || fi->fileName().endsWith(QString::fromLatin1(".se"))) {
                    addChild(new ExprTreeItem(this, fi->fileName(), fi->filePath()));
                }
            }
        }
    }

    void addChild(ExprTreeItem *child)
    {
        child->row = children.size();
        children.push_back(child);
    }

    ExprTreeItem *getChild(const int row)
    {
        populate();
        if (row < 0 || row > (int)children.size()) {
            assert(false);
        }
        return children[row];
    }

    int getChildCount()
    {
        populate();
        return children.size();
    }

    void regen()
    {
        std::vector<QString> labels;
        std::vector<QString> paths;
        for (auto & i : children) {
            labels.push_back(i->label);
            paths.push_back(i->path);
            delete i;
        }
        children.clear();

        for (unsigned int i = 0; i < labels.size(); i++)
            addChild(new ExprTreeItem(this, labels[i], paths[i]));
    }

    int row;
    ExprTreeItem *parent;
    QString label;
    QString path;

private:
    std::vector<ExprTreeItem *> children;
    bool populated;
};

class ExprTreeModel : public QAbstractItemModel
{
    ExprTreeItem *root;

public:
    ExprTreeModel()
        : root(new ExprTreeItem(nullptr, QString(), QString()))
    {
    }

    ~ExprTreeModel() override
    {
        delete root;
    }

    void update()
    {
        beginResetModel();
        endResetModel();
    }

    void clear()
    {
        beginResetModel();
        root->clear();
        endResetModel();
    }

    void addPath(const char *label, const char *path)
    {
        root->addChild(new ExprTreeItem(root, QString::fromLatin1(label), QString::fromLatin1(path)));
    }

    QModelIndex parent(const QModelIndex &index) const override
    {
        if (!index.isValid())
            return {};
        auto *item = (ExprTreeItem *)(index.internalPointer());
        ExprTreeItem *parentItem = item->parent;
        if (parentItem == root)
            return {};
        else
            return createIndex(parentItem->row, 0, parentItem);
    }

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override
    {
        if (!hasIndex(row, column, parent))
            return {};
        else if (!parent.isValid())
            return createIndex(row, column, root->getChild(row));
        else {
            auto *item = (ExprTreeItem *)(parent.internalPointer());
            return createIndex(row, column, item->getChild(row));
        }
    }

    int columnCount(const QModelIndex &) const override
    {
        return 1;
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        if (!parent.isValid())
            return root->getChildCount();
        else {
            auto *item = (ExprTreeItem *)(parent.internalPointer());
            if (!item)
                return root->getChildCount();
            else
                return item->getChildCount();
        }
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {
        if (!index.isValid())
            return QVariant();
        if (role != Qt::DisplayRole)
            return QVariant();
        auto *item = (ExprTreeItem *)(index.internalPointer());
        if (!item)
            return QVariant();
        else
            return QVariant(item->label);
    }

    QModelIndex find(QString path)
    {
        ExprTreeItem *item = root->find(path);
        if (!item) {
            beginResetModel();
            root->regen();
            endResetModel();
            item = root->find(path);
        }
        if (item) {
            dbgSeExpr << "found it ";
            return createIndex(item->row, 0, item);
        }

        return {};
    }
};

class ExprTreeFilterModel : public QSortFilterProxyModel
{
public:
    ExprTreeFilterModel(QWidget *parent = nullptr)
        : QSortFilterProxyModel(parent)
    {
    }

    void update()
    {
        beginResetModel();
        endResetModel();
    }

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override
    {
        if (sourceParent.isValid() && sourceModel()->data(sourceParent).toString().contains(filterRegularExpression()))
            return true;
        QString data = sourceModel()->data(sourceModel()->index(sourceRow, 0, sourceParent)).toString();
        bool keep = data.contains(filterRegularExpression());

        QModelIndex subIndex = sourceModel()->index(sourceRow, 0, sourceParent);
        if (subIndex.isValid()) {
            for (int i = 0; i < sourceModel()->rowCount(subIndex); ++i)
                keep = keep || filterAcceptsRow(i, subIndex);
        }
        return keep;
    }
};

ExprBrowser::~ExprBrowser()
{
    delete treeModel;
}

ExprBrowser::ExprBrowser(QWidget *parent, ExprEditor *editor)
    : QWidget(parent)
    , editor(editor)
    , _context(QString())
    , _searchPath(QString())
    , _applyOnSelect(true)
{
    auto *rootLayout = new QVBoxLayout;
    rootLayout->setContentsMargins({});
    this->setLayout(rootLayout);
    // search and clear widgets
    auto *searchAndClearLayout = new QHBoxLayout();
    exprFilter = new QLineEdit();
    connect(exprFilter, SIGNAL(textChanged(const QString &)), SLOT(filterChanged(const QString &)));
    searchAndClearLayout->addWidget(exprFilter, 2);
    auto *clearFilterButton = new QPushButton(tr("X"));
    clearFilterButton->setFixedWidth(24);
    searchAndClearLayout->addWidget(clearFilterButton, 1);
    rootLayout->addLayout(searchAndClearLayout);
    connect(clearFilterButton, SIGNAL(clicked()), SLOT(clearFilter()));
    // model of tree
    treeModel = new ExprTreeModel();
    proxyModel = new ExprTreeFilterModel(this);
    proxyModel->setSourceModel(treeModel);
    // tree widget
    treeNew = new QTreeView;
    treeNew->setModel(proxyModel);
    treeNew->hideColumn(1);
    treeNew->setHeaderHidden(true);
    rootLayout->addWidget(treeNew);
    // selection mode and signal
    treeNew->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(treeNew->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), SLOT(handleSelection(const QModelIndex &, const QModelIndex &)));
}

void ExprBrowser::addPath(const std::string &name, const std::string &path)
{
    labels.append(QString::fromStdString(name));
    paths.append(QString::fromStdString(path));
    treeModel->addPath(name.c_str(), path.c_str());
}

void ExprBrowser::setSearchPath(const QString &context, const QString &path)
{
    _context = context;
    _searchPath = path;
}

std::string ExprBrowser::getSelectedPath()
{
    QModelIndex sel = treeNew->currentIndex();
    if (sel.isValid()) {
        QModelIndex realCurrent = proxyModel->mapToSource(sel);
        auto *item = (ExprTreeItem *)realCurrent.internalPointer();
        return item->path.toStdString();
    }
    return std::string("");
}

void ExprBrowser::selectPath(const char *path)
{
    QModelIndex index = treeModel->find(QString::fromLatin1(path));
    treeNew->setCurrentIndex(proxyModel->mapFromSource(index));
}

void ExprBrowser::update()
{
    treeModel->update();
    proxyModel->update();
}

void ExprBrowser::handleSelection(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)
    if (current.isValid()) {
        QModelIndex realCurrent = proxyModel->mapToSource(current);
        auto *item = (ExprTreeItem *)realCurrent.internalPointer();
        QString path = item->path;
        if (path.endsWith(QString::fromLatin1(".se"))) {
            QFile file(path);
            if (file.open(QIODevice::ReadOnly)) {
                QTextStream fileContents(&file);
                editor->setExpr(fileContents.readAll(), _applyOnSelect);
            }
        }
    }
}

void ExprBrowser::clear()
{
    labels.clear();
    paths.clear();
    clearSelection();

    treeModel->clear();
}

void ExprBrowser::clearSelection()
{
    treeNew->clearSelection();
}

void ExprBrowser::clearFilter()
{
    exprFilter->clear();
}

void ExprBrowser::filterChanged(const QString &str)
{
    proxyModel->setFilterRegularExpression(QRegularExpression(str));
    proxyModel->setFilterKeyColumn(0);
    if (!str.isEmpty()) {
        treeNew->expandAll();
    } else {
        treeNew->collapseAll();
    }
}

void ExprBrowser::saveExpressionAs()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Save Expression"), QString::fromStdString(_userExprDir), tr("*.se"));

    if (path.length() > 0) {
        std::ofstream file(path.toStdString().c_str());
        if (!file) {
            QString msg = tr("Could not open file %1 for writing").arg(path);
            QMessageBox::warning(this, tr("Error"), QString::fromLatin1("<font face=fixed>%1</font>").arg(msg));
            return;
        }
        file << editor->getExpr().toStdString();
        file.close();

        update();
        selectPath(path.toStdString().c_str());
    }
}

void ExprBrowser::saveLocalExpressionAs()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Save Expression"), QString::fromStdString(_localExprDir), tr("*.se"));

    if (path.length() > 0) {
        std::ofstream file(path.toStdString().c_str());
        if (!file) {
            QString msg = tr("Could not open file %1 for writing").arg(path);
            QMessageBox::warning(this, tr("Error"), QString::fromLatin1("<font face=fixed>%1</font>").arg(msg));
            return;
        }
        file << editor->getExpr().toStdString();
        file.close();

        update();
        selectPath(path.toStdString().c_str());
    }
}

void ExprBrowser::saveExpression()
{
    std::string path = getSelectedPath();
    if (path.length() == 0) {
        saveExpressionAs();
        return;
    }
    std::ofstream file(path.c_str());
    if (!file) {
        QString msg = tr("Could not open file %1 for writing.  Is it read-only?").arg(QString::fromStdString(path));
        QMessageBox::warning(this, tr("Error"), tr("<font face=fixed>%1</font>").arg(msg));
        return;
    }
    file << editor->getExpr().toStdString();
    file.close();
}

void ExprBrowser::expandAll()
{
    treeNew->expandAll();
}

void ExprBrowser::expandToDepth(int depth)
{
    treeNew->expandToDepth(depth);
}

// Location for storing user's expression files
void ExprBrowser::addUserExpressionPath(const std::string &context)
{
    char *homepath = getenv("HOME");
    if (homepath) {
        std::string path = std::string(homepath) + "/" + context + "/expressions/";
        if (QDir(QString::fromStdString(path)).exists()) {
            _userExprDir = path;
            addPath("My Expressions", path);
        }
    }
}

/*
 * NOTE: The hard-coded paint3d assumptions can be removed once
 * it (and bonsai?) are adjusted to call setSearchPath(context, path)
 */

bool ExprBrowser::getExpressionDirs()
{
    const char *env = nullptr;
    bool enableLocal = false;
    /*bool homeFound = false; -- for xgen's config.txt UserRepo section below */

    if (_searchPath.length() > 0)
        env = _searchPath.toStdString().c_str();
    else
        env = getenv(P3D_CONFIG_ENVVAR); /* For backwards compatibility */

    if (!env)
        return enableLocal;

    std::string context;
    if (_context.length() > 0) {
        context = _context.toStdString();
    } else {
        context = "paint3d"; /* For backwards compatibility */
    }

    clear();

    std::string configFile = std::string(env) + "/config.txt";
    std::ifstream file(configFile.c_str());
    if (file) {
        std::string key;
        while (file) {
            file >> key;

            if (key[0] == '#') {
                std::array<char, 1024> buffer{};
                file.getline(buffer.data(), 1024);
            } else {
                if (key == "ExpressionDir") {
                    std::string label;
                    std::string path;
                    file >> label;
                    file >> path;
                    if (QDir(QString::fromStdString(path)).exists())
                        addPath(label, path);
                } else if (key == "ExpressionSubDir") {
                    std::string path;
                    file >> path;
                    _localExprDir = path;
                    if (QDir(QString::fromStdString(path)).exists()) {
                        addPath("Local", _localExprDir);
                        enableLocal = true;
                    }
                    /* These are for compatibility with xgen.
                     * Long-term, xgen should use the same format.
                     * Longer-term, we should use JSON or something */
                } else if (key == "GlobalRepo") {
                    std::string path;
                    file >> path;
                    path += "/expressions/";
                    if (QDir(QString::fromStdString(path)).exists())
                        addPath("Global", path);
                } else if (key == "LocalRepo") {
                    std::string path;
                    file >> path;
                    path += "/expressions/";
                    _localExprDir = path;
                    if (QDir(QString::fromStdString(path)).exists()) {
                        addPath("Local", _localExprDir);
                        enableLocal = true;
                    }

                    /*
                     * xgen's config.txt has a "UserRepo" section but we
                     * intentionally ignore it since we already add the user dir
                     * down where the HOME stuff is handled
                     */

                    /*
                    } else if (key == "UserRepo") {
                        std::string path;
                        file>>path;
                        path += "/expressions/";

                        size_t found = path.find("${HOME}");

                        if (found != std::string::npos) {
                            char *homepath = getenv("HOME");
                            if (homepath) {
                                path.replace(found, strlen("${HOME}"), homepath);
                            } else {
                                continue;
                            }
                        }
                        if(QDir(QString(path.c_str())).exists()){
                            addPath("User", path);
                            homeFound = true;
                        }
                    */
                } else {
                    std::array<char, 1024> buffer {};
                    file.getline(buffer.data(), 1024);
                }
            }
        }
    }
    addUserExpressionPath(context);
    update();
    return enableLocal;
}
