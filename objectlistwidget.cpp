#include "objectlistwidget.h"

ObjectListWidget::ObjectListWidget(QWidget *parent) :
    QListWidget(parent)
{
}

void ObjectListWidget::updateObjectList(const QVector<ul::ObjectInfo> objects)
{
    this->clear();
    for(int i = 0; i < objects.size(); ++i)
    {
        QListWidgetItem* item = new QListWidgetItem(this);
        addItem(item);

        ObjectInfoListWidget *t = new ObjectInfoListWidget(objects[i].icon(),
                                                           objects[i].name(),
                                                           objects[i].URL(),
                                                           this);
        item->setSizeHint(t->minimumSizeHint());
        setItemWidget(item, t);
    }
}
