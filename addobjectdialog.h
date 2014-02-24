#ifndef ADDOBJECTDIALOG_H
#define ADDOBJECTDIALOG_H

#include <opencv2/opencv.hpp>
#include <QPushButton>
#include <QFormLayout>
#include <QLineEdit>
#include <QDialog>
#include <QWidget>
#include <QString>

namespace Ui {
class AddObjectDialog;
}

class AddObjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddObjectDialog(QWidget *parent = 0);
    ~AddObjectDialog();
signals:
    void rectInfo(const std::string, const std::string);
public slots:
    void onOkClick();
    void onCancelClick();
private:
    Ui::AddObjectDialog *ui;
};

#endif // ADDOBJECTDIALOG_H
