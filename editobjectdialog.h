#ifndef EDITOBJECTDIALOG_H
#define EDITOBJECTDIALOG_H

#include <opencv2/opencv.hpp>
#include <QPushButton>
#include <QFormLayout>
#include <QLineEdit>
#include <QDialog>
#include <QWidget>
#include <QString>
#include "utils.h"

namespace Ui {
class EditObjectDialog;
}

class EditObjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditObjectDialog(QWidget *parent = 0);
    ~EditObjectDialog();
    //getter
    int objectIndex();
signals:
    void updatedObjectInfo(int,ul::ObjectInfo);
public slots:
    void updateObjectList(const QVector<ul::ObjectInfo>);
    void activeObject(int);
    void onOkClick();
    void onCancelClick();
private:
    Ui::EditObjectDialog *ui;
    int _objectIndex;
    QVector<ul::ObjectInfo> _objectList;
};

#endif // EDITOBJECTDIALOG_H
