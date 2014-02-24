#include "addobjectdialog.h"
#include "ui_addobjectdialog.h"

AddObjectDialog::AddObjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddObjectDialog)
{
    ui->setupUi(this);
}
AddObjectDialog::~AddObjectDialog()
{
    delete ui;
}

void AddObjectDialog::onOkClick()
{
    QString name = ui->nameEdit->text();
    QString url = ui->urlEdit->text();

    if(name.compare("")!=0)
    {
        emit rectInfo(name.toStdString(),
                      url.toStdString());
    }
    close();
}

void AddObjectDialog::onCancelClick()
{
    close();
}
