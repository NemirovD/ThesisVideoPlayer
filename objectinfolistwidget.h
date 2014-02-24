#ifndef OBJECTINFOLISTWIDGET_H
#define OBJECTINFOLISTWIDGET_H

#include <QLabel>
#include <QImage>
#include <QWidget>
#include <opencv2/opencv.hpp>

namespace Ui {
class ObjectInfoListWidget;
}

class ObjectInfoListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ObjectInfoListWidget(QWidget *parent = 0);
    ObjectInfoListWidget(cv::Mat, std::string, std::string, QWidget *parent = 0);
    ~ObjectInfoListWidget();

private:
    QImage processImage(cv::Mat frame);

    cv::Mat RGBframe;
    Ui::ObjectInfoListWidget *ui;
};

#endif // OBJECTINFOLISTWIDGET_H
