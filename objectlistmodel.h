#ifndef OBJECTLISTMODEL_H
#define OBJECTLISTMODEL_H

#include <QAbstractListModel>

class ObjectListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ObjectListModel(QObject *parent = 0);
};

#endif // OBJECTLISTMODEL_H
