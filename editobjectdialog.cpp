#include "editobjectdialog.h"
#include "ui_editobjectdialog.h"

EditObjectDialog::EditObjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditObjectDialog)
{
    ui->setupUi(this);
    _objectIndex = -1;
}

EditObjectDialog::~EditObjectDialog()
{
    delete ui;
}

int EditObjectDialog::objectIndex()
{return _objectIndex;}

void EditObjectDialog::updateObjectList(const QVector<ul::ObjectInfo> list)
{
    _objectList = list;
}

void EditObjectDialog::activeObject(int index)
{
    _objectIndex = index;
    if(index > -1)
    {
        QString name(_objectList[_objectIndex].name().c_str());
        QString url(_objectList[_objectIndex].URL().c_str());
        ui->nameEdit->setText(name);
        ui->urlEdit->setText(url);
    }
}

void EditObjectDialog::onOkClick()
{
    QString name = ui->nameEdit->text();
    QString url = ui->urlEdit->text();

    if(name.compare("")!=0)
    {
        ul::ObjectInfo t = _objectList[_objectIndex];

        t.name(name.toStdString());
        t.URL(url.toStdString());

        emit updatedObjectInfo(_objectIndex,t);
    }
    close();
}

void EditObjectDialog::onCancelClick()
{
    close();
}
