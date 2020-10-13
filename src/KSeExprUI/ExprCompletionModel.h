// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: LicenseRef-Apache-2.0
/*
* @file ExprCompletionModel.h
* @brief Provides a model for providing completion items
* @author  aselle
*/

#ifndef ExprCompletionModel_h
#define ExprCompletionModel_h

#include <QtCore/QAbstractItemModel>
#include <QtCore/QString>
#include <QtCore/QSize>
#include <vector>

class ExprCompletionModel : public QAbstractItemModel  // ItemModel
{
  public:
    // clear/add functions (these are ones that will be resolved with resolveFunc()
    void clearFunctions();
    void addFunction(const QString& function, const QString& docString);

    // clear/add user variables (these are ones that will be resolved with resolveVar()
    void clearVariables();
    void addVariable(const QString& str, const QString& comment);

    // add extras
    void syncExtras(const ExprCompletionModel& otherModel);

    ExprCompletionModel(QObject* parent = 0);

    QModelIndex index(int row, int column, const QModelIndex&) const { return createIndex(row, column, nullptr); }

    QModelIndex parent(const QModelIndex&) const { return QModelIndex(); }

    int rowCount(const QModelIndex& parent = QModelIndex()) const {
        Q_UNUSED(parent);
        int count = builtins.size() + functions.size() + variables.size() + local_variables.size();
        return count;
    }

    int columnCount(const QModelIndex& parent) const {
        Q_UNUSED(parent);
        return 2;
    }

    QString getFirstLine(const std::string& all) const {
        size_t newline = all.find("\n");
        if (newline != std::string::npos)
            return QString::fromStdString(all.substr(0, newline));
        else
            return QString::fromStdString(all);
    }

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const {
        Q_UNUSED(orientation);
        if (role == Qt::DisplayRole)
            return QVariant();
        else if (role == Qt::SizeHintRole) {
            if (section == 0)
                return QVariant(QSize(100, 1));
            else
                return QVariant(QSize(200, 1));
        } else
            return QVariant();
    }
    std::vector<QString> local_variables;  // only the expression editor itself should modify these

    QString getDocString(const QString& s);

  private:
    Q_OBJECT;

    std::vector<QString> builtins;
    std::vector<QString> functions, functions_comment;
    std::map<QString, int> functionNameToFunction;
    std::vector<QString> variables, variables_comment;
};

#endif
