#ifndef OBJECTLISTWIDGET_H
#define OBJECTLISTWIDGET_H

#include <QPushButton>
#include <QListWidget>
#include "utils.h"
#include "objectinfolistwidget.h"

class ObjectListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit ObjectListWidget(QWidget *parent = 0);

signals:

public slots:
    void updateObjectList(const QVector<ul::ObjectInfo>);
};

#endif // OBJECTLISTWIDGET_H
