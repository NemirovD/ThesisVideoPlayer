#ifndef UTILS_H
#define UTILS_H

#include <map>
#include <QTime>
#include <opencv2/opencv.hpp>
#include <iostream>

namespace ul{

class Annotation
{
public:
    Annotation();
    Annotation(std::string,cv::Rect,int);
    Annotation(std::string,std::string,cv::Rect,int);
    ~Annotation();
    bool isEmpty();
    friend cv::FileStorage& operator<<(cv::FileStorage& fs, const Annotation ann);
    friend std::ostream& operator<<(std::ostream& os, const Annotation ann);

    //getters and setters
    std::string getName(){return name;}
    std::string getWebpage(){return webpage;}
    cv::Rect getRect(){return rect;}
    int getFrameNo(){return frameno;}
    void setName(std::string name){this->name = name;}
    void setWebpage(std::string page){this->webpage = page;}
    void setRect(cv::Rect rect){this->rect = rect;}
    void setFrameNo(int frame){this->frameno=frame;}
private:
    std::string name;
    std::string webpage;
    cv::Rect rect;
    int frameno;
};

class AnnotationLoader
{
public:
    AnnotationLoader();
    AnnotationLoader(std::string filename);

    //getters
    std::vector<ul::Annotation> getFrameAnnotations(int frameNumber);

    //setters
    void setAnnotationFile(std::string filename);

    //state checkers
    bool isOpened() const;
private:
    bool open;
    bool isAnnotationBeingDisplayed(ul::Annotation an,std::vector<ul::Annotation> v);
    std::string currentFile;
    std::map<int,std::vector<ul::Annotation>> annotations;
};

class FormatTime
{
public:
    FormatTime();
    FormatTime(int cur, int total);

    //getters
    std::string getFormatTime();

    //setters
    void setTotalTime(int totalSeconds);
    void setCurrentTime(int currentSeconds);

    //updaters
    std::string getFormatTime(int currentSeconds);
private:
    int currentTime;
    int totalTime;
    std::string convertTime(int seconds);
};

struct TrackerEditInfo{
    bool trackNoIsValid;
    int trackerno;
    int x_0;
    int y_0;
    bool mouseDown;
};

void MapToVec(const std::map<int,std::vector<ul::Annotation>>& m, std::vector<ul::Annotation>& v);
std::string getFileNameWithoutExtension(std::string filename);
std::vector<std::string> split(const std::string &s, char delim);
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
}

#endif // UTILS_H
