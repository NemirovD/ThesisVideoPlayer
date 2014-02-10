#ifndef VIDCONTROLLER_H
#define VIDCONTROLLER_H

#include <ctime>
#include <QMutex>
#include <QImage>
#include <QThread>
#include <opencv2/opencv.hpp>
#include "utils.h"

class VidController : public QThread
{
    Q_OBJECT
public:
    explicit VidController(QObject *parent = 0);
    ~VidController();

    //video control
    bool loadVideo(std::string filename);
    void play();
    void stopVid();
    void setCurrentFrame();

    //Video getters
    double getFrameRate() const;
    double getCurrentFrame() const;
    double getNumberOfFrames() const;

    //Video setters
    void VidController::setCurrentFrame( int frameNumber );

    //Video State Checkers
    bool isStopped() const;
    bool isOpened() const;

signals:
    void processedImage(const QImage& image);
public slots:
    void exit();

protected:
    void run();

private:
    //helper functions
    cv::Point labelSizeToImageSize(const QPoint&,const QSize&);
    QImage* processImage(cv::Mat frame);
    void addAnnotationsToFrame();
    //void addAnnotationToFrame(int frameNo, std::string name);

    bool quit;
    bool stop;
    bool draw_annotations;
    QMutex mutex;

    int framerate;
    QImage* img;
    cv::Mat frame;
    cv::Mat RGBframe;
    cv::VideoCapture* capture;
    ul::AnnotationLoader annotationLoader;
};

#endif // VIDCONTROLLER_H
