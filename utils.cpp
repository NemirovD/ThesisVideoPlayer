#include "utils.h"
namespace ul{

Annotation::Annotation()
{
    this->name = "";
    this->frameno = 0;
    this->rect = cv::Rect();
}

Annotation::Annotation(std::string name, cv::Rect rect, int frameno)
{
    this->name = name;
    this->frameno = frameno;
    this->rect = rect;
    this->webpage = "";
}

Annotation::Annotation(std::string name, std::string page, cv::Rect rect, int frameno)
{
    this->name = name;
    this->frameno = frameno;
    this->rect = rect;
    this->webpage = page;
}

Annotation::~Annotation()
{
}

bool Annotation::isEmpty()
{
    if(this->name.empty())
    {
        return true;
    }
    return false;
}

cv::FileStorage& operator<<(cv::FileStorage& fs, const Annotation ann)
{
    fs << "name" << ann.name;
    fs << "webpage" << ann.webpage;
    fs << "frameno" << ann.frameno;
    fs << "rect" << ann.rect;

    return fs;
}

std::ostream& operator<<(std::ostream& os, const Annotation ann)
{
    os << ann.name << std::endl;
    os << ann.webpage << std::endl;
    os << ann.frameno << std::endl;
    os << ann.rect.x << " " << ann.rect.y << " " << ann.rect.width << " " << ann.rect.height << std::endl;
    return os;
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
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

void MapToVec(const std::map<int,std::vector<ul::Annotation>>& m, std::vector<ul::Annotation>& v)
{
    for(std::map<int,std::vector<ul::Annotation>>::const_iterator it = m.begin(); it!=m.end(); ++it)
    {
        for(std::vector<ul::Annotation>::const_iterator ij = it->second.begin(); ij!=(it->second).end(); ++ij)
        {
            v.push_back(*ij);
        }
    }
    return;
}

FormatTime::FormatTime()
{
    this->currentTime = 0;
    this->totalTime = 0;
}

FormatTime::FormatTime(int cur, int total)
{
    this->currentTime = cur;
    this->totalTime = total;
}

std::string FormatTime::getFormatTime()
{
    return convertTime(currentTime)+" / "+convertTime(totalTime);
}

void FormatTime::setTotalTime(int totalSeconds)
{
    this->totalTime = totalSeconds;
}

void FormatTime::setCurrentTime(int currentSeconds)
{
    this->currentTime = currentSeconds;
}

std::string FormatTime::getFormatTime(int currentSeconds)
{
    this->setCurrentTime(currentSeconds);
    return this->getFormatTime();
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

AnnotationLoader::AnnotationLoader()
{
    currentFile = "";
}

AnnotationLoader::AnnotationLoader(std::string filename)
{
    setAnnotationFile(filename);
}

std::vector<ul::Annotation> AnnotationLoader::getFrameAnnotations(int frameNumber)
{
    int start = frameNumber - 30;
    if(start < 0)
        start = 0;

    int end = frameNumber + 30;

    std::vector<ul::Annotation> rectsToDraw;
    if(annotations.count(frameNumber))
    {
        std::vector<ul::Annotation> v = this->annotations[frameNumber];
        for(unsigned int i = 0; i < v.size(); ++i)
        {
            rectsToDraw.push_back(v[i]);
        }
    }

    for(int i = frameNumber; i < end; ++i)
    {
        if(annotations.count(i))
        {
            std::vector<ul::Annotation> v = annotations[i];
            for(unsigned int j = 0; j < v.size(); ++j)
            {
                if(!isAnnotationBeingDisplayed(v[j],rectsToDraw))
                {
                    rectsToDraw.push_back(v[j]);
                }
            }
        }
    }

    for(int i = frameNumber; i > start; --i)
    {
        if(annotations.count(i))
        {
            std::vector<ul::Annotation> v = annotations[i];
            for(unsigned int j = 0; j < v.size(); ++j)
            {
                if(!isAnnotationBeingDisplayed(v[j],rectsToDraw))
                {
                    rectsToDraw.push_back(v[j]);
                }
            }
        }
    }
    return rectsToDraw;
}

void AnnotationLoader::setAnnotationFile(std::string filename)
{
    cv::FileStorage fs(filename, cv::FileStorage::READ);
    if(!fs.isOpened())
    {
        open = false;
    }
    cv::FileNode n = fs["Annotations"];
    std::map<int,std::vector<ul::Annotation>> toSet;

    cv::FileNodeIterator it = n.begin(), it_end = n.end();
    for(; it!= it_end; ++it)
    {
        cv::FileNode ni = n[((cv::FileNode)*it).name()];

        cv::Rect r;
        cv::FileNode rn = ni["rect"];
        cv::FileNodeIterator rni = rn.begin();


        r.x = *rni;
        r.y = *(++rni);
        r.width = *(++rni);
        r.height = *(++rni);

        ul::Annotation ret((std::string)ni["name"],
                           (std::string)ni["webpage"],
                           r,
                           (int)ni["frameno"]);
        toSet[(int)ni["frameno"]].push_back(ret);
    }
    annotations = toSet;
}

bool AnnotationLoader::isOpened() const
{
    return open;
}

bool AnnotationLoader::isAnnotationBeingDisplayed(Annotation an, std::vector<Annotation> v)
{
    for(unsigned int i = 0; i < v.size(); ++i)
    {
        if(an.getName().compare(v[i].getName())==0)
        {
            return true;
        }
    }
    return false;
}
}
