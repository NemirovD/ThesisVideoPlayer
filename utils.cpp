#include "utils.h"
namespace ul
{
FormatTime::FormatTime()
{
    _currentTime = 0;
    _totalTime = 0;
}

FormatTime::FormatTime(int cur, int total)
{
    _currentTime = cur;
    _totalTime = total;
}

std::string FormatTime::getFormattedTime()
{
    return convertTime(_currentTime)+" / "+convertTime(_totalTime);
}

std::string FormatTime::getFormattedTime(int currentSeconds)
{
    currentTime(currentSeconds);
    return getFormattedTime();
}

void FormatTime::totalTime(int totalSeconds)
{
    _totalTime = totalSeconds;
}

void FormatTime::currentTime(int currentSeconds)
{
    _currentTime = currentSeconds;
}

std::string FormatTime::convertTime(int timeInSeconds)
{
    int seconds = (int) (timeInSeconds) % 60 ;
    int minutes = (int) ((timeInSeconds / 60) % 60);
    int hours   = (int) ((timeInSeconds / (60*60)) % 24);
    QTime t(hours, minutes, seconds);
    if (hours == 0 )
      return t.toString("mm:ss").toStdString();
    else
      return t.toString("h:mm:ss").toStdString();
}

std::string getFileNameWithoutExtension(std::string filename)
{
    std::string toBeReturned = "";
    std::vector<std::string> fileparts;

    fileparts = ul::split(filename,'.');


    toBeReturned += fileparts[0];
    for(unsigned int i = 1; i < fileparts.size()-1;++i)
    {
        toBeReturned += '.';
        toBeReturned += fileparts[i];
    }

    return toBeReturned;
}

ObjectInfo::ObjectInfo()
{
    _icon = cv::Mat();
    _name = "";
    _URL = "";
    //the frame number can never
    //naturally be less than zero
    _frameNumber = -1;
    _location = cv::Rect();
}

ObjectInfo::ObjectInfo(cv::Mat icon,
                       std::string name,
                       std::string URL,
                       int frameNumber,
                       cv::Rect location)
{
    _icon = icon;
    _name = name;
    _URL = URL;
    _frameNumber = frameNumber;
    _location = location;
}

ObjectInfo::~ObjectInfo()
{}

cv::Mat ObjectInfo::icon() const
{return _icon;}

std::string ObjectInfo::name() const
{return _name;}

std::string ObjectInfo::URL() const
{return _URL;}

int ObjectInfo::frameNumber() const
{return _frameNumber;}

cv::Rect ObjectInfo::location() const
{return _location;}

void ObjectInfo::icon(cv::Mat i)
{_icon = i;}

void ObjectInfo::name(std::string n)
{_name = n;}

void ObjectInfo::URL(std::string u)
{_URL = u;}

void ObjectInfo::frameNumber(int f)
{_frameNumber = f;}

void ObjectInfo::location(cv::Rect l)
{_location = l;}

cv::FileStorage& operator << (cv::FileStorage& fs, const ObjectInfo oi)
{
    fs << "name" << oi._name;
    fs << "icon" << oi._icon;
    fs << "URL" << oi._URL;
    fs << "frameNumber" << oi._frameNumber;
    fs << "location" << oi._location;

    return fs;
}

cv::FileNode& operator >> (cv::FileNode& fn, ObjectInfo& oi)
{
    cv::Mat ico;
    fn["icon"] >> ico;
    oi.icon(ico);
    oi.name((std::string)fn["name"]);

    oi.URL((std::string)fn["URL"]);

    oi.frameNumber((int)fn["frameNumber"]);

    std::vector<int> r;
    fn["location"] >> r;
    cv::Rect rl(r[0],r[1],r[2],r[3]);
    oi.location(rl);

    return fn;
}

ObjectInfoHandler::ObjectInfoHandler()
{
    _filename = "";
    _open = false;
}

ObjectInfoHandler::ObjectInfoHandler(std::string filename)
{
    loadObjectsFromFile(filename);
}

QVector<ObjectInfo> ObjectInfoHandler::objectList() const
{return _objectList;}

bool ObjectInfoHandler::isOpened() const
{return _open;}

void ObjectInfoHandler::loadObjectsFromFile(std::string filename)
{
    _filename = filename;
    loadObjectInfo();
}

void ObjectInfoHandler::addObject(ObjectInfo oi)
{
    _objectList.push_back(oi);
}

void ObjectInfoHandler::editObject(int index, ObjectInfo oi)
{
    _objectList[index] = oi;
}

void ObjectInfoHandler::writeObjectsToFile()
{writeObjectsToFile(_filename);}

void ObjectInfoHandler::writeObjectsToFile(std::string filename)
{
    cv::FileStorage fs(filename,cv::FileStorage::WRITE);
    fs << "Objects" << "{";
    for(int i = 0; i < _objectList.size(); ++i)
    {
        //setup arbitrary ID
        std::stringstream ss;
        ss << "object" << i;

        //write ID to file storage
        std::string ID = ss.str();
        fs << ID << "{";

        //write objectinfo
        fs << _objectList[i];
        fs << "}";
    }
    fs << "}";
    fs.release();
}

QVector<ObjectInfo> ObjectInfoHandler::getObjectsIn(int frameNumber)
{
    int lower = frameNumber - 30;
    lower = lower < 0 ? 0 : lower;

    int upper = frameNumber + 30;

    QVector<ObjectInfo> objectsInFrame;

    for(int i = 0; i < _objectList.size(); ++i)
    {
        if(_objectList[i].frameNumber()==frameNumber)
        {
            objectsInFrame.push_back(_objectList[i]);
        }
    }

    for(int i = 0; i < _objectList.size(); ++i)
    {
        if(lower < _objectList[i].frameNumber() &&
                _objectList[i].frameNumber() < upper){
            if(!isObjectInList(objectsInFrame,_objectList[i]))
            {
                objectsInFrame.push_back(_objectList[i]);
            }
        }
    }
    return objectsInFrame;
}

void ObjectInfoHandler::loadObjectInfo()
{
    cv::FileStorage fs(_filename, cv::FileStorage::READ);
    if(!fs.isOpened())
    {
        _open = false;
        return;
    }
    QVector<ObjectInfo> toReturn;
    cv::FileNode objects = fs["Objects"];
    cv::FileNodeIterator it = objects.begin(), end = objects.end();
    for(; it!= end; ++it)
    {
        cv::FileNode node = objects[((cv::FileNode)*it).name()];

        ObjectInfo t;
        node >> t;
        toReturn.push_back(t);
    }
    _objectList = toReturn;
    _open = true;
}

bool ObjectInfoHandler::isObjectInList(QVector<ObjectInfo> list, ObjectInfo ob)
{
    for(int i = 0; i < list.size(); ++i)
    {
        if(list[i].name() == ob.name())
        {
            return true;
        }
    }
    return false;
}

std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}
}
