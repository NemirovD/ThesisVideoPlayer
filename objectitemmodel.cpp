#include "objectitemmodel.h"

ObjectItemModel::ObjectItemModel(QObject *parent) :
    QAbstractItemModel(parent)
{
}

int ObjectItemModel::rowCount(const QModelIndex &parent) const
{
    return 2;
}

int ObjectItemModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant ObjectItemModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole)
    {
        return QString("Row%1, Column%2")
                            .arg(index.row() + 1)
                            .arg(index.column() +1);
    }
    return QVariant();
}
