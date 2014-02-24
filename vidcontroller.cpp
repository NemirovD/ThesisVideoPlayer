#include "vidcontroller.h"
#include "vidcontroller.h"

VidController::VidController(QObject *parent) :
    QThread(parent)
{
    stop = true;
    quit = false;
    capture = new cv::VideoCapture;
    draw_objects = false;
    mouseCallbackMode = MODE_NONE;
}

VidController::~VidController()
{
    if(capture->isOpened())
        objectHandler.writeObjectsToFile();
    mutex.lock();
    quit = true;
    capture->release();
    mutex.unlock();
    wait();
}
void VidController::play()
{if(isStopped()){stop = false;}}

void VidController::stopVid()
{stop = true;}

void VidController::exit()
{quit = true;}

bool VidController::loadVideo(std::string filename)
{
    exit();
    wait();
    quit = false;
    capture = new cv::VideoCapture;
    capture->open(filename);

    std::string fWithoutExt = ul::getFileNameWithoutExtension(filename);

    objectHandler = ul::ObjectInfoHandler(fWithoutExt+".xml");
    emit objectListChanged(objectHandler.objectList());

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
{return capture->get(CV_CAP_PROP_POS_FRAMES);}

double VidController::getNumberOfFrames() const
{return capture->get(CV_CAP_PROP_FRAME_COUNT);}

double VidController::getFrameRate() const
{return framerate;}

void VidController::setCurrentFrame( int frameNumber )
{capture->set(CV_CAP_PROP_POS_FRAMES, frameNumber);}

void VidController::setMouseCallbackMode(int MODE)
{mouseCallbackMode = MODE;}

bool VidController::isStopped() const
{return stop;}

bool VidController::isOpened() const
{return capture->isOpened();}

void VidController::mouseDown(const QPoint &pt, const QSize &sz)
{
    if(stop){
        switch(mouseCallbackMode)
        {
        case MODE_ADD_OBJECT:
        {
            AddObjectRectDrawer *t = new AddObjectRectDrawer(pt,sz,frame,getCurrentFrame());
            connect(t,
                    SIGNAL(objectInfoCreated(ul::ObjectInfo)),
                    this,
                    SLOT(addObject(ul::ObjectInfo)));
            _rectDrawer = t;
            break;
        }
            break;
        case MODE_MOVE_RECT:
        {
            MoveRectDrawer *t = new MoveRectDrawer(pt,sz,frame,objectHandler.objectList());
            connect(t,
                    SIGNAL(updatedObject(int,ul::ObjectInfo)),
                    this,
                    SLOT(editObject(int,ul::ObjectInfo)));
            _rectDrawer = t;
            break;
        }
            break;
        default:
        case MODE_NONE:
            break;
        }
    }
}

void VidController::mouseMove(const QPoint &pt, const QSize &sz)
{
    switch(mouseCallbackMode)
    {
    case MODE_ADD_OBJECT:
    case MODE_MOVE_RECT:
    {
        cv::Mat t = _rectDrawer->updateRect(pt,sz);
        QImage * qimg = processImage(t);
        emit processedImage(*qimg);
        break;
    }
        break;
    default:
    case MODE_NONE:
        break;
    }
}

void VidController::mouseUp(const QPoint &pt, const QSize &sz)
{
    switch(mouseCallbackMode)
    {
    case MODE_ADD_OBJECT:
        _rectDrawer->onMouseUp(pt,sz);
        break;
    case MODE_MOVE_RECT:
        _rectDrawer->onMouseUp(pt,sz);
        break;
    default:
    case MODE_NONE:
        break;
    }
}

QVector<ul::ObjectInfo> VidController::getObjectList() const
{
    return objectHandler.objectList();
}

void VidController::addObject(ul::ObjectInfo oi)
{
    objectHandler.addObject(oi);
    emit objectListChanged(objectHandler.objectList());
}

void VidController::editObject(int index, ul::ObjectInfo oi)
{
    objectHandler.editObject(index, oi);
    emit objectListChanged(objectHandler.objectList());
}

void VidController::showObject(int index)
{
    ul::ObjectInfo object(objectHandler.objectList()[index]);
    setCurrentFrame(object.frameNumber());
    if(capture->read(frame))
    {
        cv::Mat tFrame(frame.clone());
        cv::Rect r = object.location();
        cv::rectangle(tFrame,
                      r,
                      cv::Scalar::all(255),
                      4);

        cv::putText(tFrame,
                    object.name(),
                    cv::Point(r.x,r.y-10),
                    cv::FONT_HERSHEY_DUPLEX,
                    2,
                    cv::Scalar::all(255),
                    4);
        img = processImage(tFrame);
        emit processedImage(*img);
    }
    else
    {
        //we've got a problem
    }
}

void VidController::showObjects(bool show)
{draw_objects = show;}

void VidController::run()
{
    //so that the first frame gets shown on load
    capture->read(frame);
    img = processImage(frame);
    emit processedImage(*img);

    //main part of the function
    int delay = 1000/framerate;
    while(!quit)
    {
        double start = ((double)clock())/CLOCKS_PER_SEC * 1000;

        if(!stop)
        {
            //Get the frame
            if(!capture->read(frame))
            {
                stop = true;
                emit videoEnded();
            }
            else
            {
                if(draw_objects)
                   addObjectsToFrame();
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
    //Don't change RGBframe to a local variable
    //there will be plenty of issues
    //one is a performance issue
    //the other is a memory issue
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

    return img;
}

void VidController::addObjectsToFrame()
{

    objectsInFrame = objectHandler.getObjectsIn(getCurrentFrame());

    for(int i = 0; i < objectsInFrame.size(); ++i)
    {
        cv::Rect r = objectsInFrame[i].location();
        cv::rectangle(frame,
                      r,
                      cv::Scalar::all(255),
                      4);

        cv::putText(frame,
                    objectsInFrame[i].name(),
                    cv::Point(r.x,r.y-10),
                    cv::FONT_HERSHEY_DUPLEX,
                    2,
                    cv::Scalar::all(255),
                    4);
    }
}
