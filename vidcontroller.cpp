#include "vidcontroller.h"

VidController::VidController(QObject *parent) :
    QThread(parent)
{
    this->stop = true;
    this->quit = false;
    this->capture = new cv::VideoCapture;
}

VidController::~VidController()
{
    this->mutex.lock();
    this->stop = true;
    this->capture->release();
    this->mutex.unlock();
    this->wait();
}

void VidController::exit()
{
    this->quit = true;
}

void VidController::stopVid()
{
    stop = true;
}

void VidController::play()
{
    if(isStopped())
    {
        stop = false;
    }
}

bool VidController::loadVideo(std::string filename)
{
    exit();
    wait();
    quit = false;
    capture = new cv::VideoCapture;
    capture->open(filename);

    std::string fWithoutExt = ul::getFileNameWithoutExtension(filename);

    annotationLoader = ul::AnnotationLoader(fWithoutExt + ".xml");

    if(capture->isOpened())
    {
        this->framerate = (int) capture->get(CV_CAP_PROP_FPS);
        start(NormalPriority);
        return true;
    }
    else
    {
        return false;
    }
}

double VidController::getCurrentFrame() const
{
    return capture->get(CV_CAP_PROP_POS_FRAMES);
}

double VidController::getNumberOfFrames() const
{
    return capture->get(CV_CAP_PROP_FRAME_COUNT);
}
double VidController::getFrameRate() const
{
    return framerate;
}
void VidController::setCurrentFrame( int frameNumber )
{
    capture->set(CV_CAP_PROP_POS_FRAMES, frameNumber);
}

bool VidController::isStopped() const
{
    return this->stop;
}

bool VidController::isOpened() const
{
    return capture->isOpened();
}

void VidController::run()
{
    int delay = 1000/framerate;
    while(!quit)
    {
        double start = ((double)clock())/CLOCKS_PER_SEC * 1000;

        if(!stop)
        {
            //Get the frame
            if(!capture->read(this->frame))
            {
                stop = true;
            }
            else
            {
                if(draw_annotations)
                    addAnnotationsToFrame();
                img = processImage(frame);
            }
        }
        if(!frame.empty())
        {
            emit processedImage(*img);

            if(stop)
                this->msleep(delay);
        }
        double currTime = ((double)clock())/CLOCKS_PER_SEC * 1000;
        long waitTime = (long)(start + 1000/framerate - currTime);

        if(waitTime > 0 && !stop)
            this->msleep(waitTime);
    }
}

cv::Point VidController::labelSizeToImageSize(const QPoint& pt,const QSize& sz)
{
    float xf = (pt.x()/(float)sz.width())*frame.size().width;
    float yf = (pt.y()/(float)sz.width())*frame.size().height;
    return cv::Point((int)xf,(int)yf);
}

QImage* VidController::processImage(cv::Mat frame)
{
    QImage * img;
    //3channel transfer
    if(frame.channels() == 3)
    {
        cv::cvtColor(frame,RGBframe, CV_BGR2RGB);
        img = new QImage((const unsigned char*)(RGBframe.data),
                         RGBframe.cols,
                         RGBframe.rows,
                         QImage::Format_RGB888);
    }
    //single channel transfer
    else
    {
        img = new QImage((const unsigned char*)(frame.data),
                         frame.cols,
                         frame.rows,
                         QImage::Format_Indexed8);
    }
    return img;
}

void VidController::addAnnotationsToFrame()
{
    std::vector<ul::Annotation> rectsToDraw(annotationLoader.getFrameAnnotations(
                                          getCurrentFrame()));

    for(unsigned int i = 0; i < rectsToDraw.size(); ++i)
    {
        cv::rectangle(frame,rectsToDraw[i].getRect(),cv::Scalar::all(255),4);
    }
}
