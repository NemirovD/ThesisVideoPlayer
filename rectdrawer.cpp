#include "rectdrawer.h"

RectDrawer::RectDrawer(QObject *parent) :
    QObject(parent)
{
}

RectDrawer::RectDrawer(const QPoint & pt, const QSize & sz, cv::Mat frame, QObject *parent) :
    QObject(parent)
{
    _frame = frame;


    cv::Point cpt(labelSizeToImageSize(pt,sz));
    _rect = cv::Rect(cpt, cv::Size(0,0));
}

cv::Mat RectDrawer::updateRect(const QPoint &pt, const QSize &sz)
{
    _tFrame = _frame.clone();
    cv::Point cpt(labelSizeToImageSize(pt,sz));

    _rect.width = cpt.x - _rect.x;
    _rect.height = cpt.y - _rect.y;

    cv::rectangle(_tFrame, _rect, cv::Scalar::all(255),3);
    return _tFrame;
}

void RectDrawer::onMouseUp(const QPoint&, const QSize&)
{
}

cv::Point RectDrawer::labelSizeToImageSize(const QPoint& pt,const QSize& sz)
{
    float xf = (pt.x()/(float)sz.width())*_frame.size().width;
    float yf = (pt.y()/(float)sz.height())*_frame.size().height;

    return cv::Point((int)xf,(int)yf);
}

AddObjectRectDrawer::AddObjectRectDrawer(const QPoint & pt,
                                         const QSize & sz,
                                         cv::Mat frame,
                                         int frameNumber) :
    RectDrawer(pt,sz,frame)
{
    _frameNumber = frameNumber;
}

void AddObjectRectDrawer::onMouseUp(const QPoint &pt, const QSize &sz)
{
    _tFrame = _frame.clone();
    cv::Point cpt(labelSizeToImageSize(pt,sz));

    _rect.width = cpt.x - _rect.x;
    _rect.height = cpt.y - _rect.y;

    _icon = cv::Mat(_frame,_rect);

    cv::rectangle(_tFrame, _rect, cv::Scalar::all(255),3);
    if(_rect.area() > 0)
    {
        _addObjectDialog = new AddObjectDialog();
        connect(_addObjectDialog,
                SIGNAL(rectInfo(std::string,std::string)),
                this,
                SLOT(createObjectInfo(std::string,std::string)));
        _addObjectDialog->exec();
    }
}

void AddObjectRectDrawer::createObjectInfo(std::string name, std::string url)
{
    ul::ObjectInfo t(_icon, name, url, _frameNumber, _rect);
    emit objectInfoCreated(t);
}

MoveRectDrawer::MoveRectDrawer(const QPoint &pt,
                               const QSize &sz,
                               cv::Mat frame,
                               QVector<ul::ObjectInfo> list) :
    RectDrawer(pt,sz,frame)
{
    cv::Point cpt(labelSizeToImageSize(pt,sz));

    _objectList = list;

    QVector<int> iList;
    for(int i = 0; i < list.size(); ++i)
    {
        if(list[i].location().contains(cpt))
        {
            iList.push_back(i);
        }
    }

    if(iList.size() == 1)
    {
        _rect = list[iList[0]].location();
        _initialPoint = cpt;
        _oldRect = list[iList[0]].location();
        _objectIndex = iList[0];
    }
    else if(iList.size() == 0)
    {
        _rectSelected = false;
    }
    else
    {
        //set distance to some impossibly
        //larger number
       double distance = 1000000;
       int index = -1;
       for(int i = 0; i < iList.size(); ++i)
       {
           std::cout << list[iList[i]].location().x << std::endl;
           double calcDistance = dist(cpt,getCenterOf(list[iList[i]].location()));
           if(calcDistance < distance)
           {
               distance = calcDistance;
               index = iList[i];
           }
       }
       _rect = list[iList[index]].location();
       _initialPoint = cpt;
       _oldRect = list[iList[index]].location();
       _objectIndex = iList[index];
       _rectSelected = true;
    }
}

cv::Mat MoveRectDrawer::updateRect(const QPoint &pt, const QSize &sz)
{
    if(_rectSelected)
    {
        _tFrame = _frame.clone();
        cv::Point cpt(labelSizeToImageSize(pt,sz));
        cv::Point difference = cpt - _initialPoint;
        cv::Size rsize = _oldRect.size();
        _rect = cv::Rect(_initialPoint + difference, rsize);

        cv::rectangle(_tFrame, _rect, cv::Scalar::all(255),3);
        cv::rectangle(_tFrame, _oldRect, cv::Scalar(0,0,255),3);
        return _tFrame;
    }
    else
    {
        return _frame;
    }
}

void MoveRectDrawer::onMouseUp(const QPoint &pt, const QSize &sz)
{
    if(_rectSelected)
    {
        cv::Point cpt(labelSizeToImageSize(pt,sz));
        cv::Point difference = cpt - _initialPoint;
        cv::Size rsize = _oldRect.size();
        _rect = cv::Rect(_initialPoint + difference, rsize);

        ul::ObjectInfo t = _objectList[_objectIndex];
        t.location(_rect);

        emit updatedObject(_objectIndex,t);
    }
}

cv::Point MoveRectDrawer::getCenterOf(cv::Rect rect)
{
    cv::Point mid(rect.width/2, rect.height/2);
    return mid+rect.tl();
}

double MoveRectDrawer::dist(cv::Point p1, cv::Point p2)
{
    double x = std::abs(p1.x - p2.x);
    double x2 = x*x;

    double y = std::abs(p1.y - p2.y);
    double y2 = y*y;

    return sqrt(x2+y2);
}
