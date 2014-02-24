#include "objectinfolistwidget.h"
#include "ui_objectinfolistwidget.h"

ObjectInfoListWidget::ObjectInfoListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ObjectInfoListWidget)
{
    ui->setupUi(this);
}

ObjectInfoListWidget::ObjectInfoListWidget(cv::Mat icon,
                                           std::string name,
                                           std::string url,
                                           QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ObjectInfoListWidget)
{
    ui->setupUi(this);
    ui->nameLabel->setText(name.c_str());

    std::string htmlAnchor = "";
    htmlAnchor += "<a href=\"" + url+ "\">";
    htmlAnchor += url;
    htmlAnchor += "</a>";

    ui->urlLabel->setText(htmlAnchor.c_str());
    ui->urlLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->urlLabel->setOpenExternalLinks(true);

    QImage img = processImage(icon);

    ui->iconLabel->setAlignment(Qt::AlignCenter);
    ui->iconLabel->setPixmap(QPixmap::fromImage(img).scaled(ui->iconLabel->size(),
                                                             Qt::KeepAspectRatio,
                                                             Qt::FastTransformation));
}

ObjectInfoListWidget::~ObjectInfoListWidget()
{
    delete ui;
}

QImage ObjectInfoListWidget::processImage(cv::Mat frame)
{
    QImage * img;
    //3channel transfer
    if(frame.channels() == 3)
    {
        cv::cvtColor(frame,RGBframe, CV_BGR2RGB);
        img = new QImage((const unsigned char*)(RGBframe.data),
                         RGBframe.cols,
                         RGBframe.rows,
                         RGBframe.step,
                         QImage::Format_RGB888);
    }
    //single channel transfer
    else
    {
        img = new QImage((const unsigned char*)(frame.data),
                         frame.cols,
                         frame.rows,
                         RGBframe.step,
                         QImage::Format_Indexed8);
    }
    return *img;
}
