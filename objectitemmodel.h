#ifndef OBJECTITEMMODEL_H
#define OBJECTITEMMODEL_H

#include <QAbstractItemModel>

class ObjectItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit ObjectItemModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
};

#endif // OBJECTITEMMODEL_H
